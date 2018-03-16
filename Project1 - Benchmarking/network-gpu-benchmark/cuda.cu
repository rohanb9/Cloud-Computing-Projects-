#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

double finalGval;
struct timeval t;

__global__ void DoubleAdd(double *d_a,int threadCount,int loopcount)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if (i < threadCount)
	{
		for (int j = 0; j < loopcount; ++j)
		{
                        d_a[i] = d_a[i] + 0.0000001;
						d_a[i] = 0.0000001 + 0.0000001;
						d_a[i] = d_a[i] + 0.0000001;
						d_a[i] = 0.0000002 + 0.0000001;
						d_a[i] = d_a[i] + 0.0000001;
						d_a[i] = 0.0000003 + 0.0000001;
						d_a[i] = 0.0000004 + 0.0000001;
						d_a[i] = 0.0000001 + 0.0000001;
						d_a[i] = d_a[i] + 0.0000001;
						d_a[i] = d_a[i] + 0.0000001;
						d_a[i] = 0.0000002 + 0.0000001;
						d_a[i] = d_a[i] + 0.0000001;
						d_a[i] = 0.0000005 + 0.0000001;
						d_a[i] = 0.0000001 + 0.0000001;
						d_a[i] = d_a[i] + 0.0000001;
						
		}
	}
}

__global__ void IntRunner(int *i_a,int threadCount,int loopcount)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if (i < threadCount)
	{
		for (int j = 0; j < loopcount; ++j)
		{
                        i_a[i] = i_a[i] + 41000;
						i_a[i] = 41000 + 1;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 2 + 41000;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 3 + 41000;
						i_a[i] = 4 + 41000;
						i_a[i] = 1 + 41000;
						i_a[i] = i_a[i] + 1;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 2 + 41000;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 5 + 41000;
						i_a[i] = 1 + 41000;
						i_a[i] = i_a[i] + 1;
						
		}
	}
}

__global__ void ShortRunner(short *i_a,int threadCount,int loopcount)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if (i < threadCount)
	{
		for (int j = 0; j < loopcount; ++j)
		{
                        i_a[i] = i_a[i] + 30000;
						i_a[i] = 1 + 30000;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 2 + 30000;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 3 + 30000;
						i_a[i] = 4 + 30000;
						i_a[i] = 1 + 30000;
						i_a[i] = i_a[i] + 1;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 2 + 30000;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 5 + 30000;
						i_a[i] = 1 + 30000;
						i_a[i] = i_a[i] + 1;
						
		}
	}
}

__global__ void CharRunner(char *i_a,int threadCount,int loopcount)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if (i < threadCount)
	{
		for (int j = 0; j < loopcount; ++j)
		{
                        i_a[i] = i_a[i] + 1;
						i_a[i] = 1 + 1;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 2 + 1;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 3 + 1;
						i_a[i] = 4 + 1;
						i_a[i] = 1 + 1;
						i_a[i] = i_a[i] + 1;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 2 + 1;
						i_a[i] = i_a[i] + 1;
						i_a[i] = 5 + 1;
						i_a[i] = 1 + 1;
						i_a[i] = i_a[i] + 1;
						
		}
	}
}

int main(int argc, char *argv[])
{

	int cudaCores, totalSm, threadCount, loopcount;

			cudaSetDevice(0);
			totalSm = 56;
			cudaCores = 3584;
			threadCount = totalSm * cudaCores;
			loopcount = atoi(argv[1]); // loop count
			double *fa, *dfa;
			int *ia, *dia;
			short *sa, *dsa;
			char *ca, *dca;
			double startTime, endTime, curr_time;
			
			//FLOPS
			
			fa = (double *)malloc(threadCount*sizeof(double));
			cudaMalloc(&dfa, threadCount*sizeof(double));
			
			for(int i = 0; i < threadCount; ++i)
			{
				fa[i] = 0.00001;
			}
			
			cudaMemcpy(dfa, fa, threadCount*sizeof(double), cudaMemcpyHostToDevice);
			
			gettimeofday(&t, NULL);
			startTime = t.tv_sec+(t.tv_usec/1000000.0);
			DoubleAdd<<< totalSm, cudaCores >>>(dfa,threadCount,loopcount);
			cudaThreadSynchronize();
			gettimeofday(&t, NULL);
			endTime = t.tv_sec+(t.tv_usec/1000000.0);
			curr_time = endTime - startTime;
			finalGval = (loopcount*totalSm*cudaCores*32*10)/(curr_time*1000000000);
			free(fa);
			cudaFree(dfa);
			printf(" FLOPs: %lf G-FLOPs\n", finalGval);
			
			//IOPS 
			
			ia = (int *)malloc(threadCount*sizeof(int));
			cudaMalloc(&dia, threadCount*sizeof(int));
			
			for(int i = 0; i < threadCount; ++i)
			{
				ia[i] = 1;
			}
			
			cudaMemcpy(dia, ia, threadCount*sizeof(int), cudaMemcpyHostToDevice);
			
			gettimeofday(&t, NULL);
			startTime = t.tv_sec+(t.tv_usec/1000000.0);
			IntRunner<<< totalSm, cudaCores >>>(dia,threadCount,loopcount);
			cudaThreadSynchronize();
			gettimeofday(&t, NULL);
			endTime = t.tv_sec+(t.tv_usec/1000000.0);
			curr_time = endTime - startTime;
			finalGval = (loopcount*totalSm*cudaCores*32*10)/(curr_time*1000000000);
			free(ia);
			cudaFree(dia);
			printf("IOPS: %lf G-IOPS\n", finalGval);
			
			//HOPS
			
			sa = (short *)malloc(threadCount*sizeof(short));
			cudaMalloc(&dsa, threadCount*sizeof(short));
			
			for(int i = 0; i < threadCount; ++i)
			{
				sa[i] = 1;
			}
			
			cudaMemcpy(dsa, sa, threadCount*sizeof(short), cudaMemcpyHostToDevice);
			
			gettimeofday(&t, NULL);
			startTime = t.tv_sec+(t.tv_usec/1000000.0);
			ShortRunner<<< totalSm, cudaCores >>>(dsa,threadCount,loopcount);
			cudaThreadSynchronize();
			gettimeofday(&t, NULL);
			endTime = t.tv_sec+(t.tv_usec/1000000.0);
			curr_time = endTime - startTime;
			finalGval = (loopcount*totalSm*cudaCores*32*10)/(curr_time*1000000000);
			free(sa);
			cudaFree(dsa);
			printf(" HOPS: %lf G-HOPS\n", finalGval);
			
			//QOPS
			
			ca = (char *)malloc(threadCount*sizeof(char));
			cudaMalloc(&dca, threadCount*sizeof(char));
			
			for(int i = 0; i < threadCount; ++i)
			{
				ca[i] = 1;
			}
			
			cudaMemcpy(dca, ca, threadCount*sizeof(char), cudaMemcpyHostToDevice);
			
			gettimeofday(&t, NULL);
			startTime = t.tv_sec+(t.tv_usec/1000000.0);
			CharRunner<<< totalSm, cudaCores >>>(dca,threadCount,loopcount);
			cudaThreadSynchronize();
			gettimeofday(&t, NULL);
			endTime = t.tv_sec+(t.tv_usec/1000000.0);
			curr_time = endTime - startTime;
			finalGval = (loopcount*totalSm*cudaCores*32*10)/(curr_time*1000000000);
			free(ca);
			cudaFree(dca);
			printf("QOPS: %lf G-QOPS\n", finalGval);

			
			
	return 0;
}