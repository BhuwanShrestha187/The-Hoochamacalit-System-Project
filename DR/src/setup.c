/*
    Filename: setup.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines several functions that are declared in DR.h


*/

#include "../inc/DR.h"

/*
Initializes a message queue, generating its key and obtaining the message queue ID.

Parameters:
- queueKey: Pointer to a key_t variable where the generated key for the message queue will be stored.
- msgQueueId: Pointer to an int variable where the message queue ID will be stored.

The function generates a System V IPC key for the message queue using 'ftok', based on the current directory and a predefined value (AGREED_UPON_VALUE). If key generation fails, it logs an error message and returns ERROR.

With the generated key, the function attempts to create or access a message queue using 'msgget', with creation flags (IPC_CREAT) and predefined permissions (IPC_PERMISSIONS). If this step fails, it logs an error and returns ERROR.

Returns:
- OK if both the key generation and message queue creation/access are successful.
- ERROR if either the key generation fails or the message queue cannot be created/accessed.
*/

int initializeMessageQueue(key_t *queueKey, int *msgQueueId) {
    *queueKey = ftok(CURRENT_DIRECTORY, AGREED_UPON_VALUE);
    if (*queueKey == -1) {
        logError("Failed to generate key for message queue.\n");
        return ERROR;
    }

    *msgQueueId = msgget(*queueKey, IPC_CREAT | IPC_PERMISSIONS);
    if (*msgQueueId == -1) {
        logError("Failed to create or access message queue.\n");
        return ERROR;
    }

    return OK;
}

/*
Sets up shared memory for the MasterList structure and initializes it.

- `shmKey`: Pointer to store the generated key for shared memory.
- `shmId`: Pointer to store the ID of the shared memory segment.
- `masterListPtr`: Pointer to a pointer of MasterList to attach the shared memory segment.
- `msgQueueId`: Message queue ID to be stored in the MasterList.

The function generates a key for shared memory using `ftok`, then attempts to allocate a shared memory segment with `shmget`. On success, it attaches this segment to a MasterList pointer using `shmat`.

It initializes the MasterList by setting process IDs and last heard times to zero, updates the number of data centers to zero, and stores the message queue ID.

Returns `OK` on successful setup and initialization, `ERROR` on any failure, logging the specific error encountered.
*/

int setupSharedMemory(key_t *shmKey, int *shmId, MasterList **masterListPtr, int msgQueueId) {
    *shmKey = ftok(CURRENT_DIRECTORY, SHARED_MEMORY_KEY);
    if (*shmKey == -1) {
        logError("Failed to generate key for shared memory.\n");
        return ERROR;
    }

    *shmId = shmget(*shmKey, sizeof(MasterList), IPC_CREAT | IPC_PERMISSIONS);
    if (*shmId == -1) {
        logError("Failed to allocate shared memory segment.\n");
        return ERROR;
    }

    *masterListPtr = (MasterList*)shmat(*shmId, NULL, 0);
    if (*masterListPtr == (void*)-1) {
        logError("Failed to attach shared memory segment.\n");
        return ERROR;
    }

    // Initialize the master list
    for (int i = 0; i < MAX_DC_ROLES; i++) {
        (*masterListPtr)->dc[i].dcProcessID = 0;
        (*masterListPtr)->dc[i].lastTimeHeardFrom = 0;
    }
    (*masterListPtr)->numberOfDCs = 0;
    (*masterListPtr)->msgQueueID = msgQueueId;

    return OK;
}


/*
Processes messages from a queue and evaluates client status until instructed to stop.

- `msgQueueId`: ID of the message queue to listen on.
- `masterListPtr`: Pointer to the MasterList containing client info.
- `continueListening`: Pointer to a boolean controlling the loop.

Loops until `*continueListening` is false. Attempts to receive a message; if it fails, logs an error, sets `*continueListening` to false, and exits the loop. Then, checks for inactive clients and evaluates the status of the received message's client. If any check fails, stops listening. Pauses for `MESSAGE_POST_DELAY` seconds after each message.
*/

void processMessages(int msgQueueId, MasterList *masterListPtr, bool *continueListening) {
    MESSAGE msg;

    while (*continueListening) {
        if (receiveMessage(masterListPtr, &msg) == FAILURE) {
            logError("Error reading message queue. Check that it exists!!.");
            *continueListening = false;
            break;
        }

        // Process message and check for inactive clients
        if (checkInactiveClients(masterListPtr) == FAILURE || 
            evaluateClientStatus(masterListPtr, msg) == FAILURE) {
            *continueListening = false;
            break;
        }

        sleep(MESSAGE_POST_DELAY);
    }
}

/*
Cleans up resources including a message queue and shared memory.

- `msgQueueId`: ID of the message queue to be removed.
- `shmId`: ID of the shared memory segment to be removed.
- `masterListPtr`: Pointer to the attached shared memory to be detached.

First, removes the specified message queue using `msgctl`. Then detaches the shared memory pointed to by `masterListPtr` using `shmdt`. Finally, removes the shared memory segment identified by `shmId` using `shmctl`.
*/

void cleanup(int msgQueueId, int shmId, MasterList *masterListPtr) {
    msgctl(msgQueueId, IPC_RMID, NULL); // Remove message queue
    shmdt(masterListPtr); // Detach shared memory
    shmctl(shmId, IPC_RMID, NULL); // Remove shared memory segment
}