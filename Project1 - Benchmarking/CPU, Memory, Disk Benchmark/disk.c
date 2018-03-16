#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Global variables */
long noOfIterations;
int blockSize;
const int allocationSize = 10*1024;

int threadCount;
pthread_t *threadArray;
FILE *f;
FILE *f1;
FILE *f2;

double exeTime;
double latency = 0.0;
double throughput = 0.0;

char *buff;
const int mbSize = 1024*1024;

/* Function Declaration*/
void initialization(char threadOption,char blockOption);
void measureOperationPerformance(char option);
void *seqWrite();
void *randomWrite();
void *seqRead();
void *randomRead();
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
	printf("\n a) Sequential read");
	printf("\n b) Sequential write");
	printf("\n c) Random read");
	printf("\n d) Random write");
	printf("\n e) Sequential read write");

	printf("\n USER INPUT (a, b, c, d or e) : ");

	scanf(" %c", &operationOption);
	*/

	

	f = fopen("readWriteMemory.txt", "a+");

	char th[] = "abcd";
	char bl[] = "abcd";
	char op[] = "bdeac";
	
	
	fprintf(f,"\n\n ***************************************************** --- START -----  **************************************************** ");
	int x=0, y=0, z=0;
	for(x=0; op[x] != '\0'; x++) {
		operationOption = op[x];

		for(y=0; op[y] != '\0'; y++) {
		blockOption = bl[y];
	
			for(z=0; th[z] != '\0'; z++) {
			threadOption = th[z];

			f1 = fopen("tempdisk.txt", "w+");
			fseek(f1, allocationSize * mbSize , SEEK_SET);
			fputc('\0', f1);
			rewind(f1);

			if (f == NULL)
			{
			    printf("\n\nError opening file!\n");
			    
			} else {

				if (threadOption != 'a' && threadOption != 'b' && threadOption != 'c' && threadOption != 'd') {
					printf("\n \n ERROR: Invalid thread input.");
				} else if (blockOption != 'a' && blockOption != 'b' && blockOption != 'c' && blockOption != 'd') {
					printf("\n \n ERROR: Invalid block input.");
				} else if (operationOption != 'a' && operationOption != 'b' && operationOption != 'c' && operationOption != 'd' && operationOption != 'e') {
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
			fclose(f1);

			}
		}
	}
 	fclose(f);
	printf("\n\n");

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
			noOfIterations = 8000000/threadCount;
			printf(" Block: 8 byte ");
			fprintf(f," Block: 8 byte ");
			break;

		case 'b':
			// 8 Kilobyte
			blockSize = 8*1024;
			noOfIterations = 80000/threadCount;
			printf(" Block: 8 Kilobyte ");
			fprintf(f," Block: 8 Kilobyte ");
			break;

		case 'c':
			// 8 MB
			blockSize = 8*1024*1024;
			noOfIterations = 80/threadCount;
			printf(" Block: 8 MB ");
			fprintf(f," Block: 8 MB ");
			break;

		case 'd':
			// 80 MB
			blockSize = 80*1024*1024;
			noOfIterations = 24/threadCount;
			printf(" Block: 80 MB ");
			fprintf(f," Block: 80 MB ");
			break;
		default:
			printf("Invalid Option\n");
	}
	
	int i=0;
	buff = malloc(sizeof(char)*blockSize);
	for(i=0; i<blockSize; i++){
		buff[i] = 'r';
	}
}

/*
 	Benkchmarking using FLOPS.
*/
void measureOperationPerformance(char option) {


	switch (option) {
		case 'a':
			printf(" \n Operation: Sequential read ");
			fprintf(f," \n Operation: Sequential read ");
			break;

		case 'b':
			printf("\n Operation: Sequential write ");
			fprintf(f," \n Operation: Sequential write ");
			break;

		case 'c':
			printf(" \n Operation: Random read ");
			fprintf(f," \n Operation: Random read ");
			break;

		case 'd':
			printf(" \n Operation: Random write ");
			fprintf(f," \n Operation: Random write ");
			break;

		case 'e':
			printf(" \n Operation: Sequential read write ");
			fprintf(f," \n Operation: Sequential read write ");
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
				pthread_create(&threadArray[i],NULL,seqWrite, "");
			}
			break;

		case 'b':
			for(i=0; i<threadCount; i++) {
				pthread_create(&threadArray[i],NULL,seqRead, "");
			}
			break;

		case 'c':
			for(i=0; i<threadCount; i++) {
				pthread_create(&threadArray[i],NULL,randomWrite, "");
			}
			break;

		case 'd':
			for(i=0; i<threadCount; i++) {
				pthread_create(&threadArray[i],NULL,randomRead, "");
			}
			break;

		case 'e':
			for(i=0; i<threadCount; i++) {
				pthread_create(&threadArray[i],NULL,seqReadWrite, "");
			}
			break;

		default:
			printf("Invalid Option\n");
	}

	i =0;
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

	free(buff);

}

void *seqWrite() {
	int i=0;
	for (i=0; i<noOfIterations; i++){
		fwrite(buff, 1, blockSize, f1);
	}
}


void *randomWrite() {
	int i=0;
	for(i=0; i<noOfIterations; i++) {
		fseek (f1, (rand()% ((allocationSize * mbSize) - (blockSize + 1))), SEEK_SET);
		fwrite(buff, 1, blockSize, f1);
	}
}


void *seqRead() {
	int i=0;
	for (i=0; i<noOfIterations; i++){
		fread(buff, 1, blockSize, f1);
	}
}


void *randomRead() {
	int i=0;
	for(i=0; i<noOfIterations; i++) {
		fseek (f1, (rand()% ((allocationSize * mbSize) - (blockSize + 1))), SEEK_SET);
		fread(buff, 1, blockSize, f1);
	}
}

void *seqReadWrite() {
	int i=0;

	f2 = f1;
	fseek (f2, (rand()% ((allocationSize * mbSize) - (noOfIterations*blockSize*threadCount + 1))), SEEK_SET);
	for (i=0; i<noOfIterations; i++){
		fread(buff, 1, blockSize, f1);
		fwrite(buff, 1, blockSize, f2);
	}
}


