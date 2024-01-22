#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// zrob 20 watkow i yyyyy zwieksz liczbe do 20

pthread_mutex_t mutex;
int licznik =0;

void* f_zwieksz(){

    pthread_t threadID = pthread_self();

    for(int i = 0; i< 20; i++){
        pthread_mutex_lock(&mutex);
        licznik++;
        printf("\nWatek %lu zwiekszyl liczbe o 1\n", threadID);
        pthread_mutex_unlock(&mutex);
    }

    printf("\nLiczba: %d\n", licznik);
    pthread_exit(NULL);
}



int main(void){

    pthread_t watki[20];

    pthread_mutex_init(&mutex, NULL);

    for(int i =0; i< 20; i++){
        if(pthread_create(&watki[i], NULL, f_zwieksz, NULL) != 0)
            perror("pthread_create");
    }fi
    

    for(int i =0; i<20; i++){
    if(pthread_detach(watki[i]) != 0)
            perror("pthread_join");
    }
    
    pthread_mutex_destroy(&mutex);
    return 0;
}