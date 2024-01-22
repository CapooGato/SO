#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

void *func(){

    // Pobierz ID watku
    pthread_t thread_id = pthread_self();


    // Pobierz ID procesu
    pid_t pid = getpid();

    printf("\nId watku: %ld, ID procesu: %d\n", thread_id, pid);

    pthread_exit(NULL);
}



int main(void){

    pthread_t watek1, watek2;

    if(pthread_create(&watek1, NULL, func, NULL) != 0){
        perror("pthread_create");
    }
    if(pthread_create(&watek2, NULL, func, NULL) != 0){
        perror("pthread_create");
    }


    if(pthread_join(watek1, NULL) != 0){
        perror("pthread_join");
    }
    
    if(pthread_join(watek2, NULL) != 0){
        perror("pthread_join");
    }

    return 0;
}