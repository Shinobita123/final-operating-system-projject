
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include<climits>
using namespace std;

struct Process {
    string name;
    int arrival_time;
    int burst_time;
    int response_time = 0;
    int waiting_time = 0;
};

// So sanh Arrival Time cua 2 Processes
bool Compare_Arrival_Time(const Process &a, const Process &b) {
    return a.arrival_time < b.arrival_time;
}

// So sanh Burst Time cua 2 Processes
bool Compare_Burst_Time(const Process &a, const Process &b) {
    return a.burst_time < b.burst_time;
}

// Thuat toan First Come First Serve (FCFS)
void FCFS(vector<Process> &listProcess, int total) {
    // Sap xep lai cac Processes theo Arrival Time tang dan
    sort(listProcess.begin(), listProcess.end(), Compare_Arrival_Time);
    int current_time=0;
    cout << "Process\t\tArrival Time\t\tBurst Time\t\tWaiting Time\n";
    for (int i=0;i<total;i++) {
        if(listProcess[i].arrival_time<=current_time){
            listProcess[i].waiting_time = current_time - listProcess[i].arrival_time;
            cout << listProcess[i].name << "\t\t" << listProcess[i].arrival_time << "\t\t\t" << listProcess[i].burst_time << "\t\t\t" << listProcess[i].waiting_time << "\n";
            current_time += listProcess[i].burst_time;
        }
        else{
            current_time++;
        }
    }
}

// Thuat toan Shortest Job First (SJF)
void SJF(vector<Process> &listProcess, int total) {
  // sort(listProcess.begin(), listProcess.end(), Compare_Arrival_Time);
   int current_time=0, completed=0, min_burst_time=INT_MAX, min_index=0;
   bool found = false;
   vector<bool>executed(total,false);
   cout << "Process\t\tArrival Time\t\tBurst Time\t\tWaiting Time\n";
   while(completed!=total){
        for(int i=0;i<total;i++){
            if((listProcess[i].arrival_time<=current_time)&&
                (listProcess[i].burst_time<=min_burst_time)&&
                (!executed[i])){
                    min_burst_time=listProcess[i].burst_time;
                    min_index=i;
                    found=true;
            }
        }
        if(!found){
            current_time++;
            continue;
        }
        executed[min_index]=true;
        listProcess[min_index].waiting_time=current_time-listProcess[min_index].arrival_time;
        completed++;
        current_time+=listProcess[min_index].burst_time;
        cout << listProcess[min_index].name<<"\t\t"<<listProcess[min_index].arrival_time<<"\t\t\t"<<listProcess[min_index].burst_time<<"\t\t\t"<<listProcess[min_index].waiting_time<<"\n";
        min_burst_time=INT_MAX;
   }
}

// Thuat toan Shortest Remaining Time First (SRTF)
void SRTF(vector<Process>listProcess, int total)
{
    sort(listProcess.begin(), listProcess.end(), Compare_Arrival_Time);
    int current_time = 0, completed = 0, min_remaining_time = INT_MAX;
    int shortest = -1;
    bool found = false;
    vector<int> remaining_time(total);
    vector<bool> is_started(total, false);
    
    for (int i = 0; i < total; i++) {
        remaining_time[i] = listProcess[i].burst_time;
    }
    cout << "Process\t\tArrival Time\t\tBurst Time\t\tRespond Time\t\tWaiting Time\n";
    // Lap den khi nao tat ca cac process hoan thanh
    while (completed != total) {
        //
        for (int i = 0; i < total; i++) {
            if ((listProcess[i].arrival_time <= current_time) &&
                (remaining_time[i] < min_remaining_time) &&
                remaining_time[i] > 0) {
                min_remaining_time = remaining_time[i];
                shortest = i;
                found = true;
            }
        }
        
        if (!found) {
            current_time++;
            continue;
        }
        
        if (!is_started[shortest]) {
            listProcess[shortest].response_time = current_time - listProcess[shortest].arrival_time;
            is_started[shortest] = true;
        }
        
        remaining_time[shortest]--;
        min_remaining_time = remaining_time[shortest];
        
        if (min_remaining_time == 0) {
            min_remaining_time = INT_MAX;
        }
        
        if (remaining_time[shortest] == 0) {
            completed++;
            found = false;
            
            listProcess[shortest].waiting_time = (current_time + 1) - listProcess[shortest].arrival_time - listProcess[shortest].burst_time;
            
            cout << listProcess[shortest].name << "\t\t" << listProcess[shortest].arrival_time << "\t\t\t" << listProcess[shortest].burst_time << "\t\t\t" << listProcess[shortest].response_time << "\t\t\t" << listProcess[shortest].waiting_time << "\n";
        }
        
        current_time++;
    }
}

// Thuat toan Round Robin (RR)
void RR(vector<Process>listProcess, int total, int q){
    sort(listProcess.begin(), listProcess.end(), Compare_Arrival_Time);
    int current_time = 0, completed = 0, index=0;
    vector<int>remaining_time(total);
    vector<bool>is_started(total,false);
    bool found = false;
    for(int i=0;i<total;i++){
        remaining_time[i] = listProcess[i].burst_time;
    }
    cout << "Process\t\tArrival Time\t\tBurst Time\t\tRespond Time\t\tWaiting Time\n";
    while(completed!=total){
        for(int i=0;i<total;i++){
            if((listProcess[i].arrival_time<=current_time)&&
                (remaining_time[i]>0)){
                    found = true;
                    index=i;
                    break;
            }
        }
        if(!found){
            current_time++;
            continue;
        }
        if (!is_started[index]) {
            listProcess[index].response_time = current_time - listProcess[index].arrival_time;
            is_started[index] = true;
            is_started.push_back(is_started[index]);
            is_started.erase(is_started.begin());
        }
        current_time+=min(q,remaining_time[index]);
        remaining_time[index]-=(min(q,remaining_time[index]));
        if (remaining_time[index] == 0) {
            completed++;
            found = false;
            
            listProcess[index].waiting_time = current_time - listProcess[index].arrival_time - listProcess[index].burst_time;
            //current_time - listProcess[index].arrival_time - listProcess[index].burst_time;
            cout << listProcess[index].name << "\t\t" << listProcess[index].arrival_time << "\t\t\t" << listProcess[index].burst_time << "\t\t\t" << listProcess[index].response_time << "\t\t\t" << listProcess[index].waiting_time << "\n";
        }
        listProcess.push_back(listProcess[index]);
        remaining_time.push_back(remaining_time[index]);
        listProcess.erase(listProcess.begin());
        remaining_time.erase(remaining_time.begin());
    }
}

int main()
{
    int total_process;
    cin >> total_process;
    vector<Process>listProcess(total_process);
    for(int i=0;i<total_process;i++){
        cin>>listProcess[i].name>>listProcess[i].arrival_time>>listProcess[i].burst_time;
    }
    cout<<"\n\n\n";
    //FCFS(listProcess,total_process);
    //cout<<"\n\n\n";
    //SJF(listProcess, total_process);
    //cout<<"\n\n\n";
    //SRTF(listProcess, total_process);
    //cout<<"\n\n\n";
    //RR(listProcess,total_process,3);
    //cout<<"\n\n\n";
    return 0;
}





