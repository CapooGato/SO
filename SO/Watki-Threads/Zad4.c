#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void*f_laczony(){

    puts("\nRozpoczeto watek laczony.\n");

    sleep(3);

    puts("\nZakonczono watek laczony.\n");

    pthread_exit(NULL);
}


void*f_rodzielony(){

    puts("\nRozpoczeto watek rodzielny.\n");

    sleep(3);

    puts("\nZakonczono watek rozdzielny.\n");
    
    pthread_exit(NULL);
}


int main(void){

    pthread_t w_laczny, w_rodzielony;

    if(pthread_create(&w_laczny, NULL, f_laczony, NULL) != 0){
        perror("pthread_create");
    }

    if(pthread_create(&w_rodzielony, NULL, f_rodzielony, NULL) != 0){
        perror("pthread_create");
    }

    // watek laczny
    if(pthread_join(w_laczny, NULL) != 0){
        perror("pthread_join");
    }

    // watek rodzielony
    if(pthread_detach(w_rodzielony) != 0){
        perror("pthread_detach");
    }

    return 0;
}
