/**
 * @author Swapnil Acharya
 * @since 01/23/2020
 * @file P1.h
*/

#ifndef _P1_H_
#define _P1_H_

//number of child processes
#define MAXCP   2

//defing buffer lengths for read and write
#define CMD_LEN     250
#define RESP_LEN    250

//commands used to communicate between controller
//and compute
#define READY   "READY"
#define COMPUTE     "COMPUTE"
#define DONE    "DONE"
#define PRINT   "PRINT"

//deine process id for two child processes
#define PLAYER1_ID  0
#define PLAYER2_ID  1

//define increment value
#define INC 3000


#endif