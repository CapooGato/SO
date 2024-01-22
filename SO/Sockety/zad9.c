#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int main() {
    int serverSocket, clientSocket, maxSocket, activity, i;
    int clientSockets[MAX_CLIENTS];
    fd_set readfds;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Inicjalizacja tablicy gniazd klientów
    for (i = 0; i < MAX_CLIENTS; ++i) {
        clientSockets[i] = 0;
    }

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
        // Ustawienie zbioru deskryptorów
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);
        maxSocket = serverSocket;

        // Dodanie aktywnych gniazd klientów do zbioru
        for (i = 0; i < MAX_CLIENTS; ++i) {
            if (clientSockets[i] > 0) {
                FD_SET(clientSockets[i], &readfds);
                if (clientSockets[i] > maxSocket) {
                    maxSocket = clientSockets[i];
                }
            }
        }

        // Oczekiwanie na aktywność na którymkolwiek gnieździe
        activity = select(maxSocket + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("Błąd w select()");
            exit(EXIT_FAILURE);
        }

        // Nowe połączenie od klienta
        if (FD_ISSET(serverSocket, &readfds)) {
            clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
            printf("Nowe połączenie od klienta %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

            // Dodanie nowego gniazda klienta do tablicy
            for (i = 0; i < MAX_CLIENTS; ++i) {
                if (clientSockets[i] == 0) {
                    clientSockets[i] = clientSocket;
                    break;
                }
            }
        }

        // Obsługa danych od klientów
        for (i = 0; i < MAX_CLIENTS; ++i) {
            if (FD_ISSET(clientSockets[i], &readfds)) {
                ssize_t bytesReceived = recv(clientSockets[i], buffer, sizeof(buffer), 0);

                if (bytesReceived <= 0) {
                    // Błąd lub klient zamknął połączenie
                    printf("Połączenie z klientem %s:%d zakończone.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                    close(clientSockets[i]);
                    clientSockets[i] = 0;
                } else {
                    // Wyświetlenie odebranej liczby
                    buffer[bytesReceived] = '\0';
                    printf("Odebrano od klienta %s:%d: %s", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);
                }
            }
        }
    }

    // Zamknięcie gniazda serwera (to miejsce nigdy nie zostanie osiągnięte w nieskończonej pętli)
    close(serverSocket);

    return 0;
}
