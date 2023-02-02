#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_MACHINES 10
#define NUM_COMPANIES 5

// "Kevin", "Bob", "Stuart", "Otto", and "Dave" as 0, 1, 2, 3, and 4
char* companies[] = {"Kevin", "Bob", "Stuart", "Otto", "Dave"};
int num_customers = 0;
FILE* output_file;
// Data structure to represent a ticket vending machine
typedef struct {
    int company_id;  // ID of the company associated with the machine
    int balance;      // Amount of money currently in the machine
    pthread_mutex_t mutex;  // Mutex to synchronize access to the machine's data
    int machine_id;    // ID of the machine 
    int currentCustomer; // ID of the current customer
    int total_customer; // total number of customers
} Machine;

// Data structure to represent a customer
typedef struct {
  int company_id;   // Name of the company the customer is paying to
  int amount;  // Amount of money the customer is paying
  int sleep_time;     // Amount of time the customer will sleep
  int machine_id;     // ID of the machine the customer will use
  int customer_id;   // ID of the customer
} Customer;

// Data structure to represent a bank account 
typedef struct {
    char name[256];
    int balance;
    pthread_mutex_t mutex;
} BankAccount;

Machine machines[NUM_MACHINES];
BankAccount bank_accounts[NUM_COMPANIES];

// Function to simulate the actions of a customer
void* customer_thread_func(void* arg) {
    Customer* customer = (Customer*) arg;
    // Sleep for the customer's sleep time
    usleep(customer->sleep_time * 1000);
    // Acquire the mutex associated with the machine
    int machine_id = customer -> machine_id;
    Machine* machine = &machines[machine_id];
    pthread_mutex_lock(&machine->mutex);
    // Update the machine's balance and name
    machine->balance += customer->amount;
    machine->currentCustomer = customer->customer_id;
    machine->company_id = customer->company_id;

}
void* machine_thread_func(void* arg) {
    Machine* machine = (Machine*) arg;

    // Keep looping until all customers have been serviced
    while (machine->total_customer > 0) {
        
        // If the machine is empty, continue
        while (machine->company_id == -1) {
            continue;
        }
        // If the machine is not empty, find the company associated with the machine
        int company_id = machine->company_id;
        BankAccount* account = &bank_accounts[company_id];

        // Acquire the mutex associated with the company's bank account
        pthread_mutex_lock(&account->mutex);
        fprintf(output_file,"[VTM%d]: Customer%d,%dTL,%s \n", machine->machine_id+1,machine-> currentCustomer+1, machine->balance, account->name);
        // Update the company's bank account balance and machine's fields 
        account->balance += machine->balance;
        machine->balance = 0;
        machine->company_id = -1;
        machine->total_customer -= 1;
        // Release the mutex associated with the company's bank account
        pthread_mutex_unlock(&account->mutex);
        // Release the mutex associated with the machine
        pthread_mutex_unlock(&machine->mutex);
        }
        pthread_exit(NULL);
}

int main(int argc, char** argv) {
    // Check that the input file was provided
    if (argc < 2) {
        fprintf(stderr, "Error: No input file provided\n");
        return 1;
    }

    // Open the input file
    char* input_file_name = argv[1];
    // Create the output file name
    char* output_file_name = malloc(strlen(input_file_name) + 8);
    strcpy(output_file_name, input_file_name);
    output_file_name[strlen(input_file_name) - 4] = '\0';
    strcat(output_file_name, "_log.txt");
    // Open the input and output files
    output_file = fopen(output_file_name, "w");
    FILE* input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Could not open input file\n");
        return 1;
    }

    // Create bank accounts
    for (int i = 0; i < NUM_COMPANIES; i++) {
        BankAccount* account = malloc(sizeof(BankAccount));
        account->balance = 0;
        strcpy(account->name, companies[i]);
        pthread_mutex_init(&account->mutex, NULL);
        bank_accounts[i] = *account;
    }


    char line[256];
    fgets(line, 256, input_file);
    sscanf(line, "%d", &num_customers);
    //Create machine instances
    pthread_t machine_threads[NUM_MACHINES];
    for (int i = 0; i < NUM_MACHINES; i++) {
        Machine* machine = malloc(sizeof(Machine));
        machine->machine_id = i;
        machine->balance = 0;
        machine->company_id = -1;
        machine->total_customer = 0;
        pthread_mutex_init(&machine->mutex, NULL);
        machines[i] = *machine;     
    }

    pthread_t customer_threads[num_customers];
    Customer* customers[num_customers];

    // Create customer instances
    for (int i = 0; i < num_customers; i++) {
        Customer* customer = (Customer*) malloc(sizeof(Customer));
        customer->customer_id = i;
        char* name = malloc(sizeof(char) * 256);
        fgets(line, 256, input_file);
        sscanf(line, "%d,%d,%[^,],%d", &customer->sleep_time, &customer->machine_id, name, &customer->amount);
        for (int j= 0; j < 5; j++){
            if (strcmp(name, companies[j]) == 0){
                customer->company_id = j;
                break;
            }
        }
        customer->machine_id = customer->machine_id - 1;
        customers[i] = customer;
        machines[customer->machine_id].total_customer += 1;   
    }

    // Create machine threads
    for (int i = 0; i < NUM_MACHINES; i++) {
        if(machines[i].total_customer == 0){
            continue;
        }
        pthread_create(&machine_threads[i], NULL, machine_thread_func, (void*) &machines[i]);
    }
    // Create customer threads
    for (int i = 0; i < num_customers; i++) {
        pthread_create(&customer_threads[i], NULL, customer_thread_func, (void*) customers[i]);
    }
    // Wait for all customers to finish
    for (int i = 0; i < num_customers; i++) {
        pthread_join(customer_threads[i], NULL);
    }
    // Wait for all machines to finish
    for (int i = 0; i < NUM_MACHINES; i++) {
        pthread_join(machine_threads[i], NULL);
    }
    fprintf(output_file,"[Main]: All payments are completed\n");
    // Print the final balances of each company
    for (int i = 0; i < NUM_COMPANIES; i++) {

        fprintf(output_file,"[Main]: %s: %d\n", companies[i], bank_accounts[i].balance);
        }
    fclose(input_file);
    fclose(output_file);
    return 0;
  }


