#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Process structure
typedef struct {
    int pid;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
    int priority;
} Process;


void readProcessesFromFile(Process processes[], int *n, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    fscanf(file, "PID Arrival_Time Burst_Time Priority\n");  

    int i = 0;
    while (fscanf(file, "%d %d %d %d", &processes[i].pid, &processes[i].arrivalTime, &processes[i].burstTime, &processes[i].priority) != EOF) {
        i++;
    }
    *n = i;

    fclose(file);
}


void sortByArrivalTime(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].arrivalTime > processes[j].arrivalTime) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

// FCFS Scheduling algorithm
void FCFS(Process processes[], int n) {
    int currentTime = 0;
    printf("\nGantt Chart for FCFS: ");
    for (int i = 0; i < n; i++) {
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
        printf("| P%d ", processes[i].pid);
        currentTime += processes[i].burstTime;
    }
    printf("|\n");

    printf("\nPID  WT  TAT\n");
    int totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        printf("%d    %d   %d\n", processes[i].pid, processes[i].waitingTime, processes[i].turnaroundTime);
        totalWT += processes[i].waitingTime;
        totalTAT += processes[i].turnaroundTime;
    }
    printf("Average WT: %.2f\n", (double)totalWT / n);
    printf("Average TAT: %.2f\n", (double)totalTAT / n);
}

// SJF Scheduling algorithm
void SJF(Process processes[], int n) {
    int currentTime = 0, completed = 0;
    Process readyQueue[MAX];
    int readyCount = 0;

    
    for (int i = 0; i < n; i++) {
        processes[i].waitingTime = -1;
    }

    printf("\nGantt Chart for SJF: ");
    while (completed < n) {
        
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].waitingTime == -1) {
                readyQueue[readyCount++] = processes[i];
            }
        }

        
        if (readyCount > 0) {
           
            int minBurstIndex = 0;
            for (int i = 1; i < readyCount; i++) {
                if (readyQueue[i].burstTime < readyQueue[minBurstIndex].burstTime) {
                    minBurstIndex = i;
                }
            }

            Process p = readyQueue[minBurstIndex];
            printf("| P%d ", p.pid);

            // Updating the waiting and turnaround times
            p.waitingTime = currentTime - p.arrivalTime;
            p.turnaroundTime = p.waitingTime + p.burstTime;
            currentTime += p.burstTime;

            processes[p.pid - 1] = p;  // Updating the original process 
            completed++;

            
            for (int i = minBurstIndex; i < readyCount - 1; i++) {
                readyQueue[i] = readyQueue[i + 1];
            }
            readyCount--;
        } else {
            
            currentTime++;
        }
    }
    printf("|\n");

    printf("\nPID  WT  TAT\n");
    int totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        printf("%d    %d   %d\n", processes[i].pid, processes[i].waitingTime, processes[i].turnaroundTime);
        totalWT += processes[i].waitingTime;
        totalTAT += processes[i].turnaroundTime;
    }
    printf("Average WT: %.2f\n", (double)totalWT / n);
    printf("Average TAT: %.2f\n", (double)totalTAT / n);
}

int main() {
    Process processes[MAX];
    int n;

    // Read the processes 
    readProcessesFromFile(processes, &n, "processes.txt");

    
    sortByArrivalTime(processes, n);

   
    FCFS(processes, n);

    
    sortByArrivalTime(processes, n);

    
    SJF(processes, n);

    return 0;
}
