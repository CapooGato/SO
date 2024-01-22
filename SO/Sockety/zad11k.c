#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Użycie: %s <rozmiar_pakietu>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    size_t packetSize = atoi(argv[1]);

    // Tworzenie gniazda UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Inicjalizacja struktury adresowej serwera
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Wysyłanie pakietów o zadanej wielkości do serwera
    while (1) {
        // Wypełnianie bufora losowymi danymi
        memset(buffer, 'A', packetSize);

        // Pomiar czasu przed rozpoczęciem wysyłania
    	struct timeval start, end;
	gettimeofday(&start, NULL);

        // Wysyłanie pakietu do serwera
        sendto(sockfd, buffer, packetSize, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        // Odbieranie potwierdzenia od serwera
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);

        // Pomiar czasu po wysłaniu pakietu
        gettimeofday(&end, NULL);
        long seconds = end.tv_sec - start.tv_sec;
        long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

        printf("Wysłano pakiet o rozmiarze %ld bajtów w czasie %ld mikrosekund\n", packetSize, micros);

        // Oczekiwanie przed wysłaniem kolejnego pakietu
        usleep(1000000);  // Czekaj 1 sekundę
    }

    // Zamknięcie gniazda klienta (to miejsce nigdy nie zostanie osiągnięte w nieskończonej pętli)
    close(sockfd);

    return 0;
}
