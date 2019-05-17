#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
void creatPhilosophers(int nthreads);
void *philosopherThread(void *pVoid);

typedef struct thread_info{
    pthread_t thread_id;
    int thread_num;
}   thread_info;

int main(int argc, char *argv[]){

    int nthreads = atoi(argv[1]);
    printf("Jordan McCarthy\n");
    printf("Assignment 4: # of threads = %d\n", nthreads);
    creatPhilosophers(nthreads);

}


void creatPhilosophers(int nthreads){
    struct thread_info *tinfo;
    int NUM_THREADS = nthreads;
    int t;
    pthread_t threads[NUM_THREADS];
    tinfo = calloc(NUM_THREADS, sizeof(struct thread_info));

    for(t=0;t<NUM_THREADS;t++){
        tinfo[t].thread_num = t + 1;
        pthread_create(&tinfo[t].thread_id, NULL, philosopherThread, (void *) &tinfo[t].thread_num);
    }
    
    for(t=0;t<NUM_THREADS;t++){
        pthread_join(tinfo[t].thread_id, NULL);
        } 
    printf("%d threads have been completed/joined successfully!\n", NUM_THREADS);        
}


void *philosopherThread(void *pVoid){
    int philosopherNumber = *((int *) pVoid);
    printf("I'm philosopher %d\n", philosopherNumber);

}
