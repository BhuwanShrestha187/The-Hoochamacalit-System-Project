#include "../inc/DX.h"
/*
    Filename: messageLogger.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines the logMessage() function defined in DX.h

*/

/*
Logs messages related to process management and actions in a predefined log file, based on different status codes.
- processID: Identifier for the process involved in the log message.
- status: Specifies the type of log message to be recorded (e.g., message queue deleted, invalid process).
- WODStatus: Status code for "Wipe Out Data" actions, used in specific log messages.
- dcAffected: Identifier for the data center affected in certain actions.
Returns FAILURE on error or if the status code does not match predefined cases, -1 if closing the log file fails.
*/

int logMessage(int processID, int status, int WODStatus, int dcAffected)
{
    // Get the current time
    struct tm* to;
    time_t currentTime;
    currentTime = time(NULL);
    to = localtime(&currentTime);

    // open the log file 
    FILE* logFile = NULL;
    int closeFileStatus = 0;
    logFile = fopen(LOG_FILE_PATH, "a+");
    if(logFile == NULL)
    {
        return FAILURE;
    }

    switch (status)
    {
    case MESSAGE_QUEUE_DELETED:
        // Print the formatted message to the file
        fprintf(logFile, "[%d-%02d-%02d %02d:%02d:%02d] : DX deleted the msgQ – the DR/DCs can’t talk anymore - exiting\n", 
            to->tm_year + 1900, to->tm_mon + 1, to->tm_mday, to->tm_hour, to->tm_min, to->tm_sec);
        break;
    case INVALID_PROCESS:
        fprintf(logFile, "[%d-%02d-%02d %02d:%02d:%02d] : No process with the PID [%d] could be found.\n", 
            to->tm_year + 1900, to->tm_mon + 1, to->tm_mday, to->tm_hour, to->tm_min, to->tm_sec, processID);
        break;
    case CANNOT_FIND_MESSAGE_QUEUE:
        fprintf(logFile, "[%d-%02d-%02d %02d:%02d:%02d] : Cannot find message queue.\n", 
            to->tm_year + 1900, to->tm_mon + 1, to->tm_mday, to->tm_hour, to->tm_min, to->tm_sec);
        break;
    case KILL_PROCESS_SUCCEDED:
        fprintf(logFile, "[%d-%02d-%02d %02d:%02d:%02d] : WOD Action %02d - DC-%02d [%d] TERMINATED\n", 
            to->tm_year + 1900, to->tm_mon + 1, to->tm_mday, to->tm_hour, to->tm_min, to->tm_sec, WODStatus, dcAffected, processID);
        break;
    case KILL_PROCESS_FAILED:
        fprintf(logFile, "[%d-%02d-%02d %02d:%02d:%02d] : Couldn't kill process with ID [%d].\n", 
            to->tm_year + 1900, to->tm_mon + 1, to->tm_mday, to->tm_hour, to->tm_min, to->tm_sec, dcAffected);
        break;
    case CANNOT_FIND_MEMORY_ID:
        fprintf(logFile, "[%d-%02d-%02d %02d:%02d:%02d] : Couldn't get the shared memory ID after 100 tries.\n", 
            to->tm_year + 1900, to->tm_mon + 1, to->tm_mday, to->tm_hour, to->tm_min, to->tm_sec);
        break;
    default:
        if(fclose(logFile) == -1)
        {
            return -1; 
        }
        return FAILURE;
        break;
    }

    if(fclose(logFile == -1))
    {
        return -1; 
    }
    return FAILURE;
}