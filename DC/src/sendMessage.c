/*
    Filename: sendMessage.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines the functions declared in DC.h

*/
#include "../inc/DC.h"

/*
Sends a status message to a specified message queue for a given process.

- `processID`: The process ID to which the message pertains.
- `messageQueueID`: The identifier of the message queue where the message will be sent.
- `status`: The status code to be sent in the message.

The function dynamically allocates memory for a MESSAGE structure, setting its type, process ID, and status. It then attempts to send this message to the message queue identified by `messageQueueID`. After attempting to send the message, it frees the dynamically allocated memory.

Returns:
- SUCCESS if the message is successfully sent.
- FAILURE if memory allocation fails or sending the message fails.
*/

int sendMessage(pid_t processID, int messageQueueID, int status)
{
    int sizeofdata = sizeof(MESSAGE) - sizeof(long);
    // Dynamically allocate memory for MESSAGE
    MESSAGE* msg = (MESSAGE*)malloc(sizeof(MESSAGE));
    if (msg == NULL) // Check if the memory allocation failed
    {
        return FAILURE;
    }
    msg->type = 1;
    msg->PID = processID;
    msg->status = status;

    // Send the message
    if (msgsnd(messageQueueID, (void *)msg, sizeofdata, 0) == FAILURE)
    {
        free(msg); // Free the allocated memory on failure
        return FAILURE;
    }
    else
    {
        free(msg); // Free the allocated memory on success
        return SUCCESS;
    }
}

/*
Creates or acquires a semaphore for process synchronization, initializing it if it's newly created.

1. Generates a unique key for the semaphore using `ftok` with a predefined directory and value. If key generation fails, it prints an error and returns FAILURE.
2. Attempts to get the semaphore ID with `semget`. If the semaphore doesn't exist (indicated by `-1`), it tries to create a new semaphore set with `IPC_CREAT` and appropriate permissions. If creation or access fails, it prints an error and returns FAILURE.
3. If the semaphore was successfully accessed or created and is new, it initializes the semaphore using `semctl` to a predefined initial value. If initialization fails, it prints an error and returns FAILURE.
4. Attempts to acquire the semaphore for use with an operation that decrements the semaphore value (locks it). If this operation fails, it prints an error and returns FAILURE.

Returns SUCCESS if the semaphore is successfully created or acquired and locked for use.
*/
int createSemaphore()
{
    key_t semaphoreKey = ftok(CURRENT_DIRECTORY, AGREED_UPON_VALUE);
    if (semaphoreKey == -1) {
        perror("ftok error");
        return FAILURE;
    }
    int semaphoreID = semget(semaphoreKey, 1, 0);
    if (semaphoreID == -1) {
        // If semaphore already exists, try to access it instead
        semaphoreID = semget(semaphoreKey, NUM_OF_SEMS, (IPC_CREAT | 0660));
        if (semaphoreID == -1) {
            perror("semget error");
            return FAILURE;
        }
    } else {
        unsigned short init_values[NUM_OF_SEMS] = {SEM_INITAL_VALUE};
        if (semctl(semaphoreID, 0, SETALL, init_values) == FAILURE) {
            perror("semctl error - init");
            return FAILURE;
        }
    }

    struct sembuf acquire_operation = {0, -1, SEM_UNDO};
    if (semop(semaphoreID, &acquire_operation, 1) == FAILURE) {
        perror("semop error - acquire");
        return FAILURE;
    }

    return SUCCESS;
}