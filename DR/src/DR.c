/*
    Filename: DR.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file serves as a driver code  for the Data Reader. 

*/

#include "../inc/DR.h"

int main()
{
    key_t queueKey, shmKey;
    int msgQueueId = 0, shmId = 0;
    MasterList *masterListPtr = NULL;
    bool continueListening = true;

    if (initializeMessageQueue(&queueKey, &msgQueueId) != OK ||
        setupSharedMemory(&shmKey, &shmId, &masterListPtr, msgQueueId) != OK) {
        return ERROR;
    }




    sleep(5);
    processMessages(msgQueueId, masterListPtr, &continueListening);

    cleanup(msgQueueId, shmId, masterListPtr);

    return 0;

}