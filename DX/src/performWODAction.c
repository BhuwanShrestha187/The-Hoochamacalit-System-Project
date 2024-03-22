

#include "../inc/DX.h"
/*
    Filename: messageLogger.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines the performWODAction() function defined in DX.h

*/
int killProcess(int processID, int actionStatus, int dcAffected); 
int deleteMsgQueue(int msgQueueID); 


/*
Executes a Wipe Out Data (WOD) action based on a randomly generated status, affecting specific data centers or message queues.
- masterList: Pointer to a structure containing information about data centers and a message queue.
This function generates a random status code, which determines the action to be taken:
    - No action (DO_NOTHING variants).
    - Terminate processes associated with specific data centers (KILL_DC_X variants, where X indicates the data center number).
    - Delete a message queue (DEL_MSG_QUE variants).
Returns SUCCESS if the action is successfully completed or no action is needed, FAILURE if an unknown status code is encountered.
*/

int performWODAction(MasterList *masterList)
{
    //Generate a random status
    int status = generateRandomStatus();
    switch(status)
    {
        case DO_NOTHINGA: 
            return SUCCESS; 
            break;
        
        case KILL_DC_01A: 
            return killProcess(masterList->dc[0].dcProcessID, KILL_DC_01A, status + 1);
            break;

        case KILL_DC_03A: 
            return killProcess(masterList->dc[2].dcProcessID, KILL_DC_03A, status + 1);
            break;

        case KILL_DC_02A: 
            return killProcess(masterList->dc[1].dcProcessID, KILL_DC_02A, status + 1);
            break;

        case KILL_DC_01B: 
            return killProcess(masterList->dc[0].dcProcessID, KILL_DC_01B, status + 1);
            break;

        case KILL_DC_03B: 
            return killProcess(masterList->dc[2].dcProcessID, KILL_DC_03B, status + 1);
            break;

        case KILL_DC_02B: 
            return killProcess(masterList->dc[1].dcProcessID, KILL_DC_02B, status + 1);
            break;

        case KILL_DC_04A: 
            return killProcess(masterList->dc[3].dcProcessID, KILL_DC_04A, status + 1);
            break;

        case DO_NOTHINGB: 
            return SUCCESS; 
            break;

        case KILL_DC_05A: 
            return killProcess(masterList->dc[4].dcProcessID, KILL_DC_05A, status + 1);
            break;

        case DEL_MSG_QUEA:
            return deleteMsgQueue(masterList->msgQueueID); 
            break;

        case KILL_DC_01C: 
            return killProcess(masterList->dc[0].dcProcessID, KILL_DC_01C, status + 1);
            break;

        case KILL_DC_06A: 
            return killProcess(masterList->dc[5].dcProcessID, KILL_DC_06A, status + 1);
            break;

        case KILL_DC_02C: 
            return killProcess(masterList->dc[1].dcProcessID, KILL_DC_02C, status + 1);
            break;
        
        case KILL_DC_07A: 
            return killProcess(masterList->dc[6].dcProcessID, KILL_DC_07A, status + 1);
            break;

        case KILL_DC_03C: 
            return killProcess(masterList->dc[2].dcProcessID, KILL_DC_03C, status + 1);
            break;

        case KILL_DC_08A: 
            return killProcess(masterList->dc[7].dcProcessID, KILL_DC_08A, status + 1);
            break;

        case DEL_MSG_QUEB:
            return deleteMsgQueue(masterList->msgQueueID); 
            break;

        case KILL_DC_09A: 
            return killProcess(masterList->dc[8].dcProcessID, KILL_DC_09A, status + 1);
            break;
        
        case DO_NOTHINGC: 
            return SUCCESS; 
            break;
        
        case KILL_DC10A:
            return killProcess(masterList->dc[9].dcProcessID, KILL_DC10A, status + 1);
            break;

        default:
        return FAILURE;
    }
}

/*
killProcess:
Attempts to terminate a specified process by sending a SIGHUP signal, first checking if the process exists without sending a signal.
- processID: The identifier of the process to be terminated.
- actionStatus: The action status code, used for logging purposes.
- dcAffected: Identifier for the data center affected, used for logging.
Returns a status code indicating the outcome: INVALID_PROCESS if the process doesn't exist, KILL_PROCESS_FAILED if the signal could not be sent, or KILL_PROCESS_SUCCEDED if the process was successfully terminated.
*/

int killProcess(int processID, int actionStatus, int dcAffected)
{
    //Check process without sending a signal
    if(kill(processID, 0) != 0)
    {
        logMessage(processID, INVALID_PROCESS, actionStatus, dcAffected);
        return INVALID_PROCESS; 
    }

    //Now the process exists, so send the SIGHUP signal
    if(kill(processID, SIGHUP) != 0)
    {
        logMessage(processID, KILL_PROCESS_FAILED, actionStatus, dcAffected); 
        return KILL_PROCESS_FAILED; 
    }

    //Else successfully killed
    logMessage(processID, KILL_PROCESS_SUCCEDED, actionStatus, dcAffected);
    return KILL_PROCESS_SUCCEDED;
}

/*
Attempts to delete a specified message queue.
- msgQueueID: The identifier of the message queue to be deleted.
First checks if the message queue exists. If it doesn't, logs the failure and returns CANNOT_FIND_MESSAGE_QUEUE. 
If the message queue exists, it attempts to delete it. If successful, logs the deletion and returns MESSAGE_QUEUE_DELETED; otherwise, returns FAILURE.
*/
int deleteMsgQueue(int msgQueueID)
{
    int messageQueueExists = checkMessageQueue(msgQueueID); 
    if(messageQueueExists == FAILURE)
    {
        logMessage(0, CANNOT_FIND_MESSAGE_QUEUE, 0, 0);
        return CANNOT_FIND_MESSAGE_QUEUE;
    }

    else
    {
        messageQueueExists = msgctl(msgQueueID, IPC_RMID, NULL); 
        if(messageQueueExists == 0)
        {
            logMessage(0, MESSAGE_QUEUE_DELETED, 0, 0); 
            return MESSAGE_QUEUE_DELETED; 
        }
        else
        {
            return FAILURE; 
        }
    }
}