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


class Arguments{
public:
    int from;
    int to;
    pthread_t thread;
    int* data;
};

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

void bubbleSortAsc(int* arr, int to){
    int i, j;
    for (i = 0; i < to - 1; i++)
        for (j = 0; j < to-i-1; j++)
            if (arr[j] > arr[j+1])
                swap(&arr[j], &arr[j+1]);
}


//Sorting algorithm for threads
void* bubbleSortAscAsync(void* arguments){
    Arguments* args = (Arguments*)arguments;
    bubbleSortAsc(args->data, args->to - args->from);
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


//Algorithm to verify if array is sorted
bool isSortedAsc(int* arr, int N){
    for(int i = 0; i < N - 1; i++){
        if(arr[i]> arr[i + 1]){ return false; }
    }
    return true;
}


int main()
{
    timeval time_before, time_after;
    int N = 100000; //Elements count
    int M = 150; //Threads count
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
        pthread_create(&arguments[i].thread, NULL, bubbleSortAscAsync, (void*) &arguments[i]);
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

		mergeArraysAsc(output, arguments[i].data, size_o, arguments[i].to - arguments[i].from, tmp);
		output = tmp;
		size_o = s;
	}


    gettimeofday( &time_after, NULL );
    printf( "Time: %d [ms]\n", timeval_to_ms( &time_before, &time_after ) );


    //Verify if array is sorted
    if(isSortedAsc(output, N)){
        printf("Sorted\n\r");
    }else{
        printf("Unsorted\n\r");
    }
    return 0;
}
