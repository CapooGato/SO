#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct liczby{
        int a;
        int b;
};

void *fun1(void * dane){

    struct liczby* licz = (struct liczby*)dane;

    int suma = licz->a + licz->b;

    printf("\nSuma: %d\n", suma);

    pthread_exit(NULL);
}


void *fun2(void *dane){
    
    struct liczby *licz = (struct liczby*)dane;

    int roznica = licz->a -licz->b;

    printf("\nRoznica: %d\n", roznica);
    
    pthread_exit(NULL);
}

int main(void){

    pthread_t watek1, watek2;


    struct liczby liczby1 = {10,5};
    struct liczby liczby2 = {8,3};



    if(pthread_create(&watek1, NULL, fun1, (void*)&liczby1) != 0){
        perror("pthread_create");
    }

    if(pthread_create(&watek2, NULL, fun2, (void*)&liczby2) != 0){
        perror("pthread_create");
    }

    if(pthread_join(watek1, NULL)){
        perror("pthread_join");
    }

    if(pthread_join(watek2, NULL)){
        perror("pthread_join");
    }
    return 0;
}


