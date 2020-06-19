/**
 * @author Swapnil Acharya
 * @since 01/23/2020
 * @file P1.h
*/

#ifndef _P1_H_
#define _P1_H_

//number of child processes
#define MAXCP   2

//define buffer lengths for commands between parent and child processes
#define CMD_LEN     250

//commands used to communicate between controller
//and compute
#define READY   "READY"
#define COMPUTE     "COMPUTE"
#define DONE    "DONE"
#define PRINT   "PRINT"

//define process id for two child processes
#define COMPUTE_ID0  0
#define COMPUTE_ID1  1

//define increment value
#define INC 1000


#endif