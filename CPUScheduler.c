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

// Function to read processes from a file
void readProcessesFromFile(Process processes[], int *n, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    fscanf(file, "PID Arrival_Time Burst_Time Priority\n");  // Skip header

    int i = 0;
    while (fscanf(file, "%d %d %d %d", &processes[i].pid, &processes[i].arrivalTime, &processes[i].burstTime, &processes[i].priority) != EOF) {
        i++;
    }
    *n = i;

    fclose(file);
}

// Function to sort processes by arrival time (for FCFS)
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

    // Initialize waiting times to -1 to indicate not yet processed
    for (int i = 0; i < n; i++) {
        processes[i].waitingTime = -1;
    }

    printf("\nGantt Chart for SJF: ");
    while (completed < n) {
        // Add processes that have arrived by the current time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].waitingTime == -1) {
                readyQueue[readyCount++] = processes[i];
            }
        }

        // If there's a ready queue, sort by burst time and execute the process
        if (readyCount > 0) {
            // Sort the readyQueue by burst time (SJF logic)
            int minBurstIndex = 0;
            for (int i = 1; i < readyCount; i++) {
                if (readyQueue[i].burstTime < readyQueue[minBurstIndex].burstTime) {
                    minBurstIndex = i;
                }
            }

            Process p = readyQueue[minBurstIndex];
            printf("| P%d ", p.pid);

            // Update waiting and turnaround times
            p.waitingTime = currentTime - p.arrivalTime;
            p.turnaroundTime = p.waitingTime + p.burstTime;
            currentTime += p.burstTime;

            processes[p.pid - 1] = p;  // Update the original process array
            completed++;

            // Remove processed process from ready queue
            for (int i = minBurstIndex; i < readyCount - 1; i++) {
                readyQueue[i] = readyQueue[i + 1];
            }
            readyCount--;
        } else {
            // If no processes are ready, increment current time
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

    // Read the processes from a file
    readProcessesFromFile(processes, &n, "processes.txt");

    // Sort processes by arrival time (needed for FCFS)
    sortByArrivalTime(processes, n);

    // Run FCFS Scheduling
    FCFS(processes, n);

    // Sort processes again for SJF (it may alter the order)
    sortByArrivalTime(processes, n);

    // Run SJF Scheduling
    SJF(processes, n);

    return 0;
}
