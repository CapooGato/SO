#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>


void sprawdz_termina(int terminal_desc){


    // isatty sprawdza czy jest STDIN_FILENO
    if(isatty(terminal_desc) == 1){

        // ttyname zwraca nazwe terminala
        char *nazwa_terminala = ttyname(terminal_desc);

        if(nazwa_terminala == NULL)
            perror("ttyname");

        printf("\nNazwa terminala: %s\n",nazwa_terminala);


        // struct termio przechowuje ustawienia terminala
        struct termios ustawienia;

        // binduje terminal do tej struktury
        if(tcgetattr(terminal_desc, &ustawienia) == -1)
                perror("tcgetattr");

            
        printf("\nTlumaczenie NL na CR przy inpucie: %s\n", ustawienia.c_iflag&INLCR? "Wlaczaone":"Wylaczone");

    }
    else{
        puts("\nNie jest STDINN_FILENO");
    }

}



int main(void){
    sprawdz_termina(STDIN_FILENO);

    return 0;
}