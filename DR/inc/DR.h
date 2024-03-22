
/*
    Filename: DR.h
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file serves as a header file for the Data Reader. 

*/
 #include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>    
#include <sys/msg.h>    
#include <sys/shm.h>    
#include <stdbool.h>    
#include <unistd.h>     
#include <time.h>       
#include <string.h>     

//Secret Constants and Keys
#define AGREED_UPON_VALUE 123456789

#define CURRENT_DIRECTORY "."       //Represents the current directory
#define SHARED_MEMORY_KEY 16535     // A key for creating or accessing shared memory segment, used for IPC.
#define IPC_PERMISSIONS 0660           // Specifies permission for creating IPC resources

//Error-Success Code - These are just error and success codes
#define FAILURE -1       
#define OK 0
#define ERROR 1

#define INITIAL_DELAY  15 //Sleep for 1 seconds when start
#define MESSAGE_POST_DELAY  1.5

//Limit for CLient inactivity
#define INACTIVE_CLIENT_TIME_SECONDS 35 //Seconds to give enough time for the client to start


#define EVERYTHING_OK_INT 0
#define EVERYTHING_OK_MSG "Everything is OKAY"
#define HYDR_PRES_FAIL_INT 1
#define HYDR_PRES_FAIL_MSG "Hyfraulic Presssure Failure"
#define SAFE_BUTTON_FAIL_INT 2
#define SAFE_BUTTON_FAIL_MSG "Safety Button Failure"
#define NO_RAW_MATER_INT 3
#define NO_RAW_MATER_MSG "No Raw Material in the Process"
#define OPER_TMP_OUT_RNG_INT 4
#define OPER_TMP_OUT_RNG_MSG "Operating Temperature Out of Range"
#define OPERATOR_ERROR_INT 5
#define OPERATOR_ERROR_MSG "Operator Error"
#define MACH_OFFLINE_INT 6
#define MACH_OFFLINE_MSG "Machine is Off-line"

#define EXISTING_DC_INT 0
#define NON_RESPONSIVE_DC_INT 7
#define NEW_DC_INT 8
#define DC_UPDATED_INT 9
#define ALL_DCS_OFFLINE_INT 10
#define ALL_DCS_OFFLINE_MSG "All DCs have gone offline or terminated - DR TERMINATING"

/*
    - these constants helps to distinguish message is coming form DC or DX.
*/
#define TYPE_DC  1 // type for DC messages
#define TYPE_DX  0 // type for DX messages

// given by sean from assignment specifications
#define MAX_DC_ROLES  10 //It means maximum DC that server can handle at a time

// MESSAGESTRUCT struct definition
typedef struct MESSAGESTRUCT
{
    long type;
    pid_t PID;
    int status;

} MESSAGE;

typedef struct
{
    pid_t dcProcessID;
    long lastTimeHeardFrom;
} DCInfo;

typedef struct
{
    int msgQueueID;
    int numberOfDCs;
    DCInfo dc[MAX_DC_ROLES];
} MasterList;



//  logging file location
#define LOG_LOCATION "/tmp/dataMonitor.log"

// function prototypes
int initializeMessageQueue(key_t *queueKey, int *msgQueueId);
int setupSharedMemory(key_t *shmKey, int *shmId, MasterList **masterListPtr,  int msgQueueId);
void processMessages(int msgQueueId, MasterList *masterListPtr, bool *continueListening);
void cleanup(int msgQueueId, int shmId, MasterList *masterListPtr);
int receiveMessage(MasterList *list, MESSAGE *msg);
int evaluateClientStatus(MasterList *list, MESSAGE msg);
int findClientById(MasterList list, int clientID);
int checkInactiveClients(MasterList *list);
void reorder(MasterList *list);
int removeClient(MasterList *list, int clientIndex);
int logMessage(int dcIndex, int dcPID, int msgStatus, int dcStatus);
int logError(char *errorMessage);