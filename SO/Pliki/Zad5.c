#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/*l_type: Określa typ blokady. Może przyjmować jedną z wartości:

    F_RDLCK: Blokada dzielona (do odczytu).
    F_WRLCK: Blokada wyłączna (do zapisu).
    F_UNLCK: Zwalnianie blokady.

l_whence: Określa, od czego zaczyna się blokada. Może przyjąć jedną z wartości:

    SEEK_SET: Od początku pliku.
    SEEK_CUR: Od bieżącej pozycji wskaźnika pliku.
    SEEK_END: Od końca pliku.

l_start: Określa początkowy punkt, od którego ma zostać ustawiona blokada, w odniesieniu do l_whence.

l_len: Określa długość blokady (liczbę bajtów), która ma zostać ustawiona. Jeżeli l_len wynosi zero, to blokada obejmuje cały plik.

l_pid: Zwykle nie używane w przypadku ustawiania blokad za pomocą fcntl(). */

void obsluz_potomka(int plik, int procesID, int lockType, off_t offset, off_t len) {
    printf("\nObsluguje proces o ID: %d\n\n", getpid());

    struct flock lock;
    lock.l_type = lockType;
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = len;

    if (fcntl(plik, F_SETLK, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    if (lockType == F_WRLCK) {
        printf("\nProces o ID: %d, ma plik na wylacznosc\n\n", getpid());
        sleep(2);
    } else {
        printf("\nProces o ID: %d, dzieli sie plikiem\n\n", getpid());
        sleep(2);
    }

    lock.l_type = F_UNLCK;

    if (fcntl(plik, F_SETLK, &lock) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }

    printf("\nProces o ID: %d, zwolil plik\n", getpid());
    exit(EXIT_SUCCESS);
}

int main(void) {
    int plik = open("PlikZad4.txt", O_RDWR | O_CREAT, 0600);
    if (plik == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        int procesID = fork();

        if (procesID == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (procesID == 0) { // Potomek
            if (i % 2 == 0)
                obsluz_potomka(plik, procesID, F_WRLCK, 0, 10); // Blokowanie pierwszych 10 bajtów
            else
                obsluz_potomka(plik, procesID, F_RDLCK, 10, 10); // Blokowanie kolejnych 10 bajtów
        }
    }

    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    if (close(plik) != 0)
        perror("close");

    return 0;
}
