#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>


void manage_echo(int deskryptor_terminala){

    struct termios terminal;

    if(tcgetattr(deskryptor_terminala, &terminal) == -1)
            perror("tcgetattr");

    int czy_echo = terminal.c_iflag & ECHO;

    terminal.c_iflag ^= ECHO;

    if(tcsetattr(deskryptor_terminala, TCSANOW, &terminal) == -1)
            perror("tcsetattr");

    printf("\nEcho: %s", czy_echo ? "Wlaczone\n" : "Wylaczone\n");
}


int main(void){

    manage_echo(STDIN_FILENO);

    return 0;
}