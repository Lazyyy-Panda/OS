#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 5

int b[SIZE];
int in = 0, out = 0;

sem_t empty, full, mutex;

void *producer(void *arg)
{
    for (int item = 0; item < 10; item++)
    {
        sem_wait(&empty);
        sem_wait(&mutex);

        b[in] = item;
        printf("Produced %d at b[%d]\n", item, in);

        in = (in + 1) % SIZE;

        sem_post(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *arg)
{
    int item;

    for (int i = 0; i < 10; i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);

        item = b[out];
        printf("Consumed %d from b[%d]\n", item, out);

        out = (out + 1) % SIZE;

        sem_post(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    pthread_t p, c;

    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    return 0;
}
