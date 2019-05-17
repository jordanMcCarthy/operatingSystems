#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pslibrary.h"

int main(int argc, char *argv[]){
    /* input validation */
    if(argc < 7){
        printf("usage: q x1 y1 z1 x2 y2 z2\n");
        exit(EXIT_FAILURE);
    }
    
    // assigns arguments from command line to variables
    // through the ascii to integer function then
    // creates and allocates space for char *
    // variables that will be used for printing
    // the output of the scheduler simulation
    int q = atoi(argv[1]);
    int x1 = atoi(argv[2]);
    int y1 = atoi(argv[3]);
    int z1 = atoi(argv[4]);
    int x2 = atoi(argv[5]);
    int y2 = atoi(argv[6]);
    int z2 = atoi(argv[7]);
    char *s1 = malloc(sizeof(char) * 1024);
    char *s2 = malloc(sizeof(char) * 1024);
    char *s3 = malloc(sizeof(char) * 1024);
    char *s4 = malloc(sizeof(char) * 1024);
    char *s5 = malloc(sizeof(char) * 1024);
    char *s6 = malloc(sizeof(char) * 1024);
    char *s7 = malloc(sizeof(char) * 1024);
    char *s8 = malloc(sizeof(char) * 1024); 

    // Functions for scheduler simulation are called
    // and formatting/output is printed to console
    printf("Assignment 2 program was written by Jordan McCarthy\n");
    printf("%d %d %d %d %d %d %d\n", q,x1,y1,z1,x2,y2,z2);
    printf("\nscheduler fcfs:\n");
    fcfs(s1,s2,x1,y1,z1,x2,y2,z2);
    printReport(s1,s2);
    printf("\nscheduler sjf:\n");
    sjf(s3,s4,x1,y1,z1,x2,y2,z2);
    printReport(s3,s4);
    printf("\nscheduler psjf:\n");
    psjf(s5,s6,x1,y1,z1,x2,y2,z2);
    printReport(s5,s6);
    printf("\nscheduler rr:\n");
    rr(s7,s8,q,x1,y1,z1,x2,y2,z2);
    printReport(s7,s8);


}
