#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#define MEMORY_SIZE 60
#define MAX_WORD_LENGTH 100

#define MAX_PCBS 10

int lower_bound=0;
int program_index = 1;

typedef struct {
    int pid; // Process ID
    char state[10]; // Process state
    int priority; // Current priority
    int program_counter; // Program counter
    int lower_bound; // Lower bound of process's memory space
    int upper_bound; // Upper bound of process's memory space
} PCB;

char memory[MEMORY_SIZE][MAX_WORD_LENGTH]; // Corrected declaration

PCB pcbs[MAX_PCBS];



void read_program(const char *filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
       
    }
int lb=lower_bound;
    char buffer[MAX_WORD_LENGTH];  // Buffer to hold each line
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Copy the line into the global array
        strcpy(memory[lower_bound], buffer); // Corrected: Copy to memory[line_count]
        lower_bound++;
        if (lower_bound >= MEMORY_SIZE) {
            printf("Reached maximum number of lines\n");
            break;
        }
    }
    strcpy(memory[lower_bound], "a: ");
    lower_bound++;
    strcpy(memory[lower_bound], "b: ");
    lower_bound++;
    strcpy(memory[lower_bound], "c: ");
    lower_bound++;

    PCB pcb;
    pcb.pid = program_index; 
    sprintf(memory[lower_bound], "PID: %d", pcb.pid);
    lower_bound++;
    program_index++;

    strcpy(pcb.state, "Ready");
    sprintf(memory[lower_bound], "State: %s", pcb.state);
    lower_bound++;

    pcb.priority =1 ;
    sprintf(memory[lower_bound], "Priority: %d", pcb.priority);
    lower_bound++;

    pcb.program_counter = 0;
    strcpy(memory[lower_bound], "PC: 0");
    lower_bound++;

    pcb.lower_bound = lb;
    sprintf(memory[lower_bound], "Lower bound: %d", pcb.lower_bound);
    lower_bound++;

    pcb.upper_bound = lower_bound-9;
    sprintf(memory[lower_bound], "Upper bound: %d", pcb.upper_bound);
    lower_bound++;
   

    pcbs[pcb.pid - 1] = pcb;


    fclose(file);
}

////////////////////////////////// Instructions //////////////////////////////////////////////////

void assign(char variable, charvalue) {
    if (strcmp(value, "input") == 0) {
        printf("Please enter a value for %s: ", variable);
        scanf("%s", value);
    }
    setenv(variable, value, 1);
    if (strcmp(variable, "a") == 0) {
        strcpy(memory[p][11], value);
    } else if (strcmp(variable, "b") == 0) {
        strcpy(memory[p][12], value);
    } else if (strcmp(variable, "c") == 0) {
        strcpy(memory[p][13], value);
    } else {
        // If the variable is not a, b, or c, you can handle it here
        printf("Unknown variable: %s\n", variable);
        return;
    }

    printf("Assigned '%s' to '%s'\n", value, variable);
    }



    void writeFile(char filename, chardata) {
    // Open the file in write mode
    FILE *file = NULL;

    if (strcmp(filename, "a") == 0) {
        file = fopen(memory[p][11], "w");
    } else if (strcmp(filename, "b") == 0) {
        file = fopen(memory[p][12], "w");
    } else if (strcmp(filename, "c") == 0) {
       file = fopen(memory[p][13], "w");
    }

    // Check if file opened successfully
    if (file == NULL) {
        printf("Error opening file %s: %s\n", filename, strerror(errno));
        return;
    }

    // Write data to the file
    if (strcmp(data, "a") == 0) {
        data = memory[p][11];
    } else if (strcmp(data, "b") == 0) {
        data = memory[p][12];
    } else if (strcmp(data, "c") == 0) {
       data = memory[p][13];
    }


    int bytes_written = fprintf(file, "%s", data);
    if (bytes_written < 0) {
        printf("Error writing data to file %s: %s\n", filename, strerror(errno));
        fclose(file);
        return;
    }

    // Close the file
    fclose(file);

    printf("Data written to %s successfully.\n", filename);
}
char* readFile(char filename) {
    // Open the file in read mode
    FILEfile = fopen(filename, "r");

    // Check if file opened successfully
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for file contents
    char file_contents = (char)malloc(file_size + 1);
    if (file_contents == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read data from the file into the buffer
    size_t bytes_read = fread(file_contents, 1, file_size, file);
    if (bytes_read != file_size) {
        printf("Error reading file\n");
        fclose(file);
        free(file_contents);
        return NULL;
    }

    // Null-terminate the file contents to make it a valid C string
    file_contents[file_size] = '\0';

    // Close the file
    fclose(file);

    // Return the file contents
    printf("File contents: %s\n", file_contents);
    return file_contents;
}
void printFromTo(int x, int y) {
    int start, end;

    // Convert 'x' and 'y' to their corresponding values in memory
    if (x == 'a') {
        start = atoi(memory[p][11]);
    } else if (x == 'b') {
        start = atoi(memory[p][12]);
    } else if (x == 'c') {
        start = atoi(memory[p][13]);
    } else {
        start = x; // Use 'x' as it is
    }

    if (y == 'a') {
        end = atoi(memory[p][11]);
    } else if (y == 'b') {
        end = atoi(memory[p][12]);
    } else if (y == 'c') {
        end = atoi(memory[p][13]);
    } else {
        end = y; // Use 'y' as it is
    }

    // Determine the starting and ending numbers
    int min = (start <= end) ? start : end;
    int max = (start <= end) ? end : start;

    // Print numbers between start and end
    printf("Numbers between %d and %d:\n", min, max);
    for (int i = min; i <= max; i++) {
        printf("%d ", i);
    }
    printf("\n");
}



void execute_instruction(const char *instruction) {
    char command[MAX_LINE_LENGTH], arg1[MAX_LINE_LENGTH], arg2[MAX_LINE_LENGTH], arg3[MAX_LINE_LENGTH];

    // Parse the instruction
     sscanf(instruction, "%s %s %s %s", command, arg1, arg2, arg3);

    // Determine the action based on the command
    if (strcmp(command, "assign") == 0) {
        if(strcmp(arg2, "readFile") == 0){
            assign(arg1, readFile(memory[p][11]));

        }
        else{
            if(strcmp(arg1, "readFile") == 0){
                assign(readFile(arg2), arg3);

            }
            else{
                assign(arg1,arg2);
            }
        }

    } else if (strcmp(command, "writeFile") == 0) {
        writeFile(arg1, arg2);
    } else if (strcmp(command, "readFile") == 0) {
        readFile(arg1);
    } else if (strcmp(command, "printFromTo") == 0) {
        printFromTo(arg1[0], arg2[0]);
    } else if (strcmp(command, "semWait") == 0) {
        if(strcmp(arg1, "file") == 0) {
    semWait(&files);
} else if(strcmp(arg1, "userInput") == 0) {
    semWait(&userInput);
} else if(strcmp(arg1, "userOutput") == 0) {
    semWait(&userOutput);
}
        // int mutexIndex = atoi(arg1); // Assuming arg1 contains the index of the mutex
        // semWait(&mutexes[mutexIndex]);
    } else if (strcmp(command, "semSignal") == 0) {
        if(strcmp(arg1, "file") == 0) {
    semSignal(&files);
} else if(strcmp(arg1, "userInput") == 0) {
    semSignal(&userInput);
} else if(strcmp(arg1, "userOutput") == 0) {
    semSignal(&userOutput);
}

        // int mutexIndex = atoi(arg1); // Assuming arg1 contains the index of the mutex
        // semSignal(&mutexes[mutexIndex]);
    }
    else if(strcmp(command, "print") == 0) {
    print(arg1);
}
 else {
        printf("Unknown command: %s\n", command);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void print_memory() {
    printf("Memory Contents:\n");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("\n word:");
        printf("%s", memory[i]);
        printf("\n");
    }
}

int main() {
    const char *filenames[3] = {"Program_1.txt", "Program_2.txt", "Program_3.txt"};

    for (int i = 0; i < 3; i++) {
        read_program(filenames[i]);
        
       
    }
print_memory();
    

    return 0;
}