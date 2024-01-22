#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Użycie: %s <plik_tekstowy>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Błąd otwierania pliku");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Tworzenie gniazda TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Inicjalizacja struktury adresowej serwera
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Nawiązywanie połączenia z serwerem
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Błąd w connect()");
        exit(EXIT_FAILURE);
    }

    // Wysyłanie wierszy z pliku do serwera
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        send(sockfd, buffer, strlen(buffer), 0);
        sleep(1);  // Oczekiwanie przed wysłaniem kolejnego wiersza
    }

    // Zamknięcie gniazda klienta
    close(sockfd);
    fclose(file);

    return 0;
}
