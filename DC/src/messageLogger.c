/*
    Filename: messageLogger.h
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines the functions declared in DC.h

*/
#include "../inc/DC.h"

/*
Logs a specific status message for a data center to a log file with a timestamp.

- `dcPID`: The process ID of the data center for which the message is being logged.
- `logFile`: Pointer to the file where the log should be written.
- `messageToLog`: The specific message to log.
- `to`: Pointer to a `struct tm` representing the current time.

Formats and writes a log entry to the specified file, including a timestamp, the data center's process ID, and the specific message.

Returns SUCCESS after logging the message.
*/

int logMessageSpecStatus(int dcPID, FILE* logFile, char* messageToLog, struct tm* to)
{
    fprintf(logFile, "[%d-%02d-%02d %02d:%02d:%02d] : DC [%d] - %s\n", to->tm_year + 1900, to->tm_mon + 1, 
                to->tm_mday, to->tm_hour, to->tm_min, to->tm_sec, dcPID, messageToLog);
    return SUCCESS; 
}

/*
Logs a regular status message with a specific status code for a data center to a log file, including a timestamp.

- `dcPID`: Process ID of the data center the message pertains to.
- `logFile`: File pointer to the log file where the message should be written.
- `statusToLog`: The status code related to the message being logged.
- `messageToLog`: The descriptive message that accompanies the status code.
- `to`: Pointer to a `struct tm` structure representing the current time.

Writes a formatted log entry to `logFile` that includes the current timestamp, the data center's process ID, the status code, and a descriptive message.

Returns SUCCESS after successfully logging the message.
*/

int logMessageRegStatus(int dcPID, FILE* logFile, int statusToLog, char* messageToLog, struct tm* to)
{
    fprintf(logFile, "[%d-%02d-%02d %02d:%02d:%02d] : DC [%d] - MSG SENT - Status %d (%s)\n", to->tm_year + 1900, to->tm_mon + 1, 
                to->tm_mday, to->tm_hour, to->tm_min, to->tm_sec, dcPID, statusToLog, messageToLog);

    return SUCCESS; 
}

/*
Maps integer status codes to their corresponding status message strings.

- `status`: The integer status code to be mapped to a message.

Returns the message string corresponding to the provided status code. If the status code does not match any predefined case, it returns "Unknown Status".

This function facilitates translating numerical status codes into human-readable messages for logging and display purposes.
*/

char* getStatusMessage(int status) {
    switch (status) {
        case EVERYTHING_OK_INT: return EVERYTHING_OK_MSG;
        case HYDR_PRES_FAIL_INT: return HYDR_PRES_FAIL_MSG;
        case SAFE_BUTTON_FAIL_INT: return SAFE_BUTTON_FAIL_MSG;
        case NO_RAW_MATER_INT: return NO_RAW_MATER_MSG;
        case OPER_TMP_OUT_RNG_INT: return OPER_TMP_OUT_RNG_MSG;
        case OPERATOR_ERROR_INT: return OPERATOR_ERROR_MSG;
        case MACH_OFFLINE_INT: return MACH_OFFLINE_MSG;
        default: return "Unknown Status";
    }
}

/*
Maps special integer status codes to their corresponding detailed status messages.

- `status`: The integer status code to be mapped.

Switches on the `status` code to return a specific message string for special cases that require detailed explanations. These messages are used for logging or displaying more critical or specific system states.

Returns:
- A detailed message string for recognized special status codes.
- "Unknown Special Status" for any code not explicitly handled.
*/

char* getSpecialStatusMessage(int status) {
    switch (status) {
        case MACH_OFF_SHUTD_INT: return "Machine Offline Status Generated. Shutting Down the Machine!!";
        case MSG_QUE_GONE_INT: return "Cannot send the message. The queue has been gone. Shutting Down the Machine";
        case CANT_GET_MSG_KEY_INT: return "Cannot get the message key. Shutting Down the Machine";
        default: return "Unknown Special Status";
    }
}


/*
Logs a message with a specific status code for a data center to a predefined log file.

- `dcPID`: Process ID of the data center to log the message for.
- `statusToLog`: Status code indicating the type of message to log.

The function performs the following steps:
1. Attempts to create a semaphore for thread/process synchronization. Returns FAILURE if unable to create.
2. Opens the log file specified by LOG_FILE_PATH in append mode. If the file cannot be opened, returns FAILURE.
3. Retrieves the current time to include as a timestamp in the log message.
4. Depending on the status code (`statusToLog`), it logs either a regular status message (using `logMessageRegStatus`) or a special status message (using `logMessageSpecStatus`).
5. Closes the log file and releases the semaphore. If either action fails, returns FAILURE.

Returns:
- SUCCESS if the log message is successfully written and the log file is properly closed and the semaphore is released.
- FAILURE if any step fails, including semaphore creation, file opening, file closing, or semaphore release.
*/

int logMessage(int dcPID, int statusToLog) {
    if (createSemaphore() == FAILURE) {
        return FAILURE;
    }

    // Attempt to open the log file
    FILE* logFile = fopen(LOG_FILE_PATH, "a+");
    if (!logFile) {
        return FAILURE;
    }

    // Get the current time
    time_t currentTime = time(NULL);
    struct tm* to = localtime(&currentTime);

    // Log the message based on the status
    switch (statusToLog) {
        case EVERYTHING_OK_INT:
        case HYDR_PRES_FAIL_INT:
        case SAFE_BUTTON_FAIL_INT:
        case NO_RAW_MATER_INT:
        case OPER_TMP_OUT_RNG_INT:
        case OPERATOR_ERROR_INT:
        case MACH_OFFLINE_INT:
            logMessageRegStatus(dcPID, logFile, statusToLog, getStatusMessage(statusToLog), to);
            break;

        case MACH_OFF_SHUTD_INT:
        case MSG_QUE_GONE_INT:
        case CANT_GET_MSG_KEY_INT:
            logMessageSpecStatus(dcPID, logFile, getSpecialStatusMessage(statusToLog), to);
            break;

        default:
            fclose(logFile);
            releaseSemaphore();
            return FAILURE;
    }

    if (fclose(logFile) == FAILURE || releaseSemaphore() == FAILURE) {
        return FAILURE;
    }

    return SUCCESS;
}



/*
Releases a semaphore to signal that a shared resource is available.

The function constructs a semaphore operation for release, increments the semaphore value by 1, allowing other processes or threads waiting on this semaphore to proceed.

Steps:
1. Generates a unique key for the semaphore using `ftok` with a predefined directory and value.
2. Retrieves the semaphore ID using `semget` with the generated key.
3. Performs a semaphore operation (`semop`) to increment the semaphore, indicating a release of the resource.

If any step fails (indicated by returning -1), an appropriate error message is printed, and the function returns FAILURE. Otherwise, it returns SUCCESS, indicating the semaphore was successfully released.
*/

int releaseSemaphore()
{
    struct sembuf release_operation = {0, 1, SEM_UNDO};
    key_t semaphoreKey = ftok(CURRENT_DIRECTORY, AGREED_UPON_VALUE);
    if (semaphoreKey == -1) {
        perror("ftok error");
        return FAILURE;
    }

    int semaphoreID = semget(semaphoreKey, 1, 0);
    if (semaphoreID == -1) {
        perror("semget error");
        return FAILURE;
    }

    if (semop(semaphoreID, &release_operation, 1) == -1) {
        perror("semop error");
        return FAILURE;
    }

    return SUCCESS;
}

/*
Logs an error message to a specified log file with a current timestamp.

- `errorMessage`: The error message to be logged.

The function first obtains the current time and converts it to local time. It then attempts to open the predefined log file in append mode. If the file cannot be opened, it prints an error message to the standard output and returns 1 to indicate failure.

If the file is successfully opened, it formats and writes the error message along with a timestamp to the file. Finally, it closes the file.

Returns SUCCESS if the error message is successfully written to the log file.
*/

int logError(char *errorMessage)
{
    struct tm *to;
    time_t currentTime;
    currentTime = time(NULL);
    to = localtime(&currentTime);

    FILE *fp = NULL;

    fp = fopen(LOG_FILE_PATH, "a");

    if (fp == NULL)
    {
        printf("Error writing to log file!\n");
        return 1;
    }

    fprintf(fp, "[%d-%02d-%02d %02d:%02d:%02d] %s\n", to->tm_year + 1900, to->tm_mon + 1,
            to->tm_mday, to->tm_hour, to->tm_min, to->tm_sec, errorMessage);

    fclose(fp);

    return SUCCESS;
}