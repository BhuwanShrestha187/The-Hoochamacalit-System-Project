/*
    Filename: DX.h
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file serves as a header file for the Data Corruptor. 

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

#define FAILURE -1
#define SUCCESS 0

#define SHM_KEY 16535
#define MAX_RETRIES 100
#define SLEEP_INTERVAL 10
#define MAX_SLEEP_SECONDS 30

#define CURRENT_DIRECTORY "."
#define LOG_FILE_PATH "/tmp/dataCorruptor.log"

#define MAX_WOD_ACTION 21
#define MAX_DC_ROLES 10

#define DO_NOTHINGA 0
#define KILL_DC_01A 1
#define KILL_DC_03A 2
#define KILL_DC_02A 3
#define KILL_DC_01B 4
#define KILL_DC_03B 5
#define KILL_DC_02B 6
#define KILL_DC_04A 7
#define DO_NOTHINGB 8
#define KILL_DC_05A 9
#define DEL_MSG_QUEA 10
#define KILL_DC_01C 11
#define KILL_DC_06A 12
#define KILL_DC_02C 13
#define KILL_DC_07A 14
#define KILL_DC_03C 15
#define KILL_DC_08A 16
#define DEL_MSG_QUEB 17
#define KILL_DC_09A 18
#define DO_NOTHINGC 19
#define KILL_DC10A 20

#define INVALID_PROCESS 2
#define KILL_PROCESS_FAILED 3
#define KILL_PROCESS_SUCCEDED 4
#define MESSAGE_QUEUE_DELETED 5
#define CANNOT_FIND_MESSAGE_QUEUE 6
#define CANNOT_FIND_MEMORY_ID 7

//Struct to hold the DC information
typedef struct
{
    pid_t dcProcessID;
    time_t lastTimeHeardFrom;
} DCInfo;

// Struct to hold the clients
typedef struct
{
    int msgQueueID;
    int numberOfDCs;
    DCInfo dc[MAX_DC_ROLES];
} MasterList;

//Struct to hold the messages
typedef struct 
{
    long type;
    pid_t PID;
    int status;

} MESSAGE;

//Function Prototypes
int logMessage(int processID, int status, int WODStatus, int dcAffected);
void randomSleep();
int checkMessageQueue(key_t msgQueueKey);
int performWODAction(MasterList *masterList);
int generateRandomStatus();
int checkSharedMemID(int sharedMemKey);
int createsharedMemKey();
