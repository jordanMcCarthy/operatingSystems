#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
void creatPhilosophers(int nthreads);
void *philosopherThread(void *pVoid);
void pickUpChopsticks(int threadIndex);
void putDownChopsticks(int threadIndex); 
void eating();
void thinking();

typedef struct thread_info{
    pthread_t thread_id;
    int thread_num;
}   thread_info;

pthread_mutex_t *chopstick;
int main(int argc, char *argv[]){
    srand(time(NULL));
    int nthreads = atoi(argv[1]);
    chopstick = malloc(sizeof(pthread_mutex_t) * nthreads + 1);
    printf("Jordan McCarthy\n");
    printf("Assignment 4: # of threads = %d\n", nthreads);
    creatPhilosophers(nthreads);

}

void creatPhilosophers(int nthreads){
    struct thread_info *tinfo;
    int NUM_THREADS = nthreads;
    int t;
    
    tinfo = calloc(NUM_THREADS, sizeof(struct thread_info));
    for(t=0;t<NUM_THREADS+1;t++){
        pthread_mutex_init(&chopstick[t],NULL);
    }

    for(t=0;t<NUM_THREADS;t++){
        tinfo[t].thread_num = t;
        pthread_create(&tinfo[t].thread_id, NULL, philosopherThread, (void *) &tinfo[t].thread_num);
    }
    
    for(t=0;t<NUM_THREADS;t++){
        pthread_join(tinfo[t].thread_id, NULL);
        } 

    for(t=0;t<NUM_THREADS+1;t++){
        pthread_mutex_destroy(&chopstick[t]);
    }
    printf("%d threads have been completed/joined successfully!\n", NUM_THREADS);        
}


void *philosopherThread(void *pVoid){
    int philosopherNumber = *((int *) pVoid);
    printf("Philosopher #-%d started thinking\n", philosopherNumber);
    thinking();
    printf("Philosopher #-%d ended thinking\n", philosopherNumber);
    pickUpChopsticks(philosopherNumber);
    printf("Philosopher #-%d starting eating\n", philosopherNumber);
    eating();
    printf("Philosopher #-%d ended eating\n", philosopherNumber);
    putDownChopsticks(philosopherNumber);
    
}

void thinking(){
    usleep(rand() % 500 + 1);
}

void eating(){
    usleep(rand() % 500 + 1);
}

void pickUpChopsticks(int threadIndex){
    int leftChopstick = threadIndex;
    int rightChopstick = (threadIndex + 1) % 5;
    pthread_mutex_lock(&chopstick[leftChopstick]);
    pthread_mutex_lock(&chopstick[rightChopstick]);
}

void putDownChopsticks(int threadIndex){
    int leftChopstick = threadIndex;
    int rightChopstick = (threadIndex + 1) % 5;
    pthread_mutex_unlock(&chopstick[leftChopstick]);
    pthread_mutex_unlock(&chopstick[rightChopstick]);

}
