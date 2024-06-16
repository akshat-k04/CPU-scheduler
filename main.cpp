#include <iostream>
#include "Scheduler.h"
using namespace std ;

string input_file="C:/Users/HP/programming/3 c++ notes/core c++ concept/lect8 OS/projrct/CPU scheduler/testcases/testcase.txt" ;
int time_quantum =2 ;

int main() {
    ProcessQueue queue = ProcessQueue() ;
    int algo ; 
    

    readInput(algo ,queue,input_file);

    switch (algo) {
        case 1:
            FCFS(queue.processes) ;
            break;
        case 2:
            SJFNP(queue.processes) ;
            break;
        case 3:
            SJFP(queue.processes,time_quantum) ;
            break;
        case 4:
            Priority_scheduling_np(queue.processes) ;
            break;
        case 5:
            Priority_scheduling_p(queue.processes,time_quantum) ;
            break;
        case 6:
            RR(queue.processes,time_quantum) ;
            break;
        case 7:
            MLQ(queue,time_quantum) ;
            break;
        case 8:
            MLFQ(queue,time_quantum) ;
            break;
        default:
            MLFQ(queue,time_quantum) ;
            break;
    }
    return 0;
}