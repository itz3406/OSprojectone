#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid, arrival_time, burst_time, priority, waiting_time, turnaround_time;
};

void readProcesses(vector<Process> &processes, string filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    string header;
    getline(file, header); // Skip the header line
    Process p;
    while (file >> p.pid >> p.arrival_time >> p.burst_time >> p.priority) {
        p.waiting_time = 0;
        p.turnaround_time = 0;
        processes.push_back(p);
    }
    file.close();
}

void fcfsScheduling(vector<Process> &processes) {
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrival_time < b.arrival_time;
    });
    int current_time = 0;
    for (auto &p : processes) {
        if (current_time < p.arrival_time) current_time = p.arrival_time;
        p.waiting_time = current_time - p.arrival_time;
        p.turnaround_time = p.waiting_time + p.burst_time;
        current_time += p.burst_time;
    }
}

void sjfScheduling(vector<Process> &processes) {
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.burst_time < b.burst_time;
    });
    int current_time = 0;
    for (auto &p : processes) {
        if (current_time < p.arrival_time) current_time = p.arrival_time;
        p.waiting_time = current_time - p.arrival_time;
        p.turnaround_time = p.waiting_time + p.burst_time;
        current_time += p.burst_time;
    }
}

void printResults(vector<Process> &processes, string algo) {
    cout << "\n" << algo << " Scheduling Results:" << endl;
    cout << "PID  Arrival  Burst  Waiting  Turnaround" << endl;
    int total_wt = 0, total_tat = 0;
    for (auto &p : processes) {
        cout << p.pid << "\t" << p.arrival_time << "\t" << p.burst_time << "\t" << p.waiting_time << "\t" << p.turnaround_time << endl;
        total_wt += p.waiting_time;
        total_tat += p.turnaround_time;
    }
    cout << "Average Waiting Time: " << (float)total_wt / processes.size() << endl;
    cout << "Average Turnaround Time: " << (float)total_tat / processes.size() << endl;
}

int main() {
    vector<Process> processes;
    readProcesses(processes, "processes.txt");
    
    vector<Process> fcfs_processes = processes;
    fcfsScheduling(fcfs_processes);
    printResults(fcfs_processes, "FCFS");
    
    vector<Process> sjf_processes = processes;
    sjfScheduling(sjf_processes);
    printResults(sjf_processes, "SJF");
    
    return 0;
}
