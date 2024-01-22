#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>





int main(void){

    int plik = open("DuplikatZad3.txt",  O_RDWR | O_CREAT, 0600);
    if(plik == -1)
        perror("open");


   const char *text = "Jakis przykladowy text";


    if(write(plik, text, strlen(text)) == -1)   
        perror("write");
    

    if(lseek(plik, 0, SEEK_SET) == -1)
        perror("lseek");

    char odczytajZPliku[64];
    if(read(plik, odczytajZPliku, strlen(text)) == -1)
        perror("read");    

    printf("\nOdczytano z pliku pierwotnego: %s\n\n", odczytajZPliku);


    int duplikatPliku = dup(plik);
    if(duplikatPliku == -1)
        perror("dup");


     if(lseek(duplikatPliku, 0, SEEK_SET) == -1)
        perror("lseek");    

    char odczytajZKopii[64];
    if(read(plik, odczytajZKopii, 64) == -1)
        perror("read");  

    printf("\nOdczytano z kopii pliku: %s\n\n", odczytajZKopii);

    return 0;
}