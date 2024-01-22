#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define MAX_THREADS 5
#define BUFFER_SIZE 1024

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Struktura przechowująca dane dla wątku obsługującego klienta
typedef struct {
    int clientSocket;
} ThreadData;

// Funkcja obsługująca połączenie z klientem
void *handleClient(void *arg) {
    ThreadData *threadData = (ThreadData *)arg;
    int clientSocket = threadData->clientSocket;
    char buffer[BUFFER_SIZE];

    while (1) {
        ssize_t bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

        if (bytesReceived <= 0) {
            break;  // Błąd lub klient zamknął połączenie
        }

        // Odbiór komunikatu od klienta
        buffer[bytesReceived] = '\0';
        printf("Odebrano od klienta %d: %s", clientSocket, buffer);
    }

    close(clientSocket);
    free(threadData);
    pthread_exit(NULL);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);
    pthread_t threads[MAX_THREADS];
    int threadIndex = 0;

    // Tworzenie gniazda TCP
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Inicjalizacja struktury adresowej serwera
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Przypisanie adresu i portu do gniazda
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Nasłuchiwanie na gnieździe
    listen(serverSocket, MAX_THREADS);

    printf("Serwer nasłuchuje na porcie %d...\n", PORT);

    // Główna pętla serwera
    while (1) {
        // Akceptowanie połączenia od klienta
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);

        if (clientSocket < 0) {
            perror("Błąd w accept()");
            continue;
        }

        printf("Nowe połączenie od klienta %d.\n", clientSocket);

        // Tworzenie wątku do obsługi klienta
        ThreadData *threadData = (ThreadData *)malloc(sizeof(ThreadData));
        threadData->clientSocket = clientSocket;

        pthread_create(&threads[threadIndex], NULL, handleClient, (void *)threadData);

        // Detach wątku, aby zwolnić zasoby automatycznie po zakończeniu
        pthread_detach(threads[threadIndex]);

        // Zwiększanie indeksu wątku (cyklicznie)
        threadIndex = (threadIndex + 1) % MAX_THREADS;
    }

    // Zamknięcie gniazda serwera (to miejsce nigdy nie zostanie osiągnięte w nieskończonej pętli)
    close(serverSocket);

    return 0;
}
