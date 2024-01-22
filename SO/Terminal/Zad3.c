#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

void set_nie_kanoniczny(int terminal){

    struct termios ustawienia;

    if(tcgetattr(terminal, &ustawienia) == -1)
            perror("tcgetattr");

    ustawienia.c_lflag &= ~(ICANON | ECHO);

    if(tcsetattr(terminal,TCSANOW, &ustawienia) == -1)
            perror("tcsetattr");

    puts("\nZmieniono na niekanoniczny\n");
}

void set_kanoniczny(int terminal){

    struct termios ustawienia;

    if(tcgetattr(terminal, &ustawienia) == -1) 
            perror("tcgetattr");

    ustawienia.c_lflag |= ICANON | ECHO;

    if(tcsetattr(terminal,TCSANOW ,&ustawienia) == -1)
            perror("tcsetattr");

    puts("\nZmieniono na kanoniczny\n");
}


int main(void){


    char input;
    read(STDIN_FILENO, &input, 1);

    printf("Pobrany: %c\n", input);

    switch(input){
        case 'k':
            set_kanoniczny(STDIN_FILENO);
        break;

        case 'n':
            set_nie_kanoniczny(STDIN_FILENO);
        break;
    }

    return 0;
}

















