#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


char* odczytaj_z_pliku(int plik){

    char *buffer = (char*) malloc(sizeof(char) *41);

    if(read(plik, buffer, 40) == -1)
        perror("read");
    else
        printf("\nOdczytano z pliku: %40s\n\n", buffer);

    return buffer;
}



void wpisz_znaki(int plik, char *buffer){

    if(write(plik, buffer, 40) == -1)
        perror("write");
    else
        printf("\nZapisano do pliku: %40s\n\n", buffer);
}



int main(void){

    int plikDoOdczytu = open("OdczytZad1.txt", O_RDONLY);

    if(plikDoOdczytu == -1)
        perror("open");


    // z poczatku
    char *odczytaneZnaki = odczytaj_z_pliku(plikDoOdczytu);


    int plikDoZapisu = open("ZapisZad1.txt", O_WRONLY);
    if(plikDoZapisu == -1)  
        perror("open");


    wpisz_znaki(plikDoZapisu , odczytaneZnaki);

    if(lseek(plikDoOdczytu, -40, SEEK_END) == -1)
        perror("lseek");


   odczytaneZnaki =  odczytaj_z_pliku(plikDoOdczytu);

    wpisz_znaki(plikDoZapisu, odczytaneZnaki);
    

    if(close(plikDoOdczytu) != 0)
        perror("close");
    return 0;
}



