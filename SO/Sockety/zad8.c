#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// Struktura przechowująca dane dla wątku obsługującego klienta
typedef struct {
    int clientSocket;
    struct sockaddr_in clientAddr;
} ClientData;

// Funkcja obsługująca połączenie z klientem
void *handleClient(void *arg) {
    ClientData *clientData = (ClientData *)arg;
    int clientSocket = clientData->clientSocket;
    struct sockaddr_in clientAddr = clientData->clientAddr;
    char buffer[BUFFER_SIZE];

    printf("Nowe połączenie od klienta %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    // Obsługa komunikacji z klientem
    while (1) {
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            // Błąd lub klient zamknął połączenie
            printf("Połączenie z klientem %s:%d zakończone.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            break;
        }

        // Wyświetlenie odebranej liczby
        buffer[bytesReceived] = '\0';
        printf("Odebrano od klienta %s:%d: %s", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);
    }

    // Zamknięcie gniazda klienta i zwolnienie zasobów
    close(clientSocket);
    free(clientData);
    pthread_exit(NULL);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);

    // Tworzenie gniazda TCP
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Inicjalizacja struktury adresowej serwera
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Przypisanie adresu i portu do gniazda
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Nasłuchiwanie na gnieździe
    listen(serverSocket, MAX_CLIENTS);

    printf("Serwer nasłuchuje na porcie %d...\n", PORT);

    // Główna pętla serwera
    while (1) {
        // Akceptowanie połączenia od klienta
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);

        // Tworzenie struktury danych dla nowego klienta
        ClientData *clientData = (ClientData *)malloc(sizeof(ClientData));
        clientData->clientSocket = clientSocket;
        clientData->clientAddr = clientAddr;

        // Tworzenie wątku do obsługi klienta
        pthread_t clientThread;
        pthread_create(&clientThread, NULL, handleClient, (void *)clientData);

        // Detach wątku, aby zwolnić zasoby automatycznie po zakończeniu
        pthread_detach(clientThread);
    }

    // Zamknięcie gniazda serwera (to miejsce nigdy nie zostanie osiągnięte w nieskończonej pętli)
    close(serverSocket);

    return 0;
}

