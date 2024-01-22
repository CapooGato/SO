#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void f_sprzatajaca() {
    printf("\nWypisano przez funkcję sprzątającą\n");
}

void *fun() {

    printf("\nID watku: %ld\n", pthread_self());

    // Wepchnięcie funkcji czyszczącej na stos wywołań
    pthread_cleanup_push(f_sprzatajaca, NULL);

   // sleep(1);

    // Zdjęcie funkcji czyszczącej ze stosu wywołań
    pthread_cleanup_pop(NULL);

    return 0;
}

int main(void) {
    pthread_t w1;

    pthread_create(&w1, 0, fun, NULL);

    sleep(1);

    // Anulowanie wątku
    pthread_cancel(w1);

    // Czekaj na zakończenie wątku
    pthread_join(w1, 0);

    return 0;
}
