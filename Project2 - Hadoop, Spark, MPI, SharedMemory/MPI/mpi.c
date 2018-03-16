/*MPI FlowMPI_Init: Initialize MPI execution environmentMPI_Comm_rank: Determine the rank of calling process in the communicatorMPI_Comm_size: Determine the size of group associated with communicatorMPI_Scatter: send the data from one process to other process in a communicatorMPI_Gather: Gather together all values from a group of processMPI_Barrier: Blocks until all processes in the communicator have reached this routine.MPI_Finalize: Terminates MPI execution environment*///Library initialization#include <stdio.h>#include <stdlib.h>#include <time.h>#include <mpi.h>//function declarationvoid merge(int *, int *, int, int, int);void mergeSort(int *, int *, int, int);int main(int argc, char** argv) {    /********** Create and populate the array **********/  FILE *fp;  char ch;  int n=0;  clock_t start, end;  double cpu_time_used;    //Read text file which is generated by gensort  fp = fopen("datafile.txt", "r");  //read each character from file  while((ch = fgetc(fp)) != EOF) {    {	  //Count number of character      n=n+1;    }  }  //close open file  fclose(fp);  //create array of n size  int *buf = malloc(n * sizeof(int));    int c;  srand(time(NULL));    for(c = 0; c < n; c++) {   //store number in array for sorting    buf[c] = rand() % 9;     }  int rank;  int char_size;    //Initialize MPI execution environment  MPI_Init(&argc,   &argv); //Determine the rank of calling process in the communicator  MPI_Comm_rank(MPI_COMM_WORLD,   &rank);  //Determine the size of group associated with communicator  MPI_Comm_size(MPI_COMM_WORLD,   &char_size);  //Create chunk  int size = n/char_size;  //Create chunk of array and memmory allocated  int *chunk_arrray = malloc(size * sizeof(int));  //send the data from one process to other process in a communicator  MPI_Scatter(buf, size, MPI_INT, chunk_arrray, size, MPI_INT, 0, MPI_COMM_WORLD);   //Create temporay array  int *temp = malloc(size * sizeof(int));  //start clock for finding starting time  start = clock(); //start merge sort mergeSort(chunk_arrray, temp, 0, (size - 1));    /********** Gather the sorted subarrays into one **********/  int *sorted = NULL;  //check for rank number  if(rank == 0) {        sorted = malloc(n * sizeof(int));      }  //Gather together all values from a group of process  MPI_Gather(chunk_arrray, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);    /********** Make the final mergeSort call **********/  if(rank == 0) {    //create temp array    int *temp_arr = malloc(n * sizeof(int));    mergeSort(sorted, temp_arr, 0, (n - 1));   //stop clock after program execution    end = clock();	//find difference between start and end time    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;	//print time taken for execution    printf("%f\n",cpu_time_used);   //free allocated memory    free(sorted);    free(temp_arr);      }    //clean allocated memory  free(buf);  free(chunk_arrray);  free(temp);    //Blocks until all processes in the communicator have reached this routine.  MPI_Barrier(MPI_COMM_WORLD);  //Terminates MPI execution environment  MPI_Finalize();  }//Function for merge sortvoid merge(int *a, int *b, int l, int m, int r) {  //Variable declaration  int h, i, j, k;  h = l;  i = l;  j = m + 1; //Condition for switching  while((h <= m) && (j <= r)) {   //check one by one position in different array  //increase position by 1 if condition true    if(a[h] <= a[j]) {         b[i] = a[h];	  //Increase first array position by 1      h++;       }     else {         b[i] = a[j];	  //increase 2nd array position by 1      j++;      }	//increase position of start postion of array by 1    i++;   }   if(m < h) {       for(k = j; k <= r; k++) {  //Copy array 	      b[i] = a[k];	  //Increase position by 1      i++;      }   }  else {       for(k = h; k <= m; k++) {   //Copy array	      b[i] = a[k];	  //Increase position by 1      i++;       }   }  for(k = l; k <= r; k++) { //copy array      a[k] = b[k];    }}//Function for recursive merg sortvoid mergeSort(int *a, int *b, int l, int r) {  int m;  //check for array contains more than 1 values  if(l < r) {       m = (l + r)/2;    //recursively calling 1st half of array    mergeSort(a, b, l, m);	//recursively calling 2nd half of array    mergeSort(a, b, (m + 1), r);	//merge two array    merge(a, b, l, m, r);    }  }