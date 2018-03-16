#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/* Global variables */
long noOfIterations;
int blockSize;

const int mbSize = 1024*1024;
const int allocationSize = 1024*1024*1024;
int threadCount;
pthread_t *threadArray;
FILE *f;

double exeTime;
double latency = 0.0;
double throughput = 0.0;

char *memSection1;
char *memSection2;


/* Function Declaration*/
void initialization(char threadOption,char blockOption);
void measureOperationPerformance(char option);
void *seqWrite();
void *randomWrite();
void *seqReadWrite();


int main()
{
	char threadOption;
	char blockOption;
	char operationOption;
	
 
	/*printf("Please select the number of threads option:");
	printf("\n a) 1 Thread");
	printf("\n b) 2 Thread");
	printf("\n c) 4 Thread");
	printf("\n d) 8 Thread");
	printf("\n USER INPUT (a, b, c or d) : ");

	scanf(" %c", &threadOption);
	
	printf("\n\nPlease select the block size option:");
	printf("\n a) 8 B");
	printf("\n b) 8 KB");
	printf("\n c) 8 MB");
	printf("\n d) 80 MB");
	printf("\n USER INPUT (a, b, c or d) : ");

	scanf(" %c", &blockOption);

	printf("\n\nPlease select the operation option:");
	printf("\n a) Sequential read write");
	printf("\n b) Sequential write");
	printf("\n c) Random write");
	printf("\n USER INPUT (a, b or c) : ");

	scanf(" %c", &operationOption);*/

	f = fopen("readWriteMemory.txt", "a+");

	char th[] = "abcd";
	char bl[] = "abcd";
	char op[] = "abc";
	
	
	fprintf(f,"\n\n ***************************************************** --- START -----  **************************************************** ");
	int x=0, y=0, z=0;
	for(x=0; op[x] != '\0'; x++) {
		operationOption = op[x];

		for(y=0; op[y] != '\0'; y++) {
		blockOption = bl[y];
	
			for(z=0; th[z] != '\0'; z++) {
			threadOption = th[z];
	
			
			if (f == NULL)
			{
			    printf("\n\nError opening file!\n");
			    
			} else {

				if (threadOption != 'a' && threadOption != 'b' && threadOption != 'c' && threadOption != 'd') {
					printf("\n \n ERROR: Invalid thread input.");
				} else if (blockOption != 'a' && blockOption != 'b' && blockOption != 'c' && blockOption != 'd') {
					printf("\n \n ERROR: Invalid block input.");
				} else if (operationOption != 'a' && operationOption != 'b' && operationOption != 'c') {
					printf("\n \n ERROR: Invalid operation input.");
				} else {
					printf("\n\nBanchmarking process started (It takes several minutes) ... ");
					fprintf(f,"\n\nBanchmarking process started (It takes several minutes) ... ");
					initialization(threadOption, blockOption);
			
					fprintf(f,"\n\n ******************************************************************************************************************************************************* ");
					printf("\n\n ******************************************************************************************************************************************************* ");

					measureOperationPerformance(operationOption);
			
					printf("\n\n ******************************************************************************************************************************************************* ");
					fprintf(f,"\n\n ******************************************************************************************************************************************************* ");
		
				}
				
			}
			printf("\n\n");
			}
		}
	}
	fclose(f);

	return 0;
}

/* 
	Initialize all the values
*/
void initialization(char threadOption,char blockOption) {

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

	switch (blockOption) {
		case 'a':
			// 8 byte
			blockSize = 8;
			noOfIterations = 800000000/threadCount;
			printf(" Block: 8 byte ");
			fprintf(f," Block: 8 byte ");
			break;

		case 'b':
			// 8 Kilobyte
			blockSize = 8*1024;
			noOfIterations = 16000000/threadCount;
			printf(" Block: 8 Kilobyte ");
			fprintf(f," Block: 8 Kilobyte ");
			break;

		case 'c':
			// 8 MB
			blockSize = 8*1024*1024;
			noOfIterations = 80000/threadCount;
			printf(" Block: 8 MB ");
			fprintf(f," Block: 8 MB ");
			break;

		case 'd':
			// 80 MB
			blockSize = 80*1024*1024;
			noOfIterations = 1600/threadCount;
			printf(" Block: 80 MB ");
			fprintf(f," Block: 80 MB ");
			break;
		default:
			printf("Invalid Option\n");
	}

	memSection1 = malloc(allocationSize);
	memSection2 = malloc(allocationSize);
	
}

/*
 	Benkchmarking using FLOPS.
*/
void measureOperationPerformance(char option) {
	
	switch (option) {
		case 'a':
			printf(" \n Operation: Sequential read write ");
			fprintf(f," \n Operation: Sequential read write ");
			break;

		case 'b':
			printf(" \n Operation: Sequential write ");
			fprintf(f," \n Operation: Sequential write ");
			break;

		case 'c':
			printf(" \n Operation: Random write ");
			fprintf(f," \n Operation: Random write ");
			break;

		default:
			printf("Invalid Option\n");
	}

	struct timespec tstart={0,0}, tend={0,0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);  
	int i=0; 

	switch (option) {
		case 'a':
					
			for(i=0; i<threadCount; i++) {
				pthread_create(&threadArray[i],NULL,seqReadWrite, "");
			}
			break;

		case 'b':
			for(i=0; i<threadCount; i++) {
				pthread_create(&threadArray[i],NULL,seqWrite, "");
			}
			break;

		case 'c':
			for(i=0; i<threadCount; i++) {
				pthread_create(&threadArray[i],NULL,randomWrite, "");
			}
			break;

		default:
			printf("Invalid Option\n");
	}

	i = 0;
	/* wait for our thread to finish before continuing */
	for(i=0; i<threadCount; i++) {
		pthread_join(threadArray[i], NULL);
	}

	clock_gettime(CLOCK_MONOTONIC, &tend);


	exeTime =  ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
	
	throughput = (noOfIterations * blockSize * threadCount) / (exeTime * mbSize);
	latency = (exeTime * 1000000)/ (noOfIterations * threadCount);
	
	printf("\n Operation execution time:  %.5f seconds\n",
		   exeTime);
	printf("\n Latency:  %.5f microsec\n", latency);
	printf("\n Throughput:  %.5f  MB/s\n", throughput);
	

	fprintf(f,"\n FLOP execution time:  %.5f seconds\n",
		   exeTime);
	fprintf(f,"\n Latency:  %.5f microsec \n", latency);
	fprintf(f,"\n Throughput:  %.5f MB/s\n", throughput);

	free(memSection1);
	free(memSection2);

}

void *seqReadWrite() {
	int indexVal = 0;
	int i=0;
	for (i=0; i<noOfIterations;i++){
		memcpy(&memSection1[indexVal], &memSection2[indexVal], blockSize);
		indexVal = (indexVal + blockSize) % (allocationSize);
	}
}


void *seqWrite() {
	int indexVal = 0;
	int i=0;
	for (i=0; i<noOfIterations;i++){
		memset(&memSection1[indexVal], '$', blockSize);
		indexVal = (indexVal + blockSize) % (allocationSize);
	}
}


void *randomWrite() {
	int indexVal = 0;
	int i=0;
	for (i=0; i<noOfIterations;i++){
		memset(&memSection1[indexVal], '$', blockSize);
		indexVal =  rand() % (allocationSize - (blockSize + 1)) ;
	}
}






