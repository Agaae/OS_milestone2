#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 60
#define MAX_LINES 60
#define MAX_LINE_LENGTH 100
#define NUM_PROGRAMS 3

typedef struct {
    int pid; // Process ID
    char state[10]; // Process state
    int priority; // Current priority
    int program_counter; // Program counter
    int lower_bound; // Lower bound of process's memory space
    int upper_bound; // Upper bound of process's memory space
} PCB;

char memory[NUM_PROGRAMS][MAX_LINES][MAX_LINE_LENGTH]; // Add an extra line for PCB
int memory_size[NUM_PROGRAMS] = {0};

void read_program(const char *filename, int program_index) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (memory_size[program_index] >= MAX_LINES) {
            printf("Error: Memory full for program %d\n", program_index);
            break;
        }
        line[strcspn(line, "\n")] = '\0';
        // Copy the entire line to memory
        strcpy(memory[program_index][memory_size[program_index]], line);
        memory_size[program_index]++;
    }

    // Save PCB after program instructions
    PCB pcb;
    pcb.pid = program_index + 1; // Assuming process ID starts from 1
    strcpy(pcb.state, "Ready");
    pcb.priority = 1;
    pcb.program_counter = 0;
    pcb.lower_bound = 0; // Lower bound starts from 0 for simplicity
    pcb.upper_bound = memory_size[program_index] - 1; // Upper bound is the last index of program instructions
    // Convert PCB to string format and save it in memory
    sprintf(memory[program_index][memory_size[program_index]], "PID: %d, State: %s, Priority: %d, PC: %d, Lower Bound: %d, Upper Bound: %d",
            pcb.pid, pcb.state, pcb.priority, pcb.program_counter, pcb.lower_bound, pcb.upper_bound);
    memory_size[program_index]++;
    
    fclose(file);
}

int main() {
    const char *filenames[NUM_PROGRAMS] = {"Program_1.txt", "Program_2.txt", "Program_3.txt"};

    // Read each program file and save program instructions and PCB in memory
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        read_program(filenames[i], i);
    }

    // Print memory contents for each program
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        printf("Contents of program %d's memory:\n", i + 1);
        for (int j = 0; j < memory_size[i]; j++) {
            printf("%s\n", memory[i][j]);
        }
        printf("\n");
    }
 

    return 0;
}
