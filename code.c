#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>
#define MEMORY_SIZE 60
#define MAX_LINES 60
#define MAX_LINE_LENGTH 100
#define NUM_PROGRAMS 3

int p;
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Define the queue structure
typedef struct {
    Node* front;
    Node* rear;
} Queue;


typedef struct Mutex {
    enum { zero, one } value;
    int ownerID;
} Mutex;


// Function to initialize a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to initialize an empty queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    queue->front = queue->rear = NULL;
    return queue;
}

// Function to check if a queue is empty
bool isEmpty(Queue* queue) {
    return (queue->front == NULL);
}

// Function to enqueue an element into the queue
void enqueue(Queue* queue, int data) {
    Node* newNode = createNode(data);
    if (isEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Function to dequeue an element from the queue
int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    Node* temp = queue->front;
    int data = temp->data;
    queue->front = temp->next;
    free(temp);
    return data;
}

// Function to display the contents of the queue
void displayQueue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue: ");
    Node* current = queue->front;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Function to free memory allocated for the queue
void freeQueue(Queue* queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

int front(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    return queue->front->data;
}


Queue* queue1 ;
Queue* queue2 ;
Queue* queue3 ;
Queue* queue4 ;
Queue* blockedQueue;


Mutex file;
Mutex userInput;
Mutex userOutput;





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
    if (memory_size[program_index] + 3 < MAX_LINES) {
        sprintf(memory[program_index][11], "a"); // Initialize 'a' with 0
        memory_size[program_index]++;
        sprintf(memory[program_index][12], "b"); // Initialize 'b' with 0
        memory_size[program_index]++;
        sprintf(memory[program_index][13], "c"); // Initialize 'c' with 0
        memory_size[program_index]++;
    } else {
        printf("Error: Not enough space to add variables\n");
    }

    // Save PCB after program instructions
    PCB pcb;
    pcb.pid = program_index + 1; // Assuming process ID starts from 1
    strcpy(pcb.state, "Ready");
    pcb.priority = program_index + 1 ;
    pcb.program_counter = 0;
    pcb.lower_bound = 0; // Lower bound starts from 0 for simplicity
    pcb.upper_bound = memory_size[program_index] - 1; // Upper bound is the last index of program instructions
    // Convert PCB to string format and save it in memory
     sprintf(memory[program_index][16], "PID: %d", pcb.pid);
     sprintf(memory[program_index][17], "STATE: %s", pcb.state);
     sprintf(memory[program_index][18], "PRIORITY: %d", pcb.priority);
     sprintf(memory[program_index][19], "Program Counter: %d", pcb.program_counter);
     sprintf(memory[program_index][20], "Lower Bound: %d", pcb.lower_bound);
     sprintf(memory[program_index][21], "Upper bound: %d", pcb.upper_bound); 
    memory_size[program_index]++;
    
    fclose(file);
}

///////////////////////////////////////////////////////////////////

void print(char *output) {
    
            printf("%s\n", memory[p][12]);

        
}




////////////////////////////////////////////////////////////////////





















////////////////////////////////////////////////////////////////


void semWait(Mutex* m) {
    if (m->value == one) {
        m->ownerID = '1';
        m->value = zero;
    } else {
        int queueNo = atoi(memory[p][18]);
        enqueue(blockedQueue, p+1);
    }
}

void semSignal(Mutex* m) {
    if (m->ownerID == '1') {
        int queueNo = atoi(memory[p][18]);
        if (isEmpty(blockedQueue)) {
            m->value = one;
        } else {
            m->ownerID = front(blockedQueue);
            switch (queueNo) {
        case 1:
            enqueue(queue1, p+1 );
            break;
        case 2:
            enqueue(queue2, p+1 );
            break;
        case 3:
            enqueue(queue3, p+1 );
            break;
        case 4:
            enqueue(queue4, p+1 );
            break;
       
    }
            
        }
    }
}






////////////////////////////////////////////////////////////////

void assign(char *variable, char *value) {
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





///////////////////////////////////////////////////////////////////////////////


void writeFile(char *filename, char *data) {
    // Open the file in write mode
    FILE *file = fopen(filename, "w");

    // Check if file opened successfully
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    // Write data to the file
    int bytes_written = fprintf(file, "%s", data);
    if (bytes_written < 0) {
        printf("Error writing data to file %s\n", filename);
        fclose(file);
        return;
    }

    // Close the file
    fclose(file);

    printf("Data written to %s successfully.\n", filename);
}




//////////////////////////////////////////////////////////////////////////////



char* readFile(char *filename) {
    // Open the file in read mode
    FILE *file = fopen(filename, "r");

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
    char *file_contents = (char*)malloc(file_size + 1);
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



//////////////////////////////////////////////////////////////

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





/////////////////////////////////////////////////////////////////////////////////////

sem_t file_sem, userInput_sem, userOutput_sem;






// // Function to acquire a resource
// void semWait(char *resource) {
//     if (strcmp(resource, "file") == 0) {
//         sem_wait(&file_sem);
//         printf("File access acquired.\n");
//     } else if (strcmp(resource, "userInput") == 0) {
//         sem_wait(&userInput_sem);
//         printf("User input acquired.\n");
//     } else if (strcmp(resource, "userOutput") == 0) {
//         sem_wait(&userOutput_sem);
//         printf("Screen output acquired.\n");
//     } else {
//         printf("Invalid resource name.\n");
//     }
// }

// // Function to release a resource
// void semSignal(char *resource) {
//     if (strcmp(resource, "file") == 0) {
//         sem_post(&file_sem);
//         printf("File access released.\n");
//     } else if (strcmp(resource, "userInput") == 0) {
//         sem_post(&userInput_sem);
//         printf("User input released.\n");
//     } else if (strcmp(resource, "userOutput") == 0) {
//         sem_post(&userOutput_sem);
//         printf("Screen output released.\n");
//     } else {
//         printf("Invalid resource name.\n");
//     }
// }

///////////////////////////////////////////////////////////////


// Execute the instruction based on its contents
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
    semWait(&file);
} else if(strcmp(arg1, "userInput") == 0) {
    semWait(&userInput);
} else if(strcmp(arg1, "userOutput") == 0) {
    semWait(&userOutput);
}
        // int mutexIndex = atoi(arg1); // Assuming arg1 contains the index of the mutex
        // semWait(&mutexes[mutexIndex]);
    } else if (strcmp(command, "semSignal") == 0) {
        if(strcmp(arg1, "file") == 0) {
    semSignal(&file);
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


/////////////////////////////////////////////////////////////////////////



void *computation0(){
    for (int i = 0; i <= 8; i++) {
    
    p=0;
     execute_instruction(memory[0][i]);
     
    
    }
}
void *computation1(){
    
    for (int i = 0; i <= 8; i++) {
        p=1;
    
     execute_instruction(memory[1][i]);
     
    }
}
void *computation2(){
    for (int i = 0; i <= 8; i++) {
     p=2;
     execute_instruction(memory[2][i]);
     
    }
}






///////////////////////////////////////////////////////////////






void print_memory(int program_index) {
    printf("Memory Contents for Program %d:\n", program_index);
    for (int i = 0; i < 22; i++) {
        printf("%s\n", memory[program_index][i]);
    }
}


//////////////////////////////////////////////////

int main() {
    const char *filenames[NUM_PROGRAMS] = {"Program_1.txt", "Program_2.txt", "Program_3.txt"};


  for (int i = 0; i < NUM_PROGRAMS; i++) {
        read_program(filenames[i], i);
    }




// for (int i = 0; i < NUM_PROGRAMS; i++) {
//         print_memory(i);
//     }

















    
    queue1 = createQueue();
    queue2 = createQueue();
    queue3 = createQueue();
    queue4 = createQueue();
    blockedQueue = createQueue();

    // Initialize mutexes
    file.value = one;
    userInput.value = one;
    userOutput.value = one;
    file.ownerID = -1;
    userInput.ownerID = -1;
    userOutput.ownerID = -1;

    
    
    

    //  for (int i = 0; i < NUM_PROGRAMS; i++) {
    //     read_program(filenames[i], i);
    // }


    // // for (int i = 1; i <= 8; i++) {
    // //     if (strcmp(front(queue1), "1") == 0 || (strcmp(front(queue2), "1") == 0 && isEmpty(queue1)) || (strcmp(front(queue3), "1") == 0 && isEmpty(queue1) && isEmpty(queue1)) || (strcmp(front(queue4), "1") == 0 && isEmpty(queue1) && isEmpty(queue2) && isEmpty(queue3)) && strcmp(front(blockedQueue), "1") != 0) {
    // //         execute_instruction(memory[0][i]);
    // //     }
    // //     if (strcmp(front(queue1), "2") == 0 || (strcmp(front(queue2), "2") == 0 && isEmpty(queue1)) || (strcmp(front(queue3), "2") == 0 && isEmpty(queue1) && isEmpty(queue1)) || (strcmp(front(queue4), "2") == 0 && isEmpty(queue1) && isEmpty(queue2) && isEmpty(queue3)) strcmp(front(blockedQueue), "2") != 0 ) {
    // //         execute_instruction(memory[1][i]);
    // //     } 
    // //     if (strcmp(front(queue1), "3") == 0 || (strcmp(front(queue2), "3") == 0 && isEmpty(queue1)) || (strcmp(front(queue3), "3") == 0 && isEmpty(queue1) && isEmpty(queue1)) || (strcmp(front(queue4), "3") == 0 && isEmpty(queue1) && isEmpty(queue2) && isEmpty(queue3)) strcmp(front(blockedQueue), "3") != 0) {
    // //         execute_instruction(memory[2][i]);
    // //     }

    // // }

    // for (int i = 1; i <= 8; i++) {
    
    //  execute_instruction(memory[1][i]);
    
    // }


   
     
pthread_t thread1;
// pthread_t thread2;
// pthread_t thread3;
pthread_create(&thread1,NULL, computation0, NULL);
// pthread_create(&thread2,NULL, computation1, NULL);
// pthread_create(&thread3,NULL, computation2, NULL);
pthread_join(thread1, NULL);
// pthread_join(thread2, NULL);
// pthread_join(thread3, NULL);

for (int i = 0; i <= 8; i++) {
    
     execute_instruction(memory[p][i]);
    
    }


        print_memory(p);
    

    return 0;
}

