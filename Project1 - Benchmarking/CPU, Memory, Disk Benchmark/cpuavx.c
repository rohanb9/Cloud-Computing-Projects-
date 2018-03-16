#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <immintrin.h>

/* Global variables */
long noOfIterations = 888888888;
const int operationsPerIteration = 32;
int threadCount;
pthread_t *threadArray;
FILE *f;

double doubleVal;
double exeFLOPTime = 0.0;
double gigaFlops;
double flops;

int intVal;
double exeIOPTime = 0.0;
double gigaIops;
double iops;


/* Function Declaration*/
void initialization(char threadOption);
void measurePerformanceUsingFLOPS();
void measurePerformanceUsingIOPS() ;
void *flopOperations();
void *iopOperations();


int main()
{
	char threadOption;
	
 	noOfIterations = noOfIterations*3;
	/*printf("Please select the number of threads option:");
	printf("\n a) 1 Thread");
	printf("\n b) 2 Thread");
	printf("\n c) 4 Thread");
	printf("\n d) 8 Thread");
	printf("\n USER INPUT (a, b, c or d) : ");

	scanf("%c", &threadOption);
	*/
	f = fopen("CPUPerformaceAVX.txt", "a");

	fprintf(f,"\n\n ***************************************************** --- START -----  **************************************************** ");
	int x=0, y=0, z=0;
	char th[] = "abcd";
	for(x=0; th[x] != '\0'; x++) {
		threadOption = th[x];

		if (f == NULL)
		{
		    printf("\n\nError opening file!\n");
		    
		} else {

			if (threadOption != 'a' && threadOption != 'b' && threadOption != 'c' && threadOption != 'd') {
				printf("\n \n ERROR: Invalid input.");
			} else {
				printf("\n\nBanchmarking process started (It takes several minutes) ... ");
				fprintf(f,"\n\nBanchmarking process started (It takes several minutes) ... ");
				initialization(threadOption);
			
				fprintf(f,"\n\n ******************************************************************************************************************************************************* ");
				printf("\n\n ******************************************************************************************************************************************************* ");
				printf("\n\nAnalysing using double precision FLOPS ... ");
				fprintf(f,"\n\nAnalysing using double precision FLOPS ... ");
				measurePerformanceUsingFLOPS();
				printf("\nBanchmarking using FLOPS results are ready. Please check performance report file.");
				fprintf(f,"\nBanchmarking using FLOPS process completed.");
			
				fprintf(f,"\n\n --------------------------------------------------------------------------------------------------------------------------- ");	
				printf("\n\n --------------------------------------------------------------------------------------------------------------------------- ");		

				printf("\n\nAnalysing using IOPS ... ");
				fprintf(f,"\n\nAnalysing using IOPS ... ");
				measurePerformanceUsingIOPS();
				printf("\nBanchmarking using IOPS results are ready. Please check performance report file.");
				fprintf(f,"\nBanchmarking using IOPS process completed.");
				printf("\n\n ******************************************************************************************************************************************************* ");
				fprintf(f,"\n\n ******************************************************************************************************************************************************* ");
		
			}
			
		}
		printf("\n\n");
	}
	fclose(f);

	return 0;
}

/* 
	Initialize all the values
*/
void initialization(char threadOption) {

	switch (threadOption) {
		case 'a':
			threadCount = 1;
			threadArray =  malloc(sizeof(pthread_t)*threadCount);
			printf("Using 1 threads.");
			fprintf(f,"Using 1 threads.");
			break;

		case 'b':
			threadCount = 2;
			threadArray =  malloc(sizeof(pthread_t)*threadCount);
			printf("Using 2 threads.");
			fprintf(f,"Using 2 threads.");
			break;

		case 'c':
			threadCount = 4;
			threadArray =  malloc(sizeof(pthread_t)*threadCount);
			printf("Using 4 threads.");
			fprintf(f,"Using 4 threads.");
			break;

		case 'd':
			threadCount = 8;
			threadArray =  malloc(sizeof(pthread_t)*threadCount);
			printf("Using 8 threads.");
			fprintf(f,"Using 8 threads.");
			break;
		default:
			printf("Invalid Option\n");
	}
}

/*
 	Benkchmarking using FLOPS.
*/
void measurePerformanceUsingFLOPS() {

	noOfIterations = noOfIterations / threadCount;
	
	struct timespec tstart={0,0}, tend={0,0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);   
	int i=0;
	for(i=0; i<threadCount; i++) {
		pthread_create(&threadArray[i],NULL,flopOperations, "");
	}

	/* wait for our thread to finish before continuing */
	i=0;
	for(i=0; i<threadCount; i++) {
		pthread_join(threadArray[i], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &tend);
	exeFLOPTime =  ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
	
	flops = (noOfIterations*operationsPerIteration*threadCount) / exeFLOPTime;
	gigaFlops = flops / 1000000000;
	
	printf("\n FLOP execution time:  %.5f seconds\n",
		   ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	printf("\n Floating point operations per second:  %.5f \n", flops);
	printf("\n Giga Floating point operations per second:  %.5f \n", gigaFlops);
	

	fprintf(f,"\n FLOP execution time:  %.5f seconds\n",
		   ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	fprintf(f,"\n Floating point operations per second:  %.5f \n", flops);
	fprintf(f,"\n Giga Floating point operations per second:  %.5f \n", gigaFlops);

}

void *flopOperations() {

	__m256 vecfa = _mm256_set_ps(6.99999, 7.99999, 8.99999, 9.99999, 10.99999, 11.99999, 12.99999, 13.999999);
	__m256 vecfb = _mm256_set_ps(14.45685679, 15.54687899, 16.7687899, 17.768689, 18.6786789, 19.7869, 20.9, 21.99969);
	__m256 vecfc = _mm256_set_ps(134.456879, 14345.5463487899, 1346.763487899, 1347.76834689, 1834.6786789, 1349.7869, 3420.9, 23431.949969);
	__m256 vecfr = _mm256_set_ps(22.5675889, 23.658879, 24.9656454645, 25.546564569, 26.4564564569, 27.5665449, 28.54656459, 29.546456459);
	int i=0;
	for(i=0; i<noOfIterations; i++) {
		//printf("%d sd", i);
		_mm256_add_ps(vecfa, vecfb);
		_mm256_add_ps(vecfc, vecfb);
		_mm256_add_ps(vecfr, vecfb);
		_mm256_add_ps(vecfr, vecfa);
	}
}



/*
 	Benkchmarking using IOPS.
*/
void measurePerformanceUsingIOPS() {
	
	noOfIterations = noOfIterations / threadCount;
	
	struct timespec tstart={0,0}, tend={0,0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);   
	int i=0;
	for(i=0; i<threadCount; i++) {
		pthread_create(&threadArray[i],NULL,iopOperations, "");
	}

	/* wait for our thread to finish before continuing */
	i = 0;
	for(i=0; i<threadCount; i++) {
		pthread_join(threadArray[i], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &tend);
	exeIOPTime =  ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
	
	iops = (noOfIterations*operationsPerIteration*threadCount) / exeIOPTime;
	gigaIops = iops / 1000000000;
	
	printf("\n IOP execution time:  %.5f seconds\n",
		   ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	printf("\n Integer operations per second:  %.5f \n", iops);
	printf("\n Giga Integer operations per second:  %.5f \n", gigaIops);

	//fprintf(f,"\n IOP execution time:  %.5f seconds\n",
		   ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	fprintf(f,"\n Integer operations per second:  %.5f \n", iops);
	fprintf(f,"\n Giga Integer operations per second:  %.5f \n", gigaIops);
}

void *iopOperations() {
	
	__m256i veca = _mm256_set_epi32(13236, 24234246, 3324426, 4324326, 5246, 64326, 72346, 823423326);
	__m256i vecb = _mm256_set_epi32(92341, 102342, 1133, 1342424, 13436, 142346, 123456, 132466);
	__m256i vecc = _mm256_set_epi32(492341, 4102342, 1324133, 13342424, 123436, 1242346, 4123456, 4132466);
	__m256i vecr = _mm256_set_epi32(173431, 14482, 13493, 2342204, 34216, 223426, 23436, 24436);
	int i=0;
	for(i=0; i<noOfIterations; i++) {
		_mm256_add_epi32(veca, vecb);
		_mm256_add_epi32(vecb, vecc);
		_mm256_add_epi32(vecr, veca);
		_mm256_add_epi32(vecr, vecb);
	}
}





