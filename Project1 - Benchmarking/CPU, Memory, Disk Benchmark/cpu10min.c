#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Global variables */
const int operationsPerIteration = 25;
const int threadCount = 8;
pthread_t threadArray[8];
FILE *f;

long flopOpsArray[8];
long flopOps;
long double doubleVal;

long iopOpsArray[8];
long iopOps;
long intVal;


/* Function Declaration*/
void initFlop();
void initIop();
void killThreads();
void measurePerformanceUsingFLOPS();
void measurePerformanceUsingIOPS();
void *flopOperations(void *threadId);
void *iopOperations(void *threadId);


int main()
{
	char option;
 
	printf("Please select the operation type:");
	printf("\n a) FLOP");
	printf("\n b) IOP");
	printf("\n USER INPUT (a or b) : ");

	scanf("%c", &option);
	//printf("\n \n ERROR: Inwddddddddddddddddvalid input. %c", option);

	if (option != 'a' && option != 'b') {
		printf("\n \n ERROR: Invalid input.");
	} else {
		printf("\n\nBanchmarking process started using 8 threads. (It will take 10 minutes) ... ");
			
		switch (option) {
			case 'a':
				f = fopen("CPUFLOPPerformace10Min.txt", "w");
				if (f == NULL)
				{
				    printf("\n\nError opening file!\n");
					    
				} else {
				fprintf(f,"\n\nBanchmarking process started using 8 threads. (It will take 10 minutes) ... ");

				fprintf(f,"\n\n******************************************************************************************************************************************************* ");
				printf("\n\n ******************************************************************************************************************************************************* ");
		
					printf("\n\nAnalysise using double precision FLOPS ... ");
					fprintf(f,"\n\nAnalysise using double precision FLOPS ... ");
					measurePerformanceUsingFLOPS();
					printf("\nBanchmarking using FLOPS results are ready. Please check performance report file.");
					fprintf(f,"\nBanchmarking using FLOPS process completed.");
				printf("\n\n ******************************************************************************************************************************************************* ");
				fprintf(f,"\n\n ******************************************************************************************************************************************************* ");
				fclose(f);
				}
				break;

			case 'b':
				f = fopen("CPUIOPPerformace10Min.txt", "w");
				if (f == NULL)
				{
				    printf("\n\nError opening file!\n");
					    
				} else {
				fprintf(f,"\n\nBanchmarking process started using 8 threads. (It will take 10 minutes) ... ");

				fprintf(f,"\n\n******************************************************************************************************************************************************* ");
				printf("\n\n ******************************************************************************************************************************************************* ");
		
					printf("\n\nAnalysise using IOPS ... ");
					fprintf(f,"\n\nAnalysise using IOPS ... ");
					measurePerformanceUsingIOPS();
					printf("\nBanchmarking using IOPS results are ready. Please check performance report file.");
					fprintf(f,"\nBanchmarking using IOPS process completed.");
				printf("\n\n ******************************************************************************************************************************************************* ");
				fprintf(f,"\n\n ******************************************************************************************************************************************************* ");
				fclose(f);
				}
				break;
		}
		
	
	}
	
	
	printf("\n\n");

	return 0;
}

/* 
	Initialize all the values of flop operations
*/
void initFlop() {
	flopOps = 0;
	for(int i=0; i<threadCount; i++) {
		flopOpsArray[i] = 0;
	}
	
}

/* 
	Initialize all the values of iop operations
*/
void initIop() {
	iopOps = 0;
	for(int i=0; i<threadCount; i++) {
		iopOpsArray[i] = 0;
	}
	
}

/* 
	Kill all threads
*/
void killThreads() {
	for(int i=0; i<threadCount; i++) {
		pthread_cancel(threadArray[i]);
	}
	
}

/*
 	Benkchmarking using FLOPS.
*/
void measurePerformanceUsingFLOPS() {
	double timeBetween, timeBetweenTmp;
	struct timespec tstartTmp = {0,0}, tstart={0,0}, tend={0,0};
	initFlop();
	clock_gettime(CLOCK_MONOTONIC, &tstartTmp);
	clock_gettime(CLOCK_MONOTONIC, &tstart);   
	
	for(int i=0; i<threadCount; i++) {
		pthread_create(&threadArray[i],NULL,flopOperations, &i);
	}

	for(;;) {
		clock_gettime(CLOCK_MONOTONIC, &tend);
		timeBetweenTmp = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstartTmp.tv_sec + 1.0e-9*tstartTmp.tv_nsec);
		timeBetween = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
		if(timeBetweenTmp > 600) {
			// 10 min over. End process.
			break;
		} else if(timeBetween >= 1.00) {
			clock_gettime(CLOCK_MONOTONIC, &tstart); 
			for(int i=0; i<threadCount; i++) {
				flopOps = flopOps + flopOpsArray[i];
			}
			fprintf(f,"\n %.ld \n", flopOps);
			initFlop();
		}
	
	}

	/* Kill all threads. */
	killThreads();


}

void *flopOperations(void *threadId) {
	
	int thId = *(int*) threadId;

	for(; ;) {
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
		flopOpsArray[thId] = flopOpsArray[thId] + 25;
	}
}



/*
 	Benkchmarking using IOPS.
*/
void measurePerformanceUsingIOPS() {

	double timeBetween, timeBetweenTmp;
	struct timespec tstartTmp = {0,0}, tstart={0,0}, tend={0,0};
	initIop();

	clock_gettime(CLOCK_MONOTONIC, &tstart);   
	
	for(int i=0; i<threadCount; i++) {
		pthread_create(&threadArray[i], NULL, iopOperations, &i);
	}

	for(;;) {
		clock_gettime(CLOCK_MONOTONIC, &tend);
		timeBetweenTmp = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstartTmp.tv_sec + 1.0e-9*tstartTmp.tv_nsec);
		timeBetween = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
		if(timeBetweenTmp > 600) {
			// 10 min over. End process.
			break;
		} else if(timeBetween >= 1.00) {
			clock_gettime(CLOCK_MONOTONIC, &tstart); 
			for(int i=0; i<threadCount; i++) {
				iopOps = iopOps + iopOpsArray[i];
			}
			fprintf(f,"\n %.ld \n", iopOps);
			initIop();
		}
	
	}

	/* Kill all threads. */
	killThreads();

}

void *iopOperations(void *threadId) {
	int thId = *(int*) threadId;
	for(; ;) {
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
			+ 22 + 23;
		iopOpsArray[thId] = iopOpsArray[thId] + 25;
	}
}





