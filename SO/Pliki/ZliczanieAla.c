#include<stdio.h>
#include<string.h>

int main(void){

    FILE *plik = fopen("kolokwium.txt", "r");

    if(plik == NULL){
        puts("Blad otwarcia pliku\n");
        perror("fopen");
        return -1;
    }

    int licznik_ala =0, licznik_przecinek =0;

    char linia[100];
    while(fgets(linia, sizeof(linia), plik) != NULL){
       
       char *prt = linia;

       while((prt = strstr(prt, "ala")) != NULL){
        licznik_ala++;
        prt +=3;
       }

       prt = linia;
       while((prt = strchr(prt, ',')) != NULL){
        licznik_przecinek++;
        prt++;
       }
    }

    if(fclose(plik) < 0){
        puts("\nBlad zamykania pliku");
        perror("fclose");
        return -1;
    }
    printf("\nAla: %d", licznik_ala);
    printf("\nPrzecinek: %d", licznik_przecinek);

    return 0;
}