#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Tworzenie gniazda UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Inicjalizacja struktury adresowej serwera
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Przypisanie adresu i portu do gniazda
    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    printf("Serwer UDP nasłuchuje na porcie %d...\n", PORT);

    // Odbieranie danych od klienta
    while (1) {
        ssize_t bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addr_size);

        if (bytesReceived <= 0) {
            perror("Błąd w odbieraniu danych");
            break;
        }

        // Wysyłanie potwierdzenia do klienta
        sendto(sockfd, "Potwierdzenie", sizeof("Potwierdzenie"), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));

        printf("Odebrano pakiet od klienta: %ld bajtów\n", bytesReceived);
    }

    close(sockfd);

    return 0;
}
