#define READY  0
#define RUNNING  1
#define WAITING  2
#define DONE  3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char stateChars[] = {'r','R','w',0};

/* prints report on a scheduler simulation function */
void printReport(char *s1, char *s2){
    int i; // counter for output
    int s1Waiting = 0; // waiting time for process1
    int s2Waiting = 0; // waiting time for proscess2
    int totalRunning; // total time running of each process
    float avgWaiting; // average waiting time between the two processes
    float longString; // stores amount of  longest string of the two processes
    float cpuUtilization; // variable for CPU Utilization
    
    for(i=0;i<=100;i++){
        printf("%c", s1[i]);
        if(s1[i] == 'r'){
            s1Waiting++;
            }
        if(s1[i] == 'R'){
            totalRunning++;
            }
        }
    printf("\n");
   
    for(i=0;i<=100;i++){
       printf("%c", s2[i]);
       if(s2[i] == 'r'){
           s2Waiting++;
        }
       if(s2[i] == 'R'){
           totalRunning++;
        }
     
    }
    
    if((float)strlen(s1) > (float)strlen(s2)){
        longString = (float)strlen(s1);
    }else{
        longString  = (float)strlen(s2);
    }
    cpuUtilization = (float)totalRunning / longString;
    printf("\n");
    avgWaiting = (((float)s1Waiting + (float)s2Waiting) / 2); 
    printf("%d %d %.1f %f\n", s1Waiting, s2Waiting, avgWaiting, cpuUtilization);

    }
        
/* 1) handle state changes:
         running process completes CPU burst
         running process has quantum expire
         IO complete
   2) do context switch if necessary
         both ready
         one ready and CPU free
   3) append appropriate characters to character arrays
         avoid putting in multiple string terminators
*/
/* assume s1 and s2 point to buffers with enough space to hold the result */
/* assume that the int parameters are strictly greater than 0 */
void fcfs(char *s1, char *s2, int x1, int y1, int z1,
                                            int x2, int y2, int z2) {
   int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
   int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */
   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
                                /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1== 0)) {
         if (ioLeft1 == 0) {
            state1 = DONE;
            s1[i] = stateChars[state1];            /* terminate the string */
         }
         else
            state1 = WAITING;
      }  
      else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
         if (ioLeft2 == 0){
             state2 = DONE;
             s2[i] = stateChars[state2];
         }
         else 
             state2 = WAITING;
      }  
                                                     /* handle IO complete */
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          cpuLeft1 = z1;
      }  
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          cpuLeft2 = z2;
      }  
                                    /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
              state1 = RUNNING;
              state2 = READY;
      }  
                                     /* handle one ready and CPU available */
      else if ( (state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;
      }  
      else if ( (state2 == READY) && (state1 != RUNNING)) {
         state2 = RUNNING;

      }  
   /* insert chars in string, but avoid putting in extra string terminators */
      if (state1 != DONE)
         s1[i] = stateChars[state1];
      if (state2 != DONE)
         s2[i] = stateChars[state2];
                                                        /* decrement counts */
//      qleft--;                   /* OK to decrement even if nothing running */
      if (state1 == RUNNING)
         cpuLeft1--;
      if (state1 == WAITING)
         ioLeft1--;
      if (state2 == RUNNING)
         cpuLeft2--;
      if (state2 == WAITING)
         ioLeft2--;
   }                                               /* end of main for loop */
  
}


 void sjf(char *s1, char *s2, int x1, int y1, int z1,
                                              int x2, int y2, int z2) {
     int i;                                   /* next string position (time) */
     int state1 = READY;                            /* start with both ready */
     int state2 = READY;
     int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
     int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
     int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
     int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */
     for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
                                  /* running process completes its CPU burst */
        if ((state1 == RUNNING) && (cpuLeft1== 0)) {
           if (ioLeft1 == 0) {
              state1 = DONE;
              s1[i] = stateChars[state1];            /* terminate the string */
           }
           else
              state1 = WAITING;
        }
        else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
           if (ioLeft2 == 0){
               state2 = DONE;
               s2[i] = stateChars[state2];
           }
          else
               state2 = WAITING;
        }
                                                       /* handle IO complete */
        if ((state1 == WAITING) && (ioLeft1 == 0)) {
            state1 = READY;
            cpuLeft1 = z1;
        }
        if ((state2 == WAITING) && (ioLeft2 == 0)) {
            state2 = READY;
            cpuLeft2 = z2;
        }
                                      /* if both ready, depends on algorithm */
        if ( (state1 == READY) && (state2 == READY)) {
            if(cpuLeft1 < cpuLeft2){
                state1 = RUNNING;
                state2 = READY;
            }else{
                state1 = READY;
                state2 = RUNNING;
            }
       }                                      /* handle one ready and CPU available */
        else if ( (state1 == READY) && (state2 != RUNNING)) {
          state1 = RUNNING;
       }
        else if ( (state2 == READY) && (state1 != RUNNING)) {
          state2 = RUNNING;
 
       }  
    /* insert chars in string, but avoid putting in extra string terminators */
        if (state1 != DONE)
          s1[i] = stateChars[state1];
        if (state2 != DONE)
          s2[i] = stateChars[state2];
                                                         /* decrement counts */
 //      qleft--;                   /* OK to decrement even if nothing running */
        if (state1 == RUNNING)
          cpuLeft1--;
        if (state1 == WAITING)
          ioLeft1--;
        if (state2 == RUNNING)
          cpuLeft2--;
        if (state2 == WAITING)
          ioLeft2--;
    }                                               /* end of main for loop */
   
 }

 void psjf(char *s1, char *s2, int x1, int y1, int z1,
                                              int x2, int y2, int z2) {
     int i;                                   /* next string position (time) */
     int state1 = READY;                            /* start with both ready */
     int state2 = READY;
     int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
     int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
     int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
     int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */
     for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
                                  /* running process completes its CPU burst */
        if ((state1 == RUNNING) && (cpuLeft1== 0)) {
           if (ioLeft1 == 0) {
              state1 = DONE;
              s1[i] = stateChars[state1];            /* terminate the string */
           }
           else
              state1 = WAITING;
        }
        else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
           if (ioLeft2 == 0){
               state2 = DONE;
               s2[i] = stateChars[state2];
           }
          else
               state2 = WAITING;
        }
                                                       /* handle IO complete */
        if ((state1 == WAITING) && (ioLeft1 == 0)) {
            state1 = READY;
            cpuLeft1 = z1;
        }
        if ((state2 == WAITING) && (ioLeft2 == 0)) {
            state2 = READY;
            cpuLeft2 = z2;
        }
                                      /* if both ready, depends on algorithm */
        if ( (state1 == READY) && (state2 == READY)) {
            if(cpuLeft1 <= cpuLeft2){
                state1 = RUNNING;
                state2 = READY;
            }else{
                state1 = READY;
                state2 = RUNNING;
            }
       }  
       else if ( (state1 == RUNNING) && (cpuLeft2 < cpuLeft1) && (state2 == READY) ){
           state1 = READY;
           state2 = RUNNING;
       }
       else if ( (state2 == RUNNING) && (cpuLeft1 < cpuLeft2) && (state1 == READY) ){
           state2 = READY;
           state1 = RUNNING;
       }
       /* handle one ready and CPU available */
        else if ( (state1 == READY) && (state2 != RUNNING)) {
          state1 = RUNNING;
       }
        else if ( (state2 == READY) && (state1 != RUNNING)) {
          state2 = RUNNING;
 
       }  
    /* insert chars in string, but avoid putting in extra string terminators */
        if (state1 != DONE)
          s1[i] = stateChars[state1];
        if (state2 != DONE)
          s2[i] = stateChars[state2];
                                                         /* decrement counts */
 //      qleft--;                   /* OK to decrement even if nothing running */
        if (state1 == RUNNING)
          cpuLeft1--;
        if (state1 == WAITING)
          ioLeft1--;
        if (state2 == RUNNING)
          cpuLeft2--;
        if (state2 == WAITING)
          ioLeft2--;
    }                                               /* end of main for loop */
   
 }


void rr(char *s1, char *s2, int quantum, int x1, int y1, int z1,
                                            int x2, int y2, int z2) {
   int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
   int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */
   int qleft = quantum;                                        /* quantum remaining */

   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
     //  printf("qleft: %d\n", qleft);
                                /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1== 0)) {
         if (ioLeft1 == 0) {
            state1 = DONE;
            s1[i] = stateChars[state1];            /* terminate the string */
         }
         else
            state1 = WAITING;
      }  
      else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
        if (ioLeft2 == 0){
            state2 = DONE;
            s2[i] = stateChars[state2];
        }
        else
            state2 = WAITING;
      }  
                                     /* running process has quantum expire */
      if ((state1 == RUNNING) && (qleft == 0) ) {
          state1 = READY;
          qleft = quantum;
          if(state2 == READY){
              state2 = RUNNING;
          }
      }
        
      if ((state2 == RUNNING) && (qleft == 0) ) {
           state2 = READY;
           qleft = quantum;
           if(state1 == READY){
               state1 = RUNNING;
           }
      } 
                                                     /* handle IO complete */
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          cpuLeft1 = z1;
      }  
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          cpuLeft2 = z2;
      }  
                                    /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
          state1 = RUNNING;
          state2 = READY;
          qleft = quantum;
      }  
                                     /* handle one ready and CPU available */
      else if ( (state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;
         qleft = quantum;
      }  
      else if ( (state2 == READY) && (state1 != RUNNING)) {
          state2 = RUNNING;
          qleft = quantum;
      }
  
    /* insert chars in string, but avoid putting in extra string terminators */
        if (state1 != DONE)
          s1[i] = stateChars[state1];
        if (state2 != DONE)
          s2[i] = stateChars[state2];
                                                         /* decrement counts */
        qleft--;                   /* OK to decrement even if nothing running */
        if (state1 == RUNNING)
          cpuLeft1--;
        if (state1 == WAITING)
          ioLeft1--;
        if (state2 == RUNNING)
          cpuLeft2--;
        if (state2 == WAITING)
          ioLeft2--;
    }                                               /* end of main for loop */
   
 }



