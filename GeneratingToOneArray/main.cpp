#include <iostream>
#include <pthread.h>

using namespace std;


void swap(double *xp, double *yp)
{
    double temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(double* arr, int n)
{
   int i, j;
   for (i = 0; i < n-1; i++)

       // Last i elements are already in place
       for (j = 0; j < n-i-1; j++)
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
}

double generateRandomDouble(double min, double max)
{
    return (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
}

typedef struct{
    int index;
    int length;
    double min;
    double max;
    double* arr;
    pthread_t t;
}Data;

void* threadFunction(void* data){
    Data* d = (Data*) data;
    for(int i = 0; i < d->length; i++){
        d->arr[i] = generateRandomDouble(d->min, d->max);
    }
}



int main()
{
    int N = 17;
    int M = 3;
    double min = -5.15;
    double max = 15.30;
    double* arr = new double[N];
    Data* data = new Data[N];

    for(int i = 0; i < M; i++){
        //data[i].index = i * (int)(N / M);
        int index = i * (int)(N / M);
        data[i].arr = &(arr[index]);
        data[i].min = min;
        data[i].max = max;
        data[i].length = (N / M);

        if(i == M - 1){
            data[i].length = (int)(N / M) + (N % M);
        }

        pthread_create(&data[i].t, nullptr, threadFunction, (void*) &data[i]);
    }

    for(int i = 0; i < M; i++){
        pthread_join(data[i].t, nullptr);
    }

    bubbleSort(arr, N);

    for(int i = 0; i < N; i++){
        printf("%f, ", arr[i]);
    }


    delete arr;
    delete data;

    return 0;
}
