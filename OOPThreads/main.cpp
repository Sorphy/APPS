#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include <pthread.h>

using namespace std;

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

void bubbleSortAsc(int* arr, int to){
    int i, j;
    for (i = 0; i < to - 1; i++)

           // Last i elements are already in place
        for (j = 0; j < to-i-1; j++)
            if (arr[j] > arr[j+1])
                swap(&arr[j], &arr[j+1]);
}



void* bubbleSortAscAsync(void* arguments){
    Arguments* args = (Arguments*)arguments;
    bubbleSortAsc(args->data, args->to - args->from);
}

int generateNumberInRange(double from, double to){
    double r1 = ((double)rand() / (double)RAND_MAX);
    double diff = to - from;
    double num = from + r1 * diff;

    return (int)num;
}

int timeval_to_ms( timeval *before, timeval *after ){
    timeval res;
    timersub( after, before, &res );
    return 1000 * res.tv_sec + res.tv_usec / 1000;
}


class Sort{
public:
    Arguments* arguments;
    int* arr;
    int N;
    int M;

    Sort(int* arr, int N, int M){
            this->arr = arr;
            this->N = N;
            this->M = M;
            this->arguments = new Arguments[M];
            this->makeArguments();
    }

    void makeArguments(){
        for(int i = 0; i < this->M; i++){

            int from = i * (int)(this->N / this->M);
            int to = from + (this->N / this->M);
            if(i == this->M - 1){
                to = from + ((int)(this->N / this->M) + (this->N % this->M));
            }

            this->arguments[i].data = new int[to - from];
            this->arguments[i].from = from;
            this->arguments[i].to = to;
            this->copyArray(this->arr, this->arguments[i].data, from, to); //Copy values from original array
        }
    }

    void copyArray(int* arr, int* output, int from, int to){
        int tmp = 0;
        for(int i = from; i < to; i++){
             output[tmp] = arr[i];
             tmp++;
        }
    }

    void createThreads(void* threadFunction(void*)){
        for(int i = 0; i < this->M; i++){
            pthread_create(&this->arguments[i].thread, NULL, threadFunction, (void*) &this->arguments[i]);
        }
    }

    void joinThreads(){
        for(int i = 0; i < this->M; i++){
            pthread_join(this->arguments[i].thread, NULL);
        }
    }

    void mergeArrays(int* arr1, int* arr2, int n1, int n2, int* arr3){
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

    void mergeMultiplyArrays(int*& output){
        int size_o = 0;

        for (int i = 0; i < M; i++) {
            int s = 0;
            for (int y = 0; y <= i; y++) {
                s += (this->arguments[y].to - this->arguments[y].from);
            }

            int* tmp = new int[s];

            this->mergeArrays(output, this->arguments[i].data, size_o, this->arguments[i].to - this->arguments[i].from, tmp);
            output = tmp;
            size_o = s;
        }
    }

    ~Sort(){
        for(int i = 0; i < this->M; i++){
            delete this->arguments[i].data;
        }
        delete this->arguments;
    }
};

bool isSortedAsc(int* arr, int N){
    for(int i = 0; i < N - 1; i++){
        if(arr[i]> arr[i + 1]){ return false; }
    }
    return true;
}

int main()
{
    timeval time_before, time_after;
    int N = 100000;     //Elements count
    int M = 460;      //Thread count
    double min = 97;   //Minimum for generation
    double max = 122;    //Maximum for generation
    int arr[N];     //Original array
    int* output = nullptr;  //Sorted array

    //**************************Generate array****************************

    for(int i = 0; i < N; i++){
        arr[i] = generateNumberInRange(min,max);
    }

    //*******************************************************************

    //**********************Sorting***********************************

    Sort* s = new Sort(arr, N, M);

    gettimeofday( &time_before, NULL );


    s->createThreads(bubbleSortAscAsync);
    s->joinThreads();
    s->mergeMultiplyArrays(output);


    gettimeofday( &time_after, NULL );
    printf( "Time: %d [ms]\n", timeval_to_ms( &time_before, &time_after ) );

    //*****************************************************************

    if(isSortedAsc(output, N)){
        printf("Sorted\n\r");
    }else{
        printf("Unsorted\n\r");
    }

    delete s;

    return 0;
}
