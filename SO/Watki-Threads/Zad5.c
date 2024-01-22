#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

 void obsluga_sygnalu(){
        puts("\nOtrzymano sygnal\n");
}

void *fun(){

    struct sigaction akcja ={.sa_handler = obsluga_sygnalu};

    sigaction(SIGALRM, &akcja, NULL);
    printf("ID watku: %ld", pthread_self());


    pause();
    return NULL;
}




int main(void){

    pthread_t watek1;

    if(pthread_create(&watek1, NULL, fun, NULL) != 0){
        perror("pthread_create");
    }

    sleep(1);

    if(pthread_kill(watek1, SIGALRM) != 0){
        perror("pthread_kill");
    }

    if(pthread_join(watek1, NULL) != 0){
        perror("pthread_joi");
    }

    return 0;
}