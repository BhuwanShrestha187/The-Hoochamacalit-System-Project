/*
    Filename: setup.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines the functions declared in DC.h

*/
#include "../inc/DC.h"

/*
Retrieves and returns the process ID of the calling process.

This function is designed to initialize a data creator by simply obtaining its own process ID using 'getpid()', which is a standard POSIX function. This could be used for tracking or managing processes in applications where the process ID is needed for identification or other purposes.

Returns: The process ID of the calling process.
*/

pid_t initializeDataCreator(void)
{
    return getpid(); 
}


/*
Generates a System V IPC key for a message queue using a unique directory and a predefined value.

This function calls 'ftok' with a constant path to the current directory (CURRENT_DIRECTORY) and a predefined project identifier (AGREED_UPON_VALUE) to generate a unique key. This key is typically used to create or access a message queue.

Returns: The generated key on success. If the function fails to generate a key, -1 is returned.
*/

key_t createMessageQueueKey(void)
{
    return ftok(CURRENT_DIRECTORY, AGREED_UPON_VALUE);
}

/*
Checks the existence of a message queue associated with the given key.

This function attempts to retrieve the message queue identifier associated with the specified key using 'msgget'. The key is passed as an argument, and the function tries to access the message queue without creating it. If successful, the function returns the message queue identifier. If the message queue does not exist or if an error occurs, the function returns -1.

Parameters:
- msgQueueKey: The System V IPC key associated with the message queue.

Returns:
- The message queue identifier if it exists.
- -1 if the message queue does not exist or if an error occurs.
*/

int checkMessageQueue(key_t msgQueueKey)
{
    return msgget(msgQueueKey, 0);
}

/*
Generates a random sleep interval between MIN_SLEEP and MAX_SLEEP.

This function initializes the random number generator using the current time as the seed. It then generates a random number within the range [MIN_SLEEP, MAX_SLEEP] using the 'rand' function. The result represents the number of seconds to sleep.

Returns:
- An integer representing a random sleep interval between MIN_SLEEP and MAX_SLEEP.
*/

int getRandSleep()
{
     int result = 0;
    time_t secondsSinceEpoch = 0;
      
    // Intialize random number generator
    srand((unsigned) time(&secondsSinceEpoch));

    // Get random interval to sleep for between 10 and 30 seconds
    result = rand() % (MAX_SLEEP + 1 - MIN_SLEEP) + MIN_SLEEP;

    // Return random interval
    return result;
}

/*
Generates a random status code.

This function initializes the random number generator using the current time as the seed. It then generates a random number representing a status code within the range [0, TOTAL_STATUSES - 1], where TOTAL_STATUSES is the total number of possible status codes.

Returns:
- An integer representing a randomly generated status code.
*/

int generateRandomStatus()
{
    int result = 0;
    time_t secondsSinceEpoch = 0;
    
    srand((unsigned) time(&secondsSinceEpoch));

    // Get random status
    result = rand() % TOTAL_STATUSES;
    return result;
}

