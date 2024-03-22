/*
    Filename: setup.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines several functions that are declared in DR.h


*/

#include "../inc/DR.h"


/*
Adds a new client to the MasterList.

- `list`: Pointer to the MasterList where the new client will be added.
- `msg`: Pointer to the MESSAGE structure containing the new client's information.
- `now`: Current time, used to update the last time heard from the new client.

This function increments the `numberOfDCs` in the MasterList, assigns the new client's process ID (`msg->PID`) and the current time (`now`) to the appropriate fields in the next available `dc` structure. If the new client's status is not marked as machine offline, it logs this addition with `logMessage`. Finally, it increments the count of data centers (`numberOfDCs`) in the list.
*/

void addNewClient(MasterList* list, MESSAGE* msg, time_t now)
{
    int newIndex = list->numberOfDCs; 
    list->dc[newIndex].dcProcessID = msg->PID; 
    list->dc[newIndex].lastTimeHeardFrom = now; 

    if(msg->status != MACH_OFFLINE_INT)
    {
        logMessage(newIndex, msg->PID, msg->status, NEW_DC_INT);
    }

    list->numberOfDCs++;
}

/*
Updates information for an existing client in the MasterList.

- `master`: Pointer to the MasterList containing the client to be updated.
- `message`: Pointer to the MESSAGE structure with the updated information.
- `index`: Index of the client in the MasterList to be updated.
- `now`: Current time, used to update the last time heard from the client.

This function updates the `lastTimeHeardFrom` field for the specified client in the MasterList to the current time (`now`). If the client's status is not marked as machine offline, it logs this update using `logMessage`.
*/

void updateClient(MasterList *master, MESSAGE *message, int index, time_t now) {
    master->dc[index].lastTimeHeardFrom = now;

    if (message->status != MACH_OFFLINE_INT) {
        logMessage(index, message->PID, message->status, EXISTING_DC_INT);
    }
}

/*
Receives and processes messages from the message queue associated with the MasterList.

- `list`: Pointer to the MasterList that contains the message queue ID.
- `msg`: Pointer to the MESSAGE structure where the received message will be stored.

First, the function attempts to receive a message from the message queue using `msgrcv`. If no message is received (indicated by FAILURE), the function returns FAILURE. 

Upon successfully receiving a message, it prints the status message associated with the received message's status code. 

If the message type indicates it's from a data center (TYPE_DC), the function then checks if the sender (identified by PID) is already in the MasterList. If not, it adds a new client to the MasterList; if the sender is found, it updates the existing client's details.

Returns OK after successfully processing the message, or FAILURE if unable to receive a message from the queue.
*/

int receiveMessage(MasterList *list, MESSAGE *msg)
{
    char *messages[] = {EVERYTHING_OK_MSG, HYDR_PRES_FAIL_MSG,
                        SAFE_BUTTON_FAIL_MSG, NO_RAW_MATER_MSG, OPER_TMP_OUT_RNG_MSG,
                        OPERATOR_ERROR_MSG, MACH_OFFLINE_MSG};

    int received = msgrcv(list->msgQueueID, (void *)msg, sizeof(MESSAGE), 0, 0);
    if (received == FAILURE) return FAILURE;

    int status = msg->status; 

    // only add to MasterList if message is from DC
    if (msg->type == TYPE_DC)
    {
        int index = findClientById(*list, msg->PID); 
        time_t now = time(NULL); 
        
        if(index == FAILURE)
        {
            addNewClient(list, msg, now); 
        }

        else
        {
            updateClient(list, msg, index, now); 
        }
    }

    return OK;
}


/*
Evaluates and handles the status of a client based on a received message.

- `list`: Pointer to the MasterList.
- `msg`: The message received for a client.

If the client's status in `msg` indicates the machine is offline (`MACH_OFFLINE_INT`), it searches for and removes the corresponding client from the MasterList. Logs the offline status of the client before removing.

Returns: 
- SUCCESS on successful removal.
- ERROR if the status is not handled or client cannot be found/removed.
*/

int evaluateClientStatus(MasterList *list, MESSAGE msg)
{
    if (msg.status == MACH_OFFLINE_INT)
    {
        // find and remove offline DC from MasterList
        for (int i = 0; i < list->numberOfDCs; i++)
        {
            if (list->dc[i].dcProcessID == msg.PID)
            {
                logMessage(i, list->dc[i].dcProcessID, MACH_OFFLINE_INT, MACH_OFFLINE_INT);  //Offline Clients
                return removeClient(list, i);
            }
        }
    }
    return ERROR; 
}

/*
Searches for a client in the MasterList by client ID.

- `list`: The MasterList to search within.
- `clientID`: The ID of the client to find.

Iterates through the data centers in the list. If a data center with a matching process ID is found, returns the index of that data center.

Returns:
- The index of the found client within the MasterList.
- FAILURE if no matching client is found.
*/

int findClientById(MasterList list, int clientID)
{
    for (int i = 0; i < list.numberOfDCs; i++) {
        if (list.dc[i].dcProcessID == clientID) return i;
    }
    return FAILURE;
}

/*
Checks for and handles inactive clients in the MasterList.

- `list`: Pointer to the MasterList to check.

Iterates through the list's data centers. If a data center hasn't communicated in more than `INACTIVE_CLIENT_TIME_SECONDS`, it logs the inactivity and attempts to remove the client from the list.

Returns:
- SUCCESS if an inactive client is found and successfully removed.
- ERROR if no inactive clients are found or if removal fails.
*/

int checkInactiveClients(MasterList *list)
{
    time_t currentTime = time(NULL);
    int i = 0;
    for (i = 0; i < list->numberOfDCs; i++)
    {
        // if last heard from time is more than 35 seconds less than current time,
        // client is inactive
        if (currentTime - list->dc[i].lastTimeHeardFrom > INACTIVE_CLIENT_TIME_SECONDS)
        {
            logMessage(i, list->dc[i].dcProcessID, MACH_OFFLINE_INT, NON_RESPONSIVE_DC_INT);
            return removeClient(list, i);
        }
    }
    return ERROR;
}

/*
Reorders the MasterList to remove gaps caused by removed clients.

- `list`: Pointer to the MasterList to reorder.

Iterates through the list, finding entries where the `dcProcessID` is 0 (indicating a removed client). For each such entry, it searches for a subsequent entry with a non-zero `dcProcessID` and moves it to the current position, effectively compacting the list.

This ensures that active clients are contiguous and removes any gaps.
*/

void reorder(MasterList *list)
{
   for (int i = 0; i < list->numberOfDCs; i++) {
        if (list->dc[i].dcProcessID == 0) {
            for (int j = i + 1; j < list->numberOfDCs; j++) {
                if (list->dc[j].dcProcessID != 0) {
                    list->dc[i] = list->dc[j];
                    list->dc[j].dcProcessID = 0;
                    list->dc[j].lastTimeHeardFrom = 0;
                    break;
                }
            }
        }
    }
}

/*
Removes a client from the MasterList and reorders the list.

- `list`: Pointer to the MasterList from which the client will be removed.
- `clientIndex`: Index of the client to remove in the MasterList.

Sets the `dcProcessID` and `lastTimeHeardFrom` of the specified client to 0, effectively marking it as removed. Decreases the total number of data centers in the list by one, then calls `reorder` to compact the list and remove the gap.

If all clients are removed, logs this with a specific message and returns FAILURE. Otherwise, returns OK.
*/

int removeClient(MasterList *list, int clientIndex)
{
    // remove DC from MasterList struct
    list->dc[clientIndex].dcProcessID = 0;
    list->dc[clientIndex].lastTimeHeardFrom = 0;

    list->numberOfDCs -= 1; //Decrease the number of DC by 1

    reorder(list);

    if (list->numberOfDCs == 0)
    {
        logMessage(clientIndex, list->dc[clientIndex].dcProcessID, ALL_DCS_OFFLINE_INT, ALL_DCS_OFFLINE_INT);
        return FAILURE;
    }

    return OK;
}