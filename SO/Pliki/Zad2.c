#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>



int main(void){

    int plikZDziura = open("DziuraZad2.txt", O_WRONLY | O_CREAT, 0600);
    if(plikZDziura == -1)
        perror("open");


    // Zrobic plik o rozmiarze x i przesunac wskaznik na koniec 
    int rozmiar_pliku = 512;

    if(lseek(plikZDziura, rozmiar_pliku -1, SEEK_SET) == -1)
        perror("lseek");

    
    if(write(plikZDziura, "s", 2) == -1)
        perror("write");


    if(close(plikZDziura) != 0)
        perror("close");
  
    return 0;
}
