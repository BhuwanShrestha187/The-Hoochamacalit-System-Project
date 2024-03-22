/*
    Filename: DC.h
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file serves as a entry point for the Data Creator. 

*/
#include "../inc/DC.h"

int main()
{
    int randomStatus = 0; 
    bool shutdownDC = false; 
    int sleepInterval = 0; 

    pid_t processID = initializeDataCreator(); 
    key_t msgQueueKey = createMessageQueueKey(); 
    if(msgQueueKey == -1)
    {
        logMessage(processID, CANT_GET_MSG_KEY_INT); 
        return 1; 
    }

    //Check if message queue exists
    int msgQueueID = checkMessageQueue(msgQueueKey); 
    while(msgQueueID == FAILURE)
    {
        sleep(SLEEP_ID_MSG_QUEUE_NOT_EXISTS);
        msgQueueID = checkMessageQueue(msgQueueKey);
    }

    if(sendMessage(processID, msgQueueID, EVERYTHING_OK_INT) == -1)
    {
        logMessage(processID, MSG_QUE_GONE_INT);
        return SUCCESS;
    }

    else
    {
        logMessage(processID, randomStatus) == FAILURE;
    }


    sleepInterval = getRandSleep(); 
    sleep(sleepInterval); 

    while(shutdownDC == false)
    {
        randomStatus = generateRandomStatus(); 
        if(randomStatus == MACH_OFFLINE_INT)
        {
            shutdownDC = true; 
        }

        if(sendMessage(processID, msgQueueID, randomStatus) == FAILURE)
        {
            if(logMessage(processID, MSG_QUE_GONE_INT) == FAILURE)
            {
                return FAILURE; 
            }

            return SUCCESS; 
        }

        else
        {
            if(logMessage(processID, randomStatus) == FAILURE)
            {
                return FAILURE;
            }

        }

        if(shutdownDC == true)
        {
            if(logMessage(processID, MACH_OFF_SHUTD_INT) == FAILURE)
            {
                return FAILURE; 
            }

            return SUCCESS; 
        }

        else
        {
            sleepInterval = getRandSleep(); 
            sleep(sleepInterval);
        }
    }

    
}
