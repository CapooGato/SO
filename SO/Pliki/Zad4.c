#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/file.h>


void obsluz_potomka(int plik, int procesID, int lockType){

    printf("\nObsluguje proces o ID: %d\n\n", getpid());  

    if(flock(plik, lockType) == -1){
        perror("flock");
        exit(EXIT_FAILURE);
    }

    if(lockType == LOCK_EX){
        printf("\nProces o ID: %d, ma plik na wylacznosc\n\n", getpid());
        sleep(2);
    }else{
        printf("\nProces o ID: %d, dzieli sie plikiem\n\n", getpid());
        sleep(2);
    }


    if(flock(plik, LOCK_UN) == -1){
        perror("flock");
        exit(EXIT_FAILURE);
    }  

    printf("\nProces o ID: %d, zwolil plik\n", getpid());
    exit(EXIT_SUCCESS);
}

int main(void){

    int plik = open("PlikZad4.txt",O_RDWR | O_CREAT, 0600);
    if(plik == -1){
        perror("open");
        exit(EXIT_FAILURE);        
    }


    for(int i =0; i < 3; i++){

        int procesID = fork();

        if(procesID == -1){
            perror("fork");
            exit(EXIT_FAILURE);

        }else if(procesID == 0){ // Potomek

            if(i %2 == 0)
                obsluz_potomka(plik, procesID, LOCK_EX);
            else
                obsluz_potomka(plik, procesID, LOCK_SH);
        }
    }
    
    for(int i =0; i< 3; i++){
        wait(NULL);
    }
   

    if(close(plik) != 0)
        perror("close");

    
    return 0;
}