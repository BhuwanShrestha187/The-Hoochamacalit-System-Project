
/*
    Filename: utilities.c
    Project: Hoochamacalit System
    Course" System Programming
    Author: Bhuwan Shrestha (8892146), Nischal Khatri(8896183), Shubh Hemal Soni(8887735)
    Description: This file defines the randomSleep(), generateRandomSleep() function defined in DX.h

*/
#include "../inc/DX.h"

/*
Generates a random sleep duration within a specified range and pauses execution for that period.
This function calculates a random sleep time based on a defined interval (SLEEP_INTERVAL) and a maximum value (MAX_SLEEP_SECONDS). The actual sleep duration is the base interval plus a random value between 0 and the difference between the maximum sleep seconds and the base interval, inclusive.
After calculating the sleep time, it prints the duration to the console and then suspends execution for that number of seconds using the 'sleep' function.
*/

void randomSleep()
{
    int sleep_time = SLEEP_INTERVAL + (rand() % (MAX_SLEEP_SECONDS - SLEEP_INTERVAL + 1));
    printf("Sleeping for %d seconds.\n", sleep_time);
    sleep(sleep_time);
}

/*
Generates a random status number within the range of defined Wipe Out Data (WOD) actions.
This function uses 'rand()' to generate a random integer, which it then bounds by 'MAX_WOD_ACTION' using the modulo operator. This ensures the generated status number falls within the acceptable range of defined WOD actions.
Returns: A random status number between 0 and MAX_WOD_ACTION - 1, inclusive.
*/

int generateRandomStatus()
{
    int statusNumber = 0; 
    statusNumber = rand() % MAX_WOD_ACTION;
    return statusNumber;
}

