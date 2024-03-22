/*
    Filename: DC.h
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file serves as a header file for the Data Creator. 

*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/sem.h>

#define AGREED_UPON_VALUE 123456789
#define CURRENT_DIRECTORY "../../DR/bin" 

#define MAX_SLEEP 30
#define MIN_SLEEP 10

#define FAILURE  -1
#define SUCCESS 0

#define SLEEP_ID_MSG_QUEUE_NOT_EXISTS 10

//Function Prototypes
int sendMessage(pid_t processID, int messageQueueID, int status); 
int logMessage(int processID, int status); 
int generateRandomStatus(); 
int createSemaphore(); 
int releaseSemaphore(); 
int getRandSleep(); 


// Setup Function Prototypes
pid_t initializeDataCreator(void);
key_t createMessageQueueKey(void);
int checkMessageQueue(key_t msgQueueKey); 

#define EVERYTHING_OK_INT 0
#define EVERYTHING_OK_MSG "Everything is OKAY"
#define HYDR_PRES_FAIL_INT 1
#define HYDR_PRES_FAIL_MSG "Hydraulic Pressure Failure"
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

#define TOTAL_STATUSES 7

// Special statuses for logging
#define MACH_OFF_SHUTD_INT 7
#define MSG_QUE_GONE_INT 8
#define CANT_GET_MSG_KEY_INT 9


#define LOG_FILE_PATH "/tmp/dataCreator.log"
#define MAX_LOG_LEN 100



// Semaphore Constants
#define SEM_FTOK_ID 123456789
#define NUM_OF_SEMS 1
#define CHECK_SEM_EXISTS 0
#define SEM_INITAL_VALUE 1
#define NUM_SOP_STRUCTS 1

// MESSAGESTRUCT struct definition
typedef struct MESSAGESTRUCT 
{
	long type;
	pid_t PID;	
	int status;	
	
} MESSAGE;
