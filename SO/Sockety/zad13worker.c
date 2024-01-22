#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int is_prime(int num) {
    if (num < 2) {
        return 0;
    }

    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main() {
    int sockfd;
    struct sockaddr_in farmerAddr, workerAddr;
    char buffer[BUFFER_SIZE];

    // Tworzenie gniazda UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Inicjalizacja struktury adresowej dla farmera
    farmerAddr.sin_family = AF_INET;
    farmerAddr.sin_port = htons(PORT);
    farmerAddr.sin_addr.s_addr = INADDR_ANY;

    // Przypisanie adresu i portu do gniazda
    bind(sockfd, (struct sockaddr*)&farmerAddr, sizeof(farmerAddr));

    // Inicjalizacja struktury adresowej dla workerów
    workerAddr.sin_family = AF_INET;

    socklen_t addr_size = sizeof(workerAddr);

    while (1) {
        // Odbieranie liczby od farmera
        ssize_t bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&workerAddr, &addr_size);

        if (bytesReceived <= 0) {
            perror("Błąd w odbieraniu danych");
            break;
        }

        // Konwertowanie otrzymanej liczby na liczbę całkowitą
        int num = atoi(buffer);

        // Sprawdzanie, czy liczba jest pierwsza
        if (is_prime(num)) {
            // Odsyłanie informacji o tym, że liczba jest pierwsza wraz z liczbą do farmara
            sprintf(buffer, "%d jest liczba pierwsza", num);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&workerAddr, sizeof(workerAddr));
        } else {
            // Odsyłanie informacji o tym, że liczba nie jest pierwsza wraz z liczbą do farmara
            sprintf(buffer, "%d nie jest liczba pierwsza", num);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&workerAddr, sizeof(workerAddr));
        }
    }

    // Zamykanie gniazda
    close(sockfd);

    return 0;
}
