#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t chopstick[N];

void *philosopher(void *num) {
    int id = *(int *)num;

    for (int i = 0; i < 2; i++) {
        printf("Philosopher %d is thinking\n", id);

        sem_wait(&chopstick[id]);
        sem_wait(&chopstick[(id + 1) % N]);

        printf("Philosopher %d is eating\n", id);
        sleep(1);

        sem_post(&chopstick[id]);
        sem_post(&chopstick[(id + 1) % N]);
    }

    return NULL;
}

int main() {
    pthread_t ph[N];
    int id[N];

    for (int i = 0; i < N; i++)
        sem_init(&chopstick[i], 0, 1);

    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&ph[i], NULL, philosopher, &id[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(ph[i], NULL);

    return 0;
}
