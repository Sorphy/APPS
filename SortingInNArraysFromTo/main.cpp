#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include <pthread.h>

using namespace std;

int timeval_to_ms( timeval *before, timeval *after ){
    timeval res;
    timersub( after, before, &res );
    return 1000 * res.tv_sec + res.tv_usec / 1000;
}

typedef struct{
    int from, to;
    pthread_t thread;
    int* data;
}Arguments;

void swap(int *xp, int *yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int generateNumberInRange(double from, double to){
    double r1 = ((double)rand() / (double)RAND_MAX);
    double diff = to - from;
    double num = from + r1 * diff;

    return (int)num;
}

//Sorting algorithm
void bubbleSortAsc(int* arr, int to){
   int i, j;
   for (i = 0; i < to - 1; i++)

       // Last i elements are already in place
       for (j = 0; j < to-i-1; j++)
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
}

void bubbleSortDesc(int* arr, int to){
   int i, j;
   for (i = 0; i < to - 1; i++)

       // Last i elements are already in place
       for (j = 0; j < to-i-1; j++)
           if (arr[j] < arr[j+1])
              swap(&arr[j], &arr[j+1]);
}

void selectionSortAsc(int arr[], int n){
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;

        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

void selectionSortDesc(int arr[], int n){
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] > arr[min_idx])
            min_idx = j;

        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

void insertionSortAsc(int arr[], int n){
   int i, key, j;
   for (i = 1; i < n; i++)
   {
       key = arr[i];
       j = i-1;

       /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
       while (j >= 0 && arr[j] > key)
       {
           arr[j+1] = arr[j];
           j = j-1;
       }
       arr[j+1] = key;
   }
}

void insertionSortDesc(int arr[], int n){
   int i, key, j;
   for (i = 1; i < n; i++)
   {
       key = arr[i];
       j = i-1;

       /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
       while (j >= 0 && arr[j] < key)
       {
           arr[j+1] = arr[j];
           j = j-1;
       }
       arr[j+1] = key;
   }
}


//Sorting algorithm for threads
void* bubbleSortAscAsync(void* arguments){
    Arguments* args = (Arguments*)arguments;
    bubbleSortAsc(args->data, args->to - args->from);
}

void* bubbleSortDescAsync(void* arguments){
    Arguments* args = (Arguments*)arguments;
    bubbleSortDesc(args->data, args->to - args->from);
}

void* selectionSortAscAsync(void* arguments){
    Arguments* args = (Arguments*)arguments;
    selectionSortAsc(args->data, args->to - args->from);
}

void* selectionSortDescAsync(void* arguments){
    Arguments* args = (Arguments*)arguments;
    selectionSortDesc(args->data, args->to - args->from);
}

void* insertionSortAscAsync(void* arguments){
    Arguments* args = (Arguments*)arguments;
    insertionSortAsc(args->data, args->to - args->from);
}

void* insertionSortDescAsync(void* arguments){
    Arguments* args = (Arguments*)arguments;
    insertionSortDesc(args->data, args->to - args->from);
}


//Copy part of array to new array
void copyArray(int* arr, int* output, int from, int to){
    int tmp = 0;
    for(int i = from; i < to; i++){
         output[tmp] = arr[i];
         tmp++;
    }
}


//Merging algorithm
void mergeArraysAsc(int* arr1, int* arr2, int n1, int n2, int* arr3){
	int i = 0, j = 0, k = 0;

	while (i < n1 && j < n2){
		if(arr1[i] < arr2[j]){
			arr3[k++] = arr1[i++];
        }
		else{
			arr3[k++] = arr2[j++];
        }
	}

	while(i < n1){
		arr3[k++] = arr1[i++];
    }
	while(j < n2){
		arr3[k++] = arr2[j++];
    }
}

void mergeArraysDesc(int* arr1, int* arr2, int n1, int n2, int* arr3){
	int i = 0, j = 0, k = 0;

	while (i < n1 && j < n2){
		if(arr1[i] > arr2[j]){
			arr3[k++] = arr1[i++];
        }
		else{
			arr3[k++] = arr2[j++];
        }
	}

	while(i < n1){
		arr3[k++] = arr1[i++];
    }
	while(j < n2){
		arr3[k++] = arr2[j++];
    }
}


//Algorithm to verify if array is sorted
bool isSortedAsc(int* arr, int N){
    for(int i = 0; i < N - 1; i++){
        if(arr[i]> arr[i + 1]){ return false; }
    }
    return true;
}

bool isSortedDesc(int* arr, int N){
    for(int i = 0; i < N - 1; i++){
        if(arr[i] < arr[i + 1]){ return false; }
    }
    return true;
}


int main()
{
    timeval time_before, time_after;
    int N = 100000; //Elements count
    int M = 1; //Threads count
    double min = -10000;   //Minimum for generation
    double max = 150000;    //Maximum for generation
    int arr[N];
    Arguments arguments[M];

    //Generate random numbers ito array
    for(int i = 0; i < N; i++){
        arr[i] = generateNumberInRange(min,max);
    }


    //Create arguments
    for(int i = 0; i < M; i++){
        int from = i * (int)(N / M);
        int to = from + (N / M);
        if(i == M - 1){
            to = from + ((int)(N / M) + (N % M));
        }

        arguments[i].data = new int[to - from];
        arguments[i].from = from;
        arguments[i].to = to;
        copyArray(arr, arguments[i].data, from, to); //Copy values from original array
    }

    //Create threads and sort
    gettimeofday( &time_before, NULL );

    for(int i = 0; i < M; i++){
        pthread_create(&arguments[i].thread, NULL, insertionSortDescAsync, (void*) &arguments[i]);
    }
    for(int i = 0; i < M; i++){
        pthread_join(arguments[i].thread, NULL);
    }

    //Merge arrays
    int* output = nullptr;
	int size_o = 0;

	for (int i = 0; i < M; i++) {
		int s = 0;
		for (int y = 0; y <= i; y++) {
			s += arguments[i].to - arguments[i].from;
		}

		int* tmp = new int[s];

		mergeArraysDesc(output, arguments[i].data, size_o, arguments[i].to - arguments[i].from, tmp);
		output = tmp;
		size_o = s;
	}


    gettimeofday( &time_after, NULL );
    printf( "Time: %d [ms]\n", timeval_to_ms( &time_before, &time_after ) );


    //Verify if array is sorted
    if(isSortedDesc(output, N)){
        printf("Sorted\n\r");
    }else{
        printf("Unsorted\n\r");
    }
    return 0;
}
