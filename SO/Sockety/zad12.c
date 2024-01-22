#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Tworzenie gniazda TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Inicjalizacja struktury adresowej serwera
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Przypisanie adresu i portu do gniazda
    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Nasłuchiwanie na gnieździe
    listen(sockfd, 1);  // Parametr 1 oznacza maksymalną liczbę oczekujących połączeń

    printf("Serwer TCP nasłuchuje na porcie %d...\n", PORT);

    // Akceptowanie połączenia od klienta
    clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);

    if (clientSocket < 0) {
        perror("Błąd w accept()");
        exit(EXIT_FAILURE);
    }

    printf("Nawiązano połączenie z klientem.\n");

    // Odbieranie danych od klienta
    while (1) {
        ssize_t bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

        if (bytesReceived <= 0) {
            perror("Błąd w odbieraniu danych");
            break;
        }

        // Wysyłanie potwierdzenia do klienta
        send(clientSocket, "Potwierdzenie", sizeof("Potwierdzenie"), 0);

        printf("Odebrano pakiet od klienta: %ld bajtów\n", bytesReceived);
    }

    close(clientSocket);
    close(sockfd);

    return 0;
}
