#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in workerAddr;
    char buffer[BUFFER_SIZE];

    // Tworzenie gniazda UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Inicjalizacja struktury adresowej dla workerów
    workerAddr.sin_family = AF_INET;
    workerAddr.sin_port = htons(PORT);
    workerAddr.sin_addr.s_addr = INADDR_ANY;

    // Rozsyłanie liczb do trzech workerów
    for (int num = 2; num <= 302; ++num) {
        // Wypełnianie bufora liczbą
        sprintf(buffer, "%d", num);

        // Wysyłanie liczby do workerów
        for (int workerId = 1; workerId <= 3; ++workerId) {
            workerAddr.sin_port = htons(PORT + workerId);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&workerAddr, sizeof(workerAddr));
        }

        // Oczekiwanie na informacje od workerów
        for (int workerId = 1; workerId <= 3; ++workerId) {
            recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);

            // Wyświetlanie informacji od workerów
            printf("Farmer: %s\n", buffer);
        }
    }

    // Zamykanie gniazda
    close(sockfd);

    return 0;
}
