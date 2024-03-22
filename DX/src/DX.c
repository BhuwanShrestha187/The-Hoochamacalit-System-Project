/*
    Filename: DX.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file serves as a entry point for the Data Corruptor. 

*/

#include "../inc/DX.h"

int main()
{
    srand(time(NULL));
    MasterList* masterList = NULL; 

    int msgQueueID = 0; 
    key_t sharedMemKey = createsharedMemKey(); 
    if(sharedMemKey == -1)
    {
        printf("Shared Mem key generation failed..\n"); 
        exit(1); 
    }

    int sharedMemID = FAILURE; 
    int retry = 0; 

    //If shared memory piece has not been created, it will sleep for 10 seconds. 
    while(sharedMemID == FAILURE && retry < MAX_RETRIES)
    {
        sharedMemID = checkSharedMemID(sharedMemKey); 
        if(sharedMemID != FAILURE)
        {
            printf("Shared Memory ID found: %d\n", sharedMemID);
            break; 
        }

        else
        {
            sleep(SLEEP_INTERVAL);
            retry++;
        }

    }

    if(retry == MAX_RETRIES)
    {
        printf("Maximum Retries reached. Now exiting the application"); 
        logMessage(0, CANNOT_FIND_MEMORY_ID, 0, 0);
        exit(2);
    }
    printf("Shared Memory is found: %d/n", sharedMemID); 

    masterList = (MasterList*)shmat(sharedMemID, NULL, SHM_RDONLY);  //Attach the shared memory segment
    if(masterList == (void*)FAILURE)
    {
        printf("Cannot attach masterlist.\n");
        exit(3);
    }

    bool continueLoop = true;
    int wODResult = 0; 
    while(continueLoop)
    {
        randomSleep(); 
        //Now check for the message queue exists or not
        msgQueueID = checkMessageQueue(masterList->msgQueueID); 
        if(msgQueueID == FAILURE)
        {
            printf("Message Queue ID Is not found!!\n"); 
            logMessage(0, CANNOT_FIND_MESSAGE_QUEUE, 0, 0);
            return FAILURE; 
        }

        else
        {
            printf("Message Queue ID found and the ID is %d\n", masterList->msgQueueID); 
        }

        printf("Now performing the WOD action..\n");
        //Now perform the action
        wODResult = performWODAction(masterList); 


        if(wODResult == FAILURE)
        {
            return FAILURE; 
        }

        if(wODResult == MESSAGE_QUEUE_DELETED || CANNOT_FIND_MESSAGE_QUEUE)
        {
            printf("Deleted the MEssage Queue, Now exiting..\n"); 
            shmdt(masterList); 
            return SUCCESS;
        }



    }
    
}