/*
    Filename: logger.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines several functions that are declared in DR.h


*/
#include "../inc/DR.h"

/*
Retrieves the current system time and formats it into a human-readable string.
This function first obtains the current time using 'time(NULL)' and then converts it to local time with 'localtime'.
A static character array 'formattedTime' is used to store the formatted string to ensure it remains valid after the function returns.
The time is formatted into a string in the format "[YYYY-MM-DD HH:MM:SS]", using 'sprintf'.
Returns: A pointer to the statically allocated string containing the formatted time. Note that subsequent calls to this function will overwrite this string.
*/

char* getCurrentFormattedTime()
{
    time_t currentTime = time(NULL); 
    struct tm *localTime = localtime(&currentTime);
    static char formattedTime[20];      //Buffer to hold the formatted time
    sprintf(formattedTime, "[%d-%02d-%02d %02d:%02d:%02d]", localTime->tm_year + 1900, localTime->tm_mon + 1,
        localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
    return formattedTime;
}

/*
Writes a log entry to the specified file.
- fp: Pointer to the file where the log entry will be written. The file should already be open for writing.
- entry: The log entry string to be written to the file.

This function checks if the file pointer 'fp' is not NULL, indicating the file is open and available for writing. If so, it writes the 'entry' string to the file using 'fputs'.
If 'fp' is NULL, indicating an issue with the file opening or it's not available, it prints an error message to the standard output.
*/

void writeLogEntry(FILE* fp, const char* entry)
{
    if(fp != NULL)
    {
        fputs(entry, fp); 
    }
    else
    {
        printf("Error waiting to log file!\n"); 
    }
}

/*
Logs various status messages for data center (DC) operations, including addition, removal, and status updates, to a specified log file.

Parameters:
- dcIndex: Index of the data center in the master list.
- dcPID: Process ID of the data center.
- msgStatus: Status code indicating the specific message or event.
- dcStatus: Status code indicating the overall status of the data center (e.g., non-responsive, offline, new).

The function first formats the current time, adjusts the dcIndex for human readability (starting from 1), and selects the appropriate log message based on the dcStatus. It constructs a detailed log entry using these parameters and predefined status messages.

It attempts to open the log file specified by LOG_LOCATION for appending. If successful, it writes the formatted log entry to the file. If the file cannot be opened, it prints an error message and returns ERROR.

Returns:
- OK if the log entry is successfully written to the file.
- ERROR if the log file cannot be opened.
*/

int logMessage(int dcIndex, int dcPID, int msgStatus, int dcStatus)
{
    char *messages[] = {EVERYTHING_OK_MSG, HYDR_PRES_FAIL_MSG, SAFE_BUTTON_FAIL_MSG, NO_RAW_MATER_MSG, OPER_TMP_OUT_RNG_MSG,
                        OPERATOR_ERROR_MSG, MACH_OFFLINE_MSG};

    FILE* fp = fopen(LOG_LOCATION, "a"); 
    if(!fp)
    {
   
        return ERROR; 
    }

    char logEntry[512]; 
    char *currentTimeFormatted = getCurrentFormattedTime(); 
    dcIndex += 1; 

     switch (dcStatus) {
        case NON_RESPONSIVE_DC_INT:
            sprintf(logEntry, "%s : DC-%.2d [%d] removed from master list - NON-RESPONSIVE\n",
                    currentTimeFormatted, dcIndex, dcPID);
            break;
        case MACH_OFFLINE_INT:
            sprintf(logEntry, "%s : DC-%.2d [%d] has gone OFFLINE - removed from master list\n",
                    currentTimeFormatted, dcIndex, dcPID);
            break;
        case NEW_DC_INT:
            sprintf(logEntry, "%s : DC-%.2d [%d] added to the master list - NEW DC - Status %d (%s)\n",
                    currentTimeFormatted, dcIndex, dcPID, msgStatus, messages[msgStatus]);
            break;
        case ALL_DCS_OFFLINE_INT:
            sprintf(logEntry, "%s %s\n", currentTimeFormatted, ALL_DCS_OFFLINE_MSG);
            break;
        default: // Normal operation.
            sprintf(logEntry, "%s : DC-%.2d [%d] updated in the master list - MSG RECEIVED - Status %d (%s)\n",
                    currentTimeFormatted, dcIndex, dcPID, msgStatus, messages[msgStatus]);
            break;
    }

    writeLogEntry(fp, logEntry);
    fclose(fp);
    return OK;
}

/*
Logs an error message to a predefined log file with the current timestamp.

Parameters:
- errorMessage: The error message to be logged.

The function opens the log file specified by LOG_LOCATION in append mode. If the file cannot be opened, it returns ERROR.

A buffer 'logEntry' is used to format the log entry, which includes the current timestamp (retrieved by calling 'getCurrentFormattedTime') and the provided error message.

After formatting the log entry, it calls 'writeLogEntry' to write the message to the file, then closes the file.

Returns:
- OK if the log entry is successfully written.
- ERROR if the file cannot be opened.
*/


int logError(char *errorMessage) {
    FILE *fp = fopen(LOG_LOCATION, "a");
    if (!fp) {
        return ERROR;
    }

    char logEntry[512]; // Buffer to construct the log entry.
    char *currentTimeFormatted = getCurrentFormattedTime();

    // Construct the log entry with the current time and the error message.
    sprintf(logEntry, "%s %s\n", currentTimeFormatted, errorMessage);

    // Write the log entry and close the file.
    writeLogEntry(fp, logEntry);
    fclose(fp);

    return OK;
}