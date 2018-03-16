#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Global variables */
long noOfIterations = 800000000;
const int operationsPerIteration = 25;
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
 
	printf("Please select the number of threads option:");
	printf("\n a) 1 Thread");
	printf("\n b) 2 Thread");
	printf("\n c) 4 Thread");
	printf("\n d) 8 Thread");
	printf("\n USER INPUT (a, b, c or d) : ");

	scanf("%c", &threadOption);

	f = fopen("CPUPerformace.txt", "a");
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
		fclose(f);
	}
	printf("\n\n");

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
	
	for(int i=0; i<threadCount; i++) {
		pthread_create(&threadArray[i],NULL,flopOperations, "");
	}

	/* wait for our thread to finish before continuing */
	for(int i=0; i<threadCount; i++) {
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
	for(int i=0; i<noOfIterations; i++) {
		//printf("%d sd", i);
		doubleVal = doubleVal + 1.999999
			+ 2.999999 + 3.999999
			+ 4.999999 + 5.999999
			+ 6.999999 + 7.999999
			+ 8.999999 + 9.999999
			+ 10.999999 + 11.999999
			+ 12.999999 + 13.999999
			+ 14.999999 + 15.999999
			+ 16.999999 + 17.999999
			+ 18.999999 + 19.999999
			+ 20.999999 + 21.999999
			+ 22.999999 + 23.999999;
	}
}



/*
 	Benkchmarking using IOPS.
*/
void measurePerformanceUsingIOPS() {
	
	noOfIterations = noOfIterations / threadCount;
	
	struct timespec tstart={0,0}, tend={0,0};
	clock_gettime(CLOCK_MONOTONIC, &tstart);   
	
	for(int i=0; i<threadCount; i++) {
		pthread_create(&threadArray[i],NULL,iopOperations, "");
	}

	/* wait for our thread to finish before continuing */
	for(int i=0; i<threadCount; i++) {
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

	fprintf(f,"\n IOP execution time:  %.5f seconds\n",
		   ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	fprintf(f,"\n Integer operations per second:  %.5f \n", iops);
	fprintf(f,"\n Giga Integer operations per second:  %.5f \n", gigaIops);
}

void *iopOperations() {
	for(int i=0; i<noOfIterations; i++) {
		//printf("%d sd", i);
		intVal = intVal + 1
			+ 2 + 3
			+ 4 + 5
			+ 6 + 7
			+ 8 + 9
			+ 10 + 11
			+ 12+ 13
			+ 14 + 15
			+ 16 + 17
			+ 18 + 19 
			+ 20 + 21
			+ 22 + 23 ;
	}
}





