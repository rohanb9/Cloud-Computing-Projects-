#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <immintrin.h>

/* Global variables */
const int operationsPerIteration = 32;

const int timeToRun = 10;
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
				f = fopen("CPUFLOPPerformace10MinAVX.txt", "w");
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
				f = fopen("CPUIOPPerformace10MinAVX.txt", "w");
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
	int i=0;
	for(i=0; i<threadCount; i++) {
		flopOpsArray[i] = 0;
	}
	
}

/* 
	Initialize all the values of iop operations
*/
void initIop() {
	iopOps = 0;
	int i=0;
	for(i=0; i<threadCount; i++) {
		iopOpsArray[i] = 0;
	}
	
}

/* 
	Kill all threads
*/
void killThreads() {
	int i=0; 
	for(i=0; i<threadCount; i++) {
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
	int i=0;
	for(i=0; i<threadCount; i++) {
		pthread_create(&threadArray[i],NULL,flopOperations, &i);
	}

	for(;;) {
		clock_gettime(CLOCK_MONOTONIC, &tend);
		timeBetweenTmp = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstartTmp.tv_sec + 1.0e-9*tstartTmp.tv_nsec);
		timeBetween = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);

		if(timeBetweenTmp > timeToRun + 1) {
			// 10 min over. End process.
			break;
		} else if(timeBetween >= 1.00) {
			clock_gettime(CLOCK_MONOTONIC, &tstart); 
			int i=0;
			for(i=0; i<threadCount; i++) {
				flopOps = flopOps + flopOpsArray[i];
			}
			fprintf(f,"\n %.ld", (flopOps));
			initFlop();
		}
	
	}

	/* Kill all threads. */
	killThreads();


}

void *flopOperations(void *threadId) {
	
	int thId = *(int*) threadId;
	__m256 vecfa = _mm256_set_ps(6.99999, 7.99999, 8.99999, 9.99999, 10.99999, 11.99999, 12.99999, 13.999999);
	__m256 vecfb = _mm256_set_ps(14.45685679, 15.54687899, 16.7687899, 17.768689, 18.6786789, 19.7869, 20.9, 21.99969);
	__m256 vecfc = _mm256_set_ps(134.456879, 14345.5463487899, 1346.763487899, 1347.76834689, 1834.6786789, 1349.7869, 3420.9, 23431.949969);
	__m256 vecfr = _mm256_set_ps(22.5675889, 23.658879, 24.9656454645, 25.546564569, 26.4564564569, 27.5665449, 28.54656459, 29.546456459);
	for(; ;) {
		//printf("%d sd", i);
		_mm256_add_ps(vecfa, vecfb);
		_mm256_add_ps(vecfc, vecfb);
		_mm256_add_ps(vecfr, vecfb);
		_mm256_add_ps(vecfr, vecfa);
		flopOpsArray[thId] = flopOpsArray[thId] + operationsPerIteration;
	}
	
}



/*
 	Benkchmarking using IOPS.
*/
void measurePerformanceUsingIOPS() {

	double timeBetween, timeBetweenTmp;
	struct timespec tstartTmp = {0,0}, tstart={0,0}, tend={0,0};
	initIop();

	clock_gettime(CLOCK_MONOTONIC, &tstartTmp);
	clock_gettime(CLOCK_MONOTONIC, &tstart);   
	
	int i=0;
	for(i=0; i<threadCount; i++) {
		pthread_create(&threadArray[i], NULL, iopOperations, &i);
	}

	for(;;) {
		clock_gettime(CLOCK_MONOTONIC, &tend);
		timeBetweenTmp = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstartTmp.tv_sec + 1.0e-9*tstartTmp.tv_nsec);
		timeBetween = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
		   ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);
		if(timeBetweenTmp > timeToRun + 1) {
			// 10 min over. End process.
			break;
		} else if(timeBetween >= 1.00) {
			clock_gettime(CLOCK_MONOTONIC, &tstart); 
			int i=0;
			for(i=0; i<threadCount; i++) {
				iopOps = iopOps + iopOpsArray[i];
			}
			fprintf(f,"\n %.ld", (iopOps));
			initIop();
		}
	
	}

	/* Kill all threads. */
	killThreads();

}

void *iopOperations(void *threadId) {
	
	int thId = *(int*) threadId;

	__m256i veca = _mm256_set_epi32(13236, 24234246, 3324426, 4324326, 5246, 64326, 72346, 823423326);
	__m256i vecb = _mm256_set_epi32(92341, 102342, 1133, 1342424, 13436, 142346, 123456, 132466);
	__m256i vecc = _mm256_set_epi32(492341, 4102342, 1324133, 13342424, 123436, 1242346, 4123456, 4132466);
	__m256i vecr = _mm256_set_epi32(173431, 14482, 13493, 2342204, 34216, 223426, 23436, 24436);
	for(; ;) {
		_mm256_add_epi32(veca, vecb);
		_mm256_add_epi32(vecb, vecc);
		_mm256_add_epi32(vecr, veca);
		_mm256_add_epi32(vecr, vecb);

		iopOpsArray[thId] = iopOpsArray[thId] + operationsPerIteration;
	}

}





