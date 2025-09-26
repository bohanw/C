#include <stdio.h>  
#include <pthread.h>  
#include <semaphore.h> 
#include  <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define SIZE 10


typedef int buffer_item;

//empty : init as the number of buffer size
//full : equals to the number
sem_t empty, full;

pthread_mutex_t mutex;
buffer_item buffer[SIZE];
int cnt,in_ptr, out_ptr;


void *producer();
void *consumer();
int insert(buffer_item elem);
int remove_item(buffer_item *elem);

//Implementation
int insert(buffer_item elem){
    int status;
    //acquire empty semaphore
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    if(cnt != SIZE) {
        buffer[in_ptr] = elem;
        in_ptr = (in_ptr + 1) % SIZE;
        cnt++;
        status = 0;
    }
    else {
        status = -1;
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return status;
}

int remove_item(buffer_item *elem){

    int sts;

    //Acquare sempahore
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    //enter critical section
    
    if (cnt != 0){
        *elem = buffer[out_ptr];
        out_ptr = (out_ptr + 1) % SIZE;
        cnt--;
        sts = 0;
    }
    else {
        sts = -1;
    }
    //release sempahre a
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return sts;
}
void *producer(){
    buffer_item item;

    while(1){
        sleep(rand());
        item = rand();
        if(insert(item)){
            printf("producer produce fail\n");
        }
        else {
            printf("producer insert item %d\n", item);
        }
    }
}

void* consumer(){
    buffer_item item;
    while(1){
        sleep(rand() % 5 + 1);
        if(remove_item(&item)){
            printf("error remove");
        } 
        else {
            printf("consumer consumed %d", item);
        }
    }
}
int main (){
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    cnt = in_ptr = out_ptr = 0;

    pthread_t producers[2];
    pthread_t consumers[2];

    for(int i = 0; i < 2;i++){
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    for(int i = 0; i < 2;i++){
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }    
    return 0;
}