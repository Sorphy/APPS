#include <iostream>
#include <pthread.h>

using namespace std;

int generateRandomNumber(int min, int max){
    return min + ( std::rand() % ( max - min + 1 ) );
}

int* generateRandomArray(int min, int max, int count){
    int* output = new int[count];

    for(int i = 0; i < count; i++){
            output[i] = generateRandomNumber(min, max);
    }

    return output;
}

class Data{
public:
    int min;
    int max;
    int count;
    int* output;
};

void* threadFunction(void* data){
    Data* d = (Data*) data;
    d->output = generateRandomArray(d->min, d->max, d->count);
}

int main()
{
    int N = 17;  //Number of elements
    int M = 3; //Number of threads

    int min_s = -200;
    int max_s = 150;

    Data** data = new Data*[M];
    pthread_t* threads = new pthread_t[N];
    for(int i = 0; i < M; i++){
        if(i < M - 1){
            data[i] = new Data();
            data[i]->min = min_s;
            data[i]->max = max_s;
            data[i]->count = (int) (N / M);
        }
        else{
            data[i] = new Data();
            data[i]->min = min_s;
            data[i]->max = max_s;
            data[i]->count = (int) ((N / M) + (N % M));
        }

        pthread_t t;
        threads[i] = t;

        pthread_create(&threads[i], nullptr, threadFunction, (void*) data[i]);
    }



    for(int i = 0; i < N; i++){
        pthread_join(threads[i], nullptr);
    }

    for(int i = 0; i < N; i++){
        for(int y = 0; y < data[i]->count; y++){
            printf("%d, ", data[i]->output[y]);
        }
        printf("\r\n");
    }


    //Deleteing
    for(int i = 0; i < M; i++){
        delete data[i];
    }
    delete data;
    delete threads;

    return 0;
}
