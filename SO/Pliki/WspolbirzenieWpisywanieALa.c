#include<stdio.h>
#include<stdlib.h>
#include<string.h>



int main(void){

    FILE *plik = fopen("plik_do_zapisu.txt", "w");

    if(plik == NULL){
        puts("\nBlad otwarcia pliku");
        perror("fopen");
        return -1;
    }

    puts("\nPodaj tekst do zapisu:\n");

    char tekst[1024];

    while(fgets(tekst, sizeof(tekst), stdin) != NULL){
        fprintf(plik, "%s", tekst);
    }

    if(fclose(plik) < 0){
        puts("\nBlad zamykania pliku");
        perror("fclose");
        return -1;  
    }

    return 0;
}