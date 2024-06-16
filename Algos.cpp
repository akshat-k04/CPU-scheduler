#include "Scheduler.h"
#include <bits/stdc++.h>
using namespace std ;



void print_stats(vector<Process> & processes, const string &filename){
    std::ofstream outFile(filename, std::ios_base::app);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    // Write time units header to the file
    outFile<<"Stats\n\n\n" ;
    outFile << setw(20)<<left<<"process";
    for (const auto &process : processes) {
        outFile << std::setw(8) << std::left << process.processID << " ";
        
    }
    outFile << "\n";
    outFile<<"--------------------" ;
    for(int e=0 ;e<processes.size() ;e++)outFile<<"---------" ;
    outFile << "------\n";

    outFile << setw(20)<<left<<"Priority";
    for (const auto &process : processes) {
        outFile << std::setw(8) << std::left << process.priority << " ";
        
    }
    outFile << "\n";

    outFile << setw(20)<<left<<"Arrival";
    for (const auto &process : processes) {
        outFile << std::setw(8) << std::left << process.arrivalTime << " ";
        
    }
    outFile << "\n";

    outFile << setw(20)<<left<<"Brust time";
    for (const auto &process : processes) {
        outFile << std::setw(8) << std::left << process.burstTime << " ";
        
    }
    outFile << "\n";

    double add =0.00 ;
    outFile << setw(20)<<left<<"Turnaround time";
    for (const auto &process : processes) {
        add+=process.turnaroundTime ;
        outFile << std::setw(8) << std::left << process.turnaroundTime << " ";
    }
    outFile << "\n";

    outFile << setw(20)<<left<<"Finish time";
    for (const auto &process : processes) {
        outFile << std::setw(8) << std::left << process.arrivalTime+process.turnaroundTime << " ";
    }
    outFile << "\n\n\n";


    add/=processes.size() ;

    outFile<<"The mean of TurnAround time is "<<add<<"\n" ;
    outFile.close();
}
void print_trace(const vector<int> &trace, vector<Process> & processes, const string &filename) {
    const int timeUnits = trace.size();
    
    // Open file for writing
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    // Write time units header to the file
    outFile << setw(7)<<" ";
    for (int i = 0; i <= timeUnits; ++i) {
        // if (i % 10 == 0 && i != 0) {
        //     outFile <<setw(3)<< "|";
        // }
        outFile <<setw(3)<< i % 10;
    }
    outFile<<"\n---------" ;
    for(int e=0 ;e<timeUnits ;e++)outFile<<"---" ;
    outFile << "-----\n";
    


    // Write each process's execution timeline to the file
    for (const auto &process : processes) {
        outFile << std::setw(10) << std::left << process.processID << " ";
        for (int i = 0; i < timeUnits; ++i) {
            if (trace[i] == process.processID) {
                outFile <<setw(3)<< "*";
            } 
            
            else {
                outFile <<setw(3)<< ".";
            }
        }
        outFile << "\n";
    }
    outFile<<"---------" ;
    for(int e=0 ;e<timeUnits ;e++)outFile<<"---" ;
    outFile << "--\n";

    // Close the file
    outFile << "\n\n\n";
    outFile.close();
    print_stats(processes,filename) ;
}




void sort_brustTime(std::vector<pair<Process,int>>::iterator begin, std::vector<pair<Process,int>>::iterator end) {
    // Sort the processes based on burst time using iterators 'begin' and 'end'
    std::sort(begin, end, [](const pair<Process,int>& a, const pair<Process,int>& b) {
        if(a.first.burstTime == b.first.burstTime)return a.first.priority < b.first.priority ;
        return a.first.burstTime < b.first.burstTime;
    });
}
void sort_arrivalTime(std::vector<Process>::iterator begin, std::vector<Process>::iterator end) {
    // Sort the Processes array based on the process arrivalTime
    sort(begin,end, [](const Process &a, const Process &b) {
        if(a.arrivalTime == b.arrivalTime)return a.priority < b.priority ;
        return a.arrivalTime < b.arrivalTime;
    });
}
void sort_priority(std::vector<pair<Process,int>>::iterator begin, std::vector<pair<Process,int>>::iterator end) {
    // Sort the processes based on priority using iterators 'begin' and 'end'
    std::sort(begin, end, [](const pair<Process,int>& a, const pair<Process,int>& b) {
        return a.first.priority > b.first.priority;
    });
}
void sort_remainingTime(std::vector<pair<Process,int>>::iterator begin, std::vector<pair<Process,int>>::iterator end) {
    // Sort the processes based on burst time using iterators 'begin' and 'end'
    std::sort(begin, end, [](const pair<Process,int>& a, const pair<Process,int>& b) {
        if(a.first.remainingTime == b.first.remainingTime)return a.first.priority < b.first.priority ;
        return a.first.remainingTime < b.first.remainingTime;
    });
}




void FCFS(vector<Process> & processes){
    int total_time =0 ;
    for(auto &it:processes)total_time=max(it.burstTime+it.arrivalTime,total_time+it.burstTime) ;

    auto & tp =processes ;
    sort_arrivalTime(processes.begin(),processes.end()) ;

    vector<int> time ; // time[0] = it will give the name of process which start run at 0sec

    int index =0 ;
    for(int tim =0 ; tim<total_time ; tim++){
        if(tp[index].arrivalTime<=tim){
            tp[index].remainingTime-- ;
            time.push_back(tp[index].processID) ;
            if(tp[index].remainingTime==0){
                tp[index].turnaroundTime = tim -tp[index].arrivalTime ; 
                tp[index].waitingTime = tp[index].turnaroundTime - tp[index].burstTime ;
                index++ ;
            }
        }
        else time.push_back(-1) ;
    }
    print_trace(time,processes,"_output_FCFS.txt") ;
}
void SJFNP(vector<Process> & processes){

    int total_time =0 ;
    for(auto &it:processes)total_time=max(it.burstTime+it.arrivalTime,total_time+it.burstTime) ;

    auto & tp =processes ;
    sort_arrivalTime(processes.begin(),processes.end()) ;

    vector<int> time ; // time[0] = it will give the name of process which start run at 0sec

    int index =0 ;

    vector<pair<Process,int>> localq ;
    bool process_completed = true ;
    for(int tim=0 ; tim<total_time ;tim++){
        while(index<tp.size() && tp[index].arrivalTime<=tim){
            localq.push_back({processes[index],index});
            index++;
        }

        if(localq.size()==0){
            time.push_back(-1) ;
            continue ;
        }
        if(process_completed)sort_brustTime(localq.begin() , localq.end()) ;
        process_completed= false ;
        tp[localq.front().second].remainingTime-- ;
        time.push_back(tp[localq.front().second].processID) ;

        if(tp[localq.front().second].remainingTime==0){
            tp[localq.front().second].turnaroundTime = tim-tp[localq.front().second].arrivalTime ;
            tp[localq.front().second].waitingTime=tp[localq.front().second].turnaroundTime-tp[localq.front().second].burstTime ;
            localq.erase(localq.begin()) ;
            process_completed=true ;
        }
    }

    print_trace(time,processes,"_output_SJFNP.txt") ;
    
}
void SJFP(vector<Process> & processes,int time_quantum){
    int total_time =0 ;
    for(auto &it:processes)total_time=max(it.burstTime+it.arrivalTime,total_time+it.burstTime) ;

    auto & tp =processes ;
    sort_arrivalTime(processes.begin(),processes.end()) ;

    vector<int> time ; // time[0] = it will give the name of process which start run at 0sec

    int index =0 ;

    vector<pair<Process,int>> localq ;

    for(int tim=0 ; tim<total_time ;tim++){
        while(index<tp.size() && tp[index].arrivalTime<=tim){
            localq.push_back({processes[index],index});
            index++;
        }

        if(localq.size()==0){
            time.push_back(-1) ;
            continue ;
        }
        if(localq.front().first.remainingTime%time_quantum== localq.front().first.burstTime%time_quantum ){
            sort_remainingTime(localq.begin() , localq.end()) ;
        }
        
        tp[localq.front().second].remainingTime-- ;localq.front().first.remainingTime-- ;
        time.push_back(tp[localq.front().second].processID) ;

        if(tp[localq.front().second].remainingTime==0){
            tp[localq.front().second].turnaroundTime = tim-tp[localq.front().second].arrivalTime ;
            tp[localq.front().second].waitingTime=tp[localq.front().second].turnaroundTime-tp[localq.front().second].burstTime ;
            localq.erase(localq.begin()) ;
        
        }
    }

    print_trace(time,processes,"_output_SJFP.txt") ;
    
}
void Priority_scheduling_np(vector<Process> & processes){
    int total_time =0 ;
    for(auto &it:processes)total_time=max(it.burstTime+it.arrivalTime,total_time+it.burstTime) ;

    auto & tp =processes ;
    sort_arrivalTime(processes.begin(),processes.end()) ;

    vector<int> time ; // time[0] = it will give the name of process which start run at 0sec

    int index =0 ;

    vector<pair<Process,int>> localq ;
    bool process_completed = true ;
    for(int tim=0 ; tim<total_time ;tim++){
        while(index<tp.size() && tp[index].arrivalTime<=tim){
            localq.push_back({processes[index],index});
            index++;
        }

        if(localq.size()==0){
            time.push_back(-1) ;
            continue ;
        }
        if(process_completed)sort_priority(localq.begin() , localq.end()) ;
        process_completed= false ;
        tp[localq.front().second].remainingTime-- ;
        time.push_back(tp[localq.front().second].processID) ;

        if(tp[localq.front().second].remainingTime==0){
            tp[localq.front().second].turnaroundTime = tim-tp[localq.front().second].arrivalTime ;
            tp[localq.front().second].waitingTime=tp[localq.front().second].turnaroundTime-tp[localq.front().second].burstTime ;
            localq.erase(localq.begin()) ;
            process_completed=true ;
        }
    }

    print_trace(time,processes,"_output_PSNP.txt") ;
    
}
void Priority_scheduling_p(vector<Process> & processes,int time_quantum){
    int total_time =0 ;
    for(auto &it:processes)total_time=max(it.burstTime+it.arrivalTime,total_time+it.burstTime) ;

    auto & tp =processes ;
    sort_arrivalTime(processes.begin(),processes.end()) ;

    vector<int> time ; // time[0] = it will give the name of process which start run at 0sec

    int index =0 ;

    vector<pair<Process,int>> localq ;

    for(int tim=0 ; tim<total_time ;tim++){
        while(index<tp.size() && tp[index].arrivalTime<=tim){
            localq.push_back({processes[index],index});
            index++;
        }

        if(localq.size()==0){
            time.push_back(-1) ;
            continue ;
        }

        if(localq.front().first.remainingTime%time_quantum== localq.front().first.burstTime%time_quantum ){
            sort_priority(localq.begin() , localq.end()) ;
        }
        
        tp[localq.front().second].remainingTime-- ;localq.front().first.remainingTime-- ;
        time.push_back(tp[localq.front().second].processID) ;

        if(tp[localq.front().second].remainingTime==0){
            tp[localq.front().second].turnaroundTime = tim-tp[localq.front().second].arrivalTime ;
            tp[localq.front().second].waitingTime=tp[localq.front().second].turnaroundTime-tp[localq.front().second].burstTime ;
            localq.erase(localq.begin()) ;
            
        }
    }

    print_trace(time,processes,"_output_PSP.txt") ;
    
}
void RR(vector<Process> & processes,int time_quantum){
    int total_time =0 ;
    for(auto &it:processes)total_time=max(it.burstTime+it.arrivalTime,total_time+it.burstTime) ;

    auto & tp =processes ;
    sort_arrivalTime(processes.begin(),processes.end()) ;

    vector<int> time ; // time[0] = it will give the name of process which start run at 0sec

    int index =0 ;

    vector<pair<Process,int>> localq ;

    for(int tim =0 ; tim<total_time ; tim++){
        while(index<tp.size() && tp[index].arrivalTime<=tim){
            localq.push_back({processes[index],index});
            index++;
        }

        if(localq.size()==0){
            time.push_back(-1) ;
            continue ;
        }

        if(localq.front().first.remainingTime%time_quantum== localq.front().first.burstTime%time_quantum && localq.front().first.remainingTime !=localq.front().first.burstTime){
            localq.front().first.burstTime=localq.front().first.remainingTime  ;
            localq.push_back(localq.front()) ;
            localq.erase(localq.begin()) ;
        }
        // for(auto & v:localq)cout<<v.first.processID<<" ";cout<<endl ;

        tp[localq.front().second].remainingTime-- ;localq.front().first.remainingTime-- ;
        time.push_back(tp[localq.front().second].processID) ;


        if(tp[localq.front().second].remainingTime==0){
            tp[localq.front().second].turnaroundTime = tim-tp[localq.front().second].arrivalTime ;
            tp[localq.front().second].waitingTime=tp[localq.front().second].turnaroundTime-tp[localq.front().second].burstTime ;
            localq.erase(localq.begin()) ;
        }
        
    }
    print_trace(time,processes,"_output_RR.txt") ;
}
void MLQ(ProcessQueue &queue,int time_quantum){
    // I have 3 queue 
    // I will use priority scheduling for the system process 
    // round robin for the interactive process
    // FCFS for the background
    
    
    // multiset<pair<int,pair<Process,int>>> sys_queue , inter_queue , back_queue ; 
    // // set.first = arrival time  set.second.first = process , set.second.second = index in the main vector
    // sort_priority(queue.system_Processes.begin() , queue.system_Processes.end()) ;
    // sort_arrivalTime(queue.interactive_Processes.begin() , queue.interactive_Processes.end()) ;
    // sort_arrivalTime(queue.background_Processes.begin() , queue.background_Processes.end()) ;

    // for(int e=0 ;e<queue.system_Processes.size() ;e++)sys_queue.insert({queue.system_Processes[e].arrivalTime,{queue.system_Processes[e],e}}) ;
    // for(int e=0 ;e<queue.interactive_Processes.size() ;e++)inter_queue.insert({queue.interactive_Processes[e].arrivalTime,{queue.interactive_Processes[e],e}}) ;
    // for(int e=0 ;e<queue.background_Processes.size() ;e++)back_queue.insert({queue.background_Processes[e].arrivalTime,{queue.background_Processes[e],e}}) ;


    // vector<int> time_line ;
    // int time =0 ;

    // while(total_process!=0){// on every iteration I have to increase the time
    //     if(system){

    //     }
    //     else if(interactive){

    //     }
    //     else{ // background

    //     }
    // }
} 
void MLFQ(ProcessQueue &queue,int time_quantum){} 
void Mix_algo(ProcessQueue &queue,int time_quantum){} 
