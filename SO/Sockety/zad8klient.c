#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Tworzenie gniazda TCP
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Inicjalizacja struktury adresowej serwera
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Nawiązywanie połączenia z serwerem
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Połączono z serwerem.\n");

    // Wysyłanie pseudolosowych liczb do serwera
    while (1) {
        int randomNumber = rand() % 100;  // Generowanie liczby od 0 do 99
        sprintf(buffer, "Liczba: %d\n", randomNumber);

        send(clientSocket, buffer, strlen(buffer), 0);

        sleep(1);  // Oczekiwanie przed wysłaniem kolejnej liczby
    }

    // Zamknięcie gniazda klienta (to miejsce nigdy nie zostanie osiągnięte w nieskończonej pętli)
    close(clientSocket);

    return 0;
}
