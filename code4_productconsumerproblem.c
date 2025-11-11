#include <stdio.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 20

void *producer(void *arg);
void *consumer(void *arg);

typedef struct {
    int buffer[BUFFER_SIZE];
    sem_t full, empty;
    int in, out;
} shared_data;

shared_data sh;
sem_t mutex;

int main() {
    pthread_t ptid1, ptid2, ctid1;
    sh.in = 0;
    sh.out = 0;

    sem_init(&sh.empty, 0, BUFFER_SIZE);
    sem_init(&sh.full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&ptid1, NULL, producer, NULL);
    pthread_create(&ptid2, NULL, producer, NULL);
    pthread_create(&ctid1, NULL, consumer, NULL);

    pthread_join(ptid1, NULL);
    pthread_join(ptid2, NULL);
    pthread_join(ctid1, NULL);

    return 0;
}

void *producer(void *arg) {
    int item;
    while (1) {
        item = rand() % 100;
        sem_wait(&sh.empty);
        sem_wait(&mutex);

        sh.buffer[sh.in] = item;
        printf("PRODUCER Thread id = %ld produced Item = %d at index %d\n",
               pthread_self(), item, sh.in);
        sh.in = (sh.in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&sh.full);
        sleep(1);
    }
}

void *consumer(void *arg) {
    int item;
    while (1) {
        sem_wait(&sh.full);
        sem_wait(&mutex);

        item = sh.buffer[sh.out];
        printf("\tCONSUMER Thread id = %ld consumed Item = %d from index %d\n",
               pthread_self(), item, sh.out);
        sh.out = (sh.out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&sh.empty);
        sleep(2);
    }
}





#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/syscall.h>

void *reader(void *argp);
void *writer(void *argp);

int buffer;
int flag = 0;
int read_count = 0;

int getbuff() {
    int temp;
    printf("Enter the number to add in buffer : ");
    scanf("%d", &temp);
    return temp;
}

void readbuff(int buffer) {
    printf("Element read by reader = %d\n", buffer);
}

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wrt = PTHREAD_MUTEX_INITIALIZER;

int main() {
    pthread_t wtid1, rtid1, rtid2;

    pthread_create(&wtid1, NULL, writer, NULL);
    pthread_create(&rtid1, NULL, reader, NULL);
    pthread_create(&rtid2, NULL, reader, NULL);

    pthread_join(wtid1, NULL);
    pthread_join(rtid1, NULL);
    pthread_join(rtid2, NULL);

    return 0;
}

void *writer(void *argp) {
    while (1) {
        pthread_mutex_lock(&wrt);
        if (flag == 0) {
            buffer = getbuff();
            flag = 1;
        }
        pthread_mutex_unlock(&wrt);
    }
}

void *reader(void *argp) {
    while (1) {
        pthread_mutex_lock(&mutex1);
        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&wrt);
        }
        pthread_mutex_unlock(&mutex1);

        if (flag == 1) {
            readbuff(buffer);
            sleep(1);
            flag = 0;
        }

        pthread_mutex_lock(&mutex1);
        read_count--;
        if (read_count == 0) {
            pthread_mutex_unlock(&wrt);
        }
        pthread_mutex_unlock(&mutex1);
    }
}
