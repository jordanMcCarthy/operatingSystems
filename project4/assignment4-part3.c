#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
void creatPhilosophers(int nthreads);
void *philosopherThread(void *pVoid);
void pickUpChopsticks();
void putDownChopsticks(); 
void eating();
void thinking();

typedef struct thread_info{
    pthread_t thread_id;
    int thread_num;
}   thread_info;

pthread_mutex_t chopsticks;
pthread_cond_t conditionalVariable;
int nextIndex = 0;

int main(int argc, char *argv[]){
    srand(time(NULL));
    int nthreads = atoi(argv[1]);
    printf("Jordan McCarthy\n");
    printf("Assignment 4: # of threads = %d\n", nthreads);
    creatPhilosophers(nthreads);

}

void creatPhilosophers(int nthreads){
    struct thread_info *tinfo;
    int NUM_THREADS = nthreads;
    int t; 
    tinfo = calloc(NUM_THREADS, sizeof(struct thread_info));
    pthread_mutex_init(&chopsticks,NULL);
    

    for(t=0;t<NUM_THREADS;t++){
        tinfo[t].thread_num = t;
        pthread_create(&tinfo[t].thread_id, NULL, philosopherThread, (void *) &tinfo[t].thread_num);
    }
    
    for(t=0;t<NUM_THREADS;t++){
        pthread_join(tinfo[t].thread_id, NULL);
        } 

    pthread_mutex_destroy(&chopsticks);
    
    printf("%d threads have been completed/joined successfully!\n", NUM_THREADS);        
}


void *philosopherThread(void *pVoid){
    int philosopherNumber = *((int *) pVoid);
    printf("Philosopher #-%d started thinking\n", philosopherNumber);
    thinking();
    printf("Philosopher #-%d ended thinking\n", philosopherNumber);
    pickUpChopsticks(philosopherNumber);
    printf("Philosopher #-%d started eating\n", philosopherNumber);
    eating();
    printf("Philosopher #-%d ended eating\n", philosopherNumber);
    putDownChopsticks(philosopherNumber);
    nextIndex++; 
}

void thinking(){
    usleep(rand() % 501);
}

void eating(){
    usleep(rand() % 501);
}


void pickUpChopsticks(int threadIndex){
    pthread_mutex_lock(&chopsticks);
    while(threadIndex != nextIndex){
        pthread_cond_wait(&conditionalVariable, &chopsticks);
    }
    pthread_mutex_unlock(&chopsticks);
}

void putDownChopsticks(int threadIndex){
   pthread_cond_broadcast(&conditionalVariable);
}

