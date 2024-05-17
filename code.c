#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#define MEMORY_SIZE 60
#define MAX_WORD_LENGTH 100

#define MAX_SIZE 10

bool blocked;

int lower_bound=0;
int program_index = 1;


typedef struct {
    char name[30];
    char value[MAX_WORD_LENGTH];
} CELL;

typedef struct {
    int pid; // Process ID
    char state[10]; // Process state
    int priority; // Current priority
    int program_counter; // Program counter
    int lower_bound; // Lower bound of process's memory space
    int upper_bound; // Upper bound of process's memory space
} PCB;

PCB currPCB;

typedef struct {
    PCB items[MAX_SIZE];
    int front;
    int rear;
} Queue;

// Function to initialize the queue
void initializeQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

// Function to check if the queue is empty
int isEmpty(Queue *q) {
    return (q->rear == -1);
}

// Function to check if the queue is full
int isFull(Queue *q) {
    return (q->rear == MAX_SIZE - 1);
}

// Function to add an element to the queue
void enqueue(Queue *q, PCB value) {
    if (isFull(q)) {
        printf("Queue Overflow\n");
        return;
    }
    if (isEmpty(q))
        q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}

// Function to remove an element from the queue
PCB dequeue(Queue *q) {
    PCB item;
    if (isEmpty(q)) {
        printf("Queue Underflow\n");
        exit(1);
    }
    item = q->items[q->front];
    if (q->front >= q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }
    return item;


}


int queueLength(Queue *q) {
    if (isEmpty(q)) {
        return 0;
    } else {
        return q->rear - q->front + 1;
    }
}

// Function to display the elements of the queue
void display(Queue *q) {
    int i;
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue elements are:\n");
    for (i = q->front; i <= q->rear; i++) {
        printf("PID: %d, State: %s, Priority: %d, PC: %d, Lower Bound: %d, Upper Bound: %d\n",
               q->items[i].pid, q->items[i].state, q->items[i].priority,
               q->items[i].program_counter, q->items[i].lower_bound, q->items[i].upper_bound);
    }
}


PCB peek(Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        exit(1);
    }
    return q->items[q->front];
}


CELL memory[MEMORY_SIZE]; // Corrected declaration



PCB pcbs[3];    

int clk=0;
int p1Arrival;
int p2Arrival;
int p3Arrival;



Queue q1, q2, q3, q4, blockedQueue;

typedef struct Mutex {
    enum { zero, one } value;
    int ownerID;
    Queue q;
} Mutex;
Mutex file;
Mutex userInput;
Mutex userOutput;

void read_program(const char *filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
       
    }
int lb=lower_bound;
CELL temp;
int insCount=1;
    char buffer[MAX_WORD_LENGTH];  // Buffer to hold each line
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        sprintf(temp.name,  "instruction %d :", insCount);
        insCount++;
        strcpy(temp.value, buffer); // Corrected: Copy to memory[line_count]
        memory[lower_bound] = temp;
        lower_bound++;
        if (lower_bound >= MEMORY_SIZE) {
            printf("Reached maximum number of lines\n");
            break;
        }
    }
    temp.value[0] = '\0';
    strcpy(temp.name, "a: ");
    memory[lower_bound] = temp;
    lower_bound++;

    strcpy(temp.name, "b: ");
    memory[lower_bound] = temp;
    lower_bound++;

     strcpy(temp.name, "c: ");
    memory[lower_bound] = temp;
    lower_bound++;
    

    PCB pcb;
    pcb.pid = program_index; 
    program_index++;
    pcb.priority =1 ;
    pcb.program_counter = 0;
    pcb.lower_bound = lb;

    strcpy(temp.name, "PID: ");
    sprintf(temp.value,  "%d", pcb.pid);
    memory[lower_bound] = temp;
    lower_bound++;

    strcpy(temp.name, "State: ");
    strcpy(temp.value, "Ready");
    memory[lower_bound] = temp;
    lower_bound++;

    strcpy(temp.name, "Priority: ");
    sprintf(temp.value,  "%d", pcb.priority);
    memory[lower_bound] = temp;
    lower_bound++;

    strcpy(temp.name, "Program Counter: ");
    sprintf(temp.value,  "%d", pcb.program_counter);
    memory[lower_bound] = temp;
    lower_bound++;

    strcpy(temp.name, "Lower Bound: ");
    sprintf(temp.value,  "%d", pcb.lower_bound);
    memory[lower_bound] = temp;
    lower_bound++;

pcb.upper_bound = lower_bound-9;
    strcpy(temp.name, "Upper Bound: ");
    sprintf(temp.value,  "%d", pcb.upper_bound);
    memory[lower_bound] = temp;
    lower_bound++;

    pcbs[program_index-2] = pcb;

    fclose(file);
}

////////////////////////////////// Instructions //////////////////////////////////////////////////
void assign(char variable, char *value) {
    int address;


    switch(variable){
        case 'a': address=currPCB.upper_bound+1;break;
        case 'b': address=currPCB.upper_bound+2;break;
        case 'c': address=currPCB.upper_bound+3;break;
        default:printf("Unknown variable: %d\n", variable);return;
    }
    if (strcmp(value, "input") == 0) {
        char *arrival_time;

        printf("Enter value for variable '%c':\n ", variable);
         scanf("%s", arrival_time);
       strcpy(memory[address].value, arrival_time);
    } else {
        // Otherwise, copy the provided value to the memory
        strcpy(memory[address].value, value);
    }

    printf("Assigned '%s' to '%c'\n", memory[address].value, variable);
}

void printFromTo(char x, char y) {
    int start, end;

    // Convert 'x' and 'y' to their corresponding values in memory
    if (x == 'a') {
        start = atoi(memory[currPCB.upper_bound+1].value);
    } else if (x == 'b') {
        start = atoi(memory[currPCB.upper_bound+2].value);
    } else if (x == 'c') {
        start = atoi(memory[currPCB.upper_bound+3].value);
    } 

    if (y == 'a') {
        end= atoi(memory[currPCB.upper_bound+1].value);
    } else if (y == 'b') {
        end = atoi(memory[currPCB.upper_bound+2].value);
    } else if (y == 'c') {
        end = atoi(memory[currPCB.upper_bound+3].value);
    } 

    printf("Numbers between %d and %d:\n", start, end);
    for (int i = start; i <=end; i++) {
        printf("%d ", i);
    }
    printf("\n");
}

  void writeFile(char filename, char data) {
    FILE *file = NULL;
    char *name;
    if (filename == 'a') {
        name = memory[currPCB.upper_bound+1].value;
    } else if (filename == 'b') {
        name = memory[currPCB.upper_bound+2].value;
    } else if (filename == 'c') {
        name = memory[currPCB.upper_bound+3].value;
    }
    file = fopen(name, "w");
    if (file == NULL) {
        printf("Error opening file %s: %s\n", name, strerror(errno));
        return;
    }

    // Write data to the file

    char *dataToWrite;
    if (data == 'a') {
        dataToWrite=memory[currPCB.upper_bound+1].value;
    } else if (data == 'b') {
        dataToWrite=memory[currPCB.upper_bound+2].value;
    } else if (data == 'c') {
        dataToWrite=memory[currPCB.upper_bound+3].value;
    }


    int bytes_written = fprintf(file, "%s", dataToWrite);
    if (bytes_written < 0) {
        printf("Error writing data to file %s: %s\n", name, strerror(errno));
        fclose(file);
        return;
    }

    // Close the file
    fclose(file);
    printf("Data written to %s successfully.\n", name);
}


char* readFile(char filename) {
    // Open the file in read mode
    
    char *name;
    if (filename == 'a') {
        name = memory[currPCB.upper_bound+1].value;
    } else if (filename == 'b') {
        name = memory[currPCB.upper_bound+2].value;
    } else if (filename == 'c') {
        name = memory[currPCB.upper_bound+3].value;
    }
    FILE *file = fopen(name, "r");

    // Check if file opened successfully
    if (file == NULL) {
        printf("Error opening file %s\n", name);
        return NULL;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for file contents
    char *file_contents = malloc(file_size + 1);
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


void semWait(Mutex *mutex) {
    if (mutex->value == one) {
        
        printf("Process %d blocked waiting for resource\n", currPCB.pid);
        enqueue(&(mutex->q), currPCB);
        blocked = true;
    } else {
        // Resource is available, acquire it
        mutex->value = one;
        mutex->ownerID = currPCB.pid; // Assuming currPCB is the current process control block
    }
}

void semSignal(Mutex *mutex) {
    if (mutex->value == one) {
        // Release the resource
        mutex->value = zero;
        mutex->ownerID = -1; // No owner
        if(!isEmpty(&(mutex->q))){
            
        
        PCB pcb=dequeue(&(mutex->q));
        semWait(mutex);
        printf("Process %d free to use resource\n", pcb.pid);
        if(pcb.priority==1){
            enqueue(&q1,pcb);
        }
        else if(pcb.priority==2){
            enqueue(&q2,pcb);
        }
        else if(pcb.priority==3){
            enqueue(&q3,pcb);
        }
        else{
            enqueue(&q4,pcb);
        }
        }


    } else {
        // Resource was not in use, error condition
        printf("Error: Attempting to release a non-acquired resource\n");
    }
}

void print(char output) {
      int address;
    switch(output){
        case 'a': address=currPCB.upper_bound+1;break;
        case 'b': address=currPCB.upper_bound+2;break;
        case 'c': address=currPCB.upper_bound+3;break;
        default:printf("Unknown variable: %d\n", output);return;


    }

    printf("%s", memory[address].value);
}
void execute_instruction(const char *instruction) {
     char command[MAX_WORD_LENGTH], arg1[MAX_WORD_LENGTH], arg2[MAX_WORD_LENGTH], arg3[MAX_WORD_LENGTH];

     

    // Parse the instruction
    sscanf(instruction, "%s %s %s %s", command, arg1, arg2, arg3);

    printf("Process %d executing \n", currPCB.pid);
    printf("Executing instruction: %s\n", instruction);

   // Determine the action based on the command
    if (strcmp(command, "semWait") == 0) {
 currPCB.program_counter++;
     sprintf(memory[currPCB.upper_bound + 7].value, "%d", currPCB.program_counter);
        if (strcmp(arg1, "userInput") == 0) {
            semWait(&userInput);
        } else if (strcmp(arg1, "file") == 0) {
            semWait(&file);
        } else if (strcmp(arg1, "userOutput") == 0) {
            semWait(&userOutput);
        } else {
            printf("Unknown semaphore: %s\n", arg1);
        }
    
    } 
    else if (strcmp(command, "assign") == 0) {
        if (strcmp(arg2, "input") == 0) {
            assign(arg1[0], arg2); // Assuming arg1 contains a single character representing the variable
        } else if (strcmp(arg2, "readFile") == 0) {
            char *fileContents = readFile(arg3[0]); // Assuming arg3 contains a single character representing the filename variable
            if (fileContents != NULL) {
                assign(arg1[0], fileContents);
                free(fileContents); // Free allocated memory
            } else {
                printf("Error reading file: %s\n", arg3);
            }
        } else {
            assign(arg1[0], arg2);
        }
       currPCB.program_counter++;
       sprintf(memory[currPCB.upper_bound + 7].value, "%d", currPCB.program_counter);
    } else if (strcmp(command, "writeFile") == 0) {
        writeFile(arg1[0], arg2[0]);currPCB.program_counter++;// Assuming arg1 and arg2 contain single characters representing the filename and data variable, respectively
    } else if (strcmp(command, "semSignal") == 0) {
        if (strcmp(arg1, "userInput") == 0) {
            semSignal(&userInput);
        } else if (strcmp(arg1, "file") == 0) {
            semSignal(&file);
        } else if (strcmp(arg1, "userOutput") == 0) {
            semSignal(&userOutput);
        } else {
            printf("Unknown semaphore: %s\n", arg1);
        }
       currPCB.program_counter++;
       sprintf(memory[currPCB.upper_bound + 7].value, "%d", currPCB.program_counter);
    } else if (strcmp(command, "print") == 0) {
       print(arg1[0]);currPCB.program_counter++;
       sprintf(memory[currPCB.upper_bound + 7].value, "%d", currPCB.program_counter);
    } else if (strcmp(command, "printFromTo") == 0) {
        printFromTo(arg1[0], arg2[0]);
        currPCB.program_counter++;
     sprintf(memory[currPCB.upper_bound + 7].value, "%d", currPCB.program_counter);
    } else {
        printf("Unknown command: %s\n", command);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void print_memory() {
    printf("Memory Contents:\n");
    for (int i = 0; i < MEMORY_SIZE; i++) {
        printf("%s", memory[i].name);
        printf("%s", memory[i].value);
        printf("\n");
    }
}




   void  printCycle(){
  printf("----------------------Clock:%d ------------------------------\n", clk);
        printf("--------------------------Q1---------------------------------------\n");
        display(&q1);
        printf("---------------------------Q2--------------------------------------\n");
        display(&q2);
        printf("----------------------------Q3-------------------------------------\n");
        display(&q3);
        printf("-----------------------------Q4------------------------------------\n");
        display(&q4);
        printf("-------------------------------Q file----------------------------------\n");
        display(&(file.q));
        printf("--------------------------------Q Input-------------------------------\n");
        display(&(userInput.q));
        printf("------------------------------Q Output-----------------------------------\n");
        display(&(userOutput.q));
        printf("---------------------------------------------------------------------------\n");
        printf(" current pid %d pc %d priority %d\n", currPCB.pid, currPCB.program_counter, currPCB.priority);

        print_memory();

   }

int main() {
    initializeQueue(&q1);
    initializeQueue(&q2);
    initializeQueue(&q3);
    initializeQueue(&q4);
    initializeQueue(&(file.q));
    initializeQueue(&(userInput.q));
    initializeQueue(&(userOutput.q));

    const char *filenames[3] = {"Program_1.txt", "Program_2.txt", "Program_3.txt"};

    for (int i = 0; i < 3; i++) {
        read_program(filenames[i]);
    }

    for (int i = 0; i < 3; i++) {
        int arrival_time;
        printf("Enter arrival time for Process %d: ", i + 1);
        scanf("%d", &arrival_time);

        // Store arrival time in global variables
        switch (i) {
            case 0:
                p1Arrival = arrival_time;
                break;
            case 1:
                p2Arrival = arrival_time;
                break;
            case 2:
                p3Arrival = arrival_time;
                break;
        }
    }

        bool haveProcess = false;
        int quantum = 0;


        while(clk<40){

            if(p1Arrival==clk){
                enqueue(&q1, pcbs[0]);
            }
            if(p2Arrival==clk){
                enqueue(&q1, pcbs[1]);
            }
            if(p3Arrival==clk){
                enqueue(&q1, pcbs[2]);
            }

            
     
            if(haveProcess&&quantum!=0){
                printCycle();
                execute_instruction(memory[currPCB.lower_bound +currPCB.program_counter].value);
                quantum--;
                if(quantum==0 &&(currPCB.upper_bound-currPCB.lower_bound)+1!=(currPCB.program_counter)){
                    
                    haveProcess=false;
                        switch(currPCB.priority){
                            case 1:  currPCB.priority++;sprintf(memory[currPCB.upper_bound + 6].value, "%d", currPCB.priority);enqueue(&q2,currPCB);break;
                            case 2:  currPCB.priority++;sprintf(memory[currPCB.upper_bound + 6].value, "%d", currPCB.priority);enqueue(&q3,currPCB);break;
                            case 3:  currPCB.priority++;sprintf(memory[currPCB.upper_bound + 6].value, "%d", currPCB.priority); enqueue(&q4,currPCB);break;
                            default:  currPCB.priority++;sprintf(memory[currPCB.upper_bound + 6].value, "%d", currPCB.priority); enqueue(&q4,currPCB);break;
                        }
                }

                
                 clk++;
                if(blocked){
                    haveProcess=false;
                    
                }
                 if( currPCB.upper_bound-currPCB.lower_bound+1==currPCB.program_counter){
                     haveProcess=false;
                 }
            }
            else{

            if(!isEmpty(&q1)){
                printCycle();
                currPCB=dequeue(&q1);
                blocked=false;
                haveProcess=false;
                quantum=0;
                currPCB.priority++;sprintf(memory[currPCB.upper_bound + 6].value, "%d", currPCB.priority);
                 execute_instruction(memory[currPCB.lower_bound +currPCB.program_counter].value);
                    
                    if(!blocked ){
                enqueue(&q2,currPCB);}
                clk++;
                

                
            }
           else if(!isEmpty(&q2)){
                currPCB=dequeue(&q2);
                blocked=false;
                haveProcess=true;
                quantum=2;
                

            }
           else if(!isEmpty(&q3)){

                currPCB=dequeue(&q3);
                blocked=false;
                haveProcess=true;
                quantum=4;

            }
           else if(!isEmpty(&q4)){

                currPCB=dequeue(&q4);
                blocked=false;
                haveProcess=true;
                quantum=8;
            }
            }
           
        }
    
    return 0;


}