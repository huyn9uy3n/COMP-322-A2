#include <stdio.h>
#include <stdlib.h>

// Define the structure for a process
struct Process {
    int id;
    int arrival;
    int total_cpu;
    int total_remaining;
    int done;
    int start_time;
    int already_started;
    int end_time;
    int turnaround_time;
};

// Functions
void enterParameters(struct Process *processes, int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        printf("Enter arrival cycle for process %d: ", i + 1);
        scanf("%d", &processes[i].arrival);
        printf("Enter total cycles for process %d: ", i + 1);
        scanf("%d", &processes[i].total_cpu);
        
        // Initialize other fields
        processes[i].id = i + 1;
        processes[i].total_remaining = processes[i].total_cpu;
        processes[i].done = 0;
        processes[i].start_time = 0;
        processes[i].already_started = 0;
        processes[i].end_time = 0;
        processes[i].turnaround_time = 0;
    }
}

void scheduleFIFO(struct Process *processes, int num_processes) {
    int current_time = 0;

    // Sort processes based on arrival time
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].arrival > processes[j + 1].arrival) {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    // Execute processes in FIFO order
    for (int i = 0; i < num_processes; i++) {
        if (current_time < processes[i].arrival) {
            current_time = processes[i].arrival;
        }

        processes[i].start_time = current_time;
        processes[i].end_time = current_time + processes[i].total_cpu;
        processes[i].turnaround_time = processes[i].end_time - processes[i].arrival;

        current_time = processes[i].end_time;
    }
}

void scheduleSJF(struct Process *processes, int num_processes) {
    int current_time = 0;
    int completed_processes = 0;
    int shortest_job_index;

    while (completed_processes < num_processes) {
        int shortest_job_time = 99999;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].total_cpu < shortest_job_time && !processes[i].done && processes[i].arrival <= current_time) {
                shortest_job_index = i;
                shortest_job_time = processes[i].total_cpu;
            }
        }

        processes[shortest_job_index].start_time = current_time;
        processes[shortest_job_index].end_time = current_time + processes[shortest_job_index].total_cpu;
        processes[shortest_job_index].turnaround_time = processes[shortest_job_index].end_time - processes[shortest_job_index].arrival;
        processes[shortest_job_index].done = 1;
        current_time = processes[shortest_job_index].end_time;
        
        completed_processes++;
    }
}

void scheduleSRT(struct Process *processes, int num_processes) {
    int current_time = 0;
    int completed_processes = 0;

    while (completed_processes < num_processes) {
        int shortest_remaining_index = -1;
        int shortest_remaining_time = 99999;
        int flag = 0;

        // Find the process with the shortest remaining time
        for (int i = 0; i < num_processes; i++) {
            if (!processes[i].done && processes[i].arrival <= current_time) {
                if (processes[i].total_remaining < shortest_remaining_time) {
                    shortest_remaining_time = processes[i].total_remaining;
                    shortest_remaining_index = i;
                    flag = 1;
                }
            }
        }

        // If no process found for execution
        if (!flag) {
            int min_arrival = 99999;
            for (int i = 0; i < num_processes; i++) {
                if (!processes[i].done && processes[i].arrival < min_arrival) {
                    min_arrival = processes[i].arrival;
                }
            }
            current_time = min_arrival;
            continue;
        }

        // Update start time if the process hasn't already started
        if (!processes[shortest_remaining_index].already_started) {
            processes[shortest_remaining_index].start_time = current_time;
            processes[shortest_remaining_index].already_started = 1;
        }

        
        processes[shortest_remaining_index].total_remaining--;
        current_time++;

        // Check if the process is completed
        if (processes[shortest_remaining_index].total_remaining == 0) {
            processes[shortest_remaining_index].end_time = current_time;
            processes[shortest_remaining_index].turnaround_time = processes[shortest_remaining_index].end_time - processes[shortest_remaining_index].arrival;
            processes[shortest_remaining_index].done = 1;
            completed_processes++;
        }
    }


}

void printTable(struct Process *processes, int num_processes) {
    printf("\nPID\tArrival\tTotal\tStart\tEnd\tTurnaround\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", processes[i].id, processes[i].arrival,
               processes[i].total_cpu, processes[i].start_time, processes[i].end_time,
               processes[i].turnaround_time);
    }
    printf("\n\n");
}

int main() {
    int choice;
    int num_processes;
    struct Process *processes;
    
    while(1) {
        // Display menu
        printf("Batch scheduling\n----------------\n");
        printf("1) Enter parameters\n");
        printf("2) Schedule processes with FIFO algorithm\n");
        printf("3) Schedule processes with SJF algorithm\n");
        printf("4) Schedule processes with SRT algorithm\n");
        printf("5) Quit and free memory\n\n");

        // Input choice
        printf("Enter selection: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                // Input parameters
                printf("Enter total number of processes: ");
                scanf("%d", &num_processes);
                processes = (struct Process*)malloc(num_processes * sizeof(struct Process));
                enterParameters(processes, num_processes);
                printTable(processes, num_processes);
                break;
            case 2:
                scheduleFIFO(processes, num_processes);
                printTable(processes, num_processes);
                break;
            case 3:
                scheduleSJF(processes, num_processes);
                printTable(processes, num_processes);
                break;
            case 4:
                scheduleSRT(processes, num_processes);
                printTable(processes, num_processes);
                break;
            case 5:
                // Free allocated memory and quit
                free(processes);
                exit(0);
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}