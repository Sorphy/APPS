#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include <pthread.h>

using namespace std;

int timeval_to_ms( timeval *before, timeval *after )
{
    timeval res;
    timersub( after, before, &res );
    return 1000 * res.tv_sec + res.tv_usec / 1000;
}

int generateRandomNumber(int min, int max){
    return min + ( rand() % ( max - min + 1 ) );
}

typedef struct{
    int min;
    int max;
    int count;
    int* output;
    pthread_t t;
} Data;

void* threadFunction(void* data){
    srand( ( int ) time( NULL ) );

    Data* d = (Data*) data;

    int* tmp = new int[d->count];
    for(int i = 0; i < d->count; i++){
         tmp[i] = generateRandomNumber(d->min, d->max);
    }
    d->output = tmp;
    delete tmp;
    return NULL;
}

int main()
{
    int N = 17000;  //Number of elements
    int M = 3; //Number of threads

    int min_s = -200;
    int max_s = 150;

    Data data[M];

    timeval time_before, time_after;

     //*****************************Multithread************************************
    gettimeofday( &time_before, NULL );

    for(int i = 0; i < M; i++){
        data[i].min = min_s;
        data[i].max = max_s;
        data[i].count = (int) (N / M);

        if(i == M - 1){
            data[i].count = ((N / M) + (N % M));
        }

        pthread_create(&data[i].t, nullptr, threadFunction, (void*) &data[i]);
    }

    for(int i = 0; i < M; i++){
        pthread_join(data[i].t, nullptr);
    }

    gettimeofday( &time_after, NULL );

    printf( "The generating time (multi): %d [ms]\n", timeval_to_ms( &time_before, &time_after ) );

    //**************************************************************************************

    //******************************Singlethread*******************************************

    gettimeofday( &time_before, NULL );

    int arr2[N];
    for(int i = 0; i < N; i++){
        arr2[i] = generateRandomNumber(min_s, max_s);
    }

    gettimeofday( &time_after, NULL );

    printf( "The generating time (single): %d [ms]\n", timeval_to_ms( &time_before, &time_after ) );

    //*********************************************************************************

   /* for(int i = 0; i < M; i++){
        for(int y = 0; y < data[i].count; y++){
            printf("%d, ", data[i].output[y]);
        }
        printf("\r\n");
    }*/


    return 0;
}
