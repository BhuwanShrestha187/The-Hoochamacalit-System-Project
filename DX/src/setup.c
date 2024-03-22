#include "../inc/DX.h"

/*
    Filename: setup.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines the createSharedMemKey(), checkShareMemID(), and checKMessageQueue() function defined in DX.h

*/


/*
Generates a System V IPC key for shared memory based on a pathname and a project identifier.
This function utilizes 'ftok' to convert a pathname (CURRENT_DIRECTORY) and a project identifier (SHM_KEY) into a unique key, intended for use in subsequent shared memory operations.
Returns: The generated key on success, or -1 on failure if it cannot generate a valid key.
*/

int createsharedMemKey()
{
    return ftok(CURRENT_DIRECTORY, SHM_KEY);
}

/*
Attempts to locate the segment of shared memory associated with the given key.
- sharedMemKey: The System V IPC key for the shared memory segment to be located.
This function calls 'shmget' with the key, specifying the size as the size of an integer and a flag of 0, implying it does not attempt to create the segment but only checks its existence.
Returns: The shared memory segment ID on success, or -1 on failure if the segment cannot be located or if an error occurs.
*/

int checkSharedMemID(int sharedMemKey)
{
    return shmget(sharedMemKey, sizeof(int), 0);
}


/*
Attempts to send a test message to a message queue identified by msgQueID to check its existence and accessibility.
- msgQueID: The identifier of the message queue to be checked.
A MESSAGE structure is filled with test data (type, PID, and status) and sent to the message queue. The function calculates the size of the MESSAGE structure excluding the 'long' type member to correctly determine the message size for 'msgsnd'.
Returns: SUCCESS if the message is successfully sent, indicating the message queue is accessible. Returns FAILURE if 'msgsnd' fails, suggesting the message queue may not exist or is not accessible.
*/

int checkMessageQueue(int msgQueID)
{
	MESSAGE msg;
	int sizeofdata = sizeof (MESSAGE) - sizeof (long);
	msg.type = 1;
	msg.PID = 1;
	msg.status = 1;

	if(msgsnd (msgQueID, (void *)&msg, sizeofdata, 0) == FAILURE)
    {
        return FAILURE;
    }
    else
    {
        return SUCCESS;
    }
}