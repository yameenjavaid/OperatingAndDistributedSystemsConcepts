/**
 * @author Swapnil Acharya
 * @since 01/23/2020
 * @file P1_controller.c
*/

//include custom files
#include "P1.h"

//include standard libary
#include <stdio.h>
#include <stdlib.h>

//include required linux headers
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

//include other headers
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct _compTime{
        time_t _startTime;
        time_t _endTime;
        double _totalTime;
} ComputationTime;


int main(int _argc, char *_argv[], char *_envp[]){

    //container to hold child process computaion time
    ComputationTime *_ctimeP1 = (ComputationTime *)malloc(sizeof(ComputationTime));
    _ctimeP1->_startTime = 0;
    _ctimeP1->_endTime = 0;
    _ctimeP1->_totalTime = 0;

    ComputationTime *_ctimeP2 = (ComputationTime *)malloc(sizeof(ComputationTime));
    _ctimeP2->_startTime = 0;
    _ctimeP2->_endTime = 0;
    _ctimeP2->_totalTime = 0;

    //define pipes for communication between
    // controller to player 1
    int _pipe_ctrlToP1[2], _pipe_p1ToCtrl[2];

    //define messasge buffers for communication between
    //controler and player 1
    char _readStreamP1[CMD_LEN], _writeStreamP1[RESP_LEN];


    //define pipes for communication between
    // controller to player 2
    int _pipe_ctrlToP2[2], _pipe_p2ToCtrl[2];

    //define message buffer for communication between
    //controller and player 2
    char _readStreamP2[CMD_LEN], _writeStreamP2[RESP_LEN];

    //define buffers for read and write
    char _readBuffer[RESP_LEN];
    char _writeBuffer[CMD_LEN];

    //create pipes for communication between
    // controller to player 1
    int _err = pipe(_pipe_ctrlToP1);
    if(_err == 1){
        printf("Controller: Error creating _pipe_ctrlToP1, errno =%d\n\n",errno);
        exit(1);
    }

    _err = pipe(_pipe_p1ToCtrl);
        if(_err == -1){
        printf("Controller: Error creating _pipe_p1ToCtrl, errno =%d\n\n",errno);
        exit(2);
    }

    //create pipes for communication between
    // controller to player 2
    _err = pipe(_pipe_ctrlToP2);
    if(_err == -1){
        printf("Controller: Error creating _pipe_ctrlToP2, errno =%d\n\n",errno);
        exit(3);
    }

    _err = pipe(_pipe_p2ToCtrl);
    if(_err == -1){
        printf("Controller: Error creating _pipe_p2ToCtrl, errno =%d\n\n",errno);
        exit(4);
    }


    //create child processes
    int i =0;
    pid_t _processId[MAXCP];
    int _error;

    for(i=0; i < MAXCP; i++){

        //storing ids in an array,
        //In parent process, the ids of child processes are stored in this array
        //If this array is later accesed in parent process, the id of child processes can be accessed
        _processId[i] = fork();

        if(_processId[i] < 0){
            printf("Error creating child Process, Process Id =%d, Errno %d\n\n",getpid(),errno);
            exit(5);
        }
        else if(_processId[i] == 0){

            if( i  == 0){ //player 1

                //close unused pipes used for communication with Player 1
                close(_pipe_ctrlToP1[1]); //close write end
                close(_pipe_p1ToCtrl[0]);//close read end

                //create read and write stream
                sprintf(_readStreamP1,"%d", _pipe_ctrlToP1[0]); //send read stream
                sprintf(_writeStreamP1,"%d", _pipe_p1ToCtrl[1]); //write stream

                _error = execl("./compute",_readStreamP1,_writeStreamP1,(char *)NULL);
                if(_error == -1){
                    printf("Error excuting Execl, Child PID %d, errno%d\n\n",getpid(),errno);
                    exit(6);
                }

            }
            else if(i == 1){ //player 2

                //close unused pipes used for communication with Player 2
                close(_pipe_ctrlToP2[1]); //close write end
                close(_pipe_p2ToCtrl[0]);//close read end

                //create read and write stream
                sprintf(_readStreamP2,"%d", _pipe_ctrlToP2[0]); //send read stream
                sprintf(_writeStreamP2,"%d",_pipe_p2ToCtrl[1]);//send read stream

                _error = execl("./compute",_readStreamP2,_writeStreamP2,(char *)NULL);
                if(_error == -1){
                    printf("Error excuting Execl, Child PID %d, errno%d\n\n",getpid(),errno);
                    exit(7);
                }
            }
        }
    }


    //close unused pipe for communication with Player 1
    close(_pipe_ctrlToP1[0]); //close read end
    close(_pipe_p1ToCtrl[1]); //close write end

    //close unused pipe for communication with Player 2
    close(_pipe_ctrlToP2[0]); //close read end
    close(_pipe_p2ToCtrl[1]); //close write end

    //wait for READY from  player 1
    _error = read(_pipe_p1ToCtrl[0],_readBuffer,RESP_LEN);
    if(_error == -1){
        printf("Error while reading from _pipe_p1ToCtrl[0] , PID:%d, errno%d\n\n",getpid(),errno);
        exit(8);
    }

    //check to see if the reposne from player 1 is => READY
    if(strncmp(_readBuffer,READY,strlen(READY)) == 0){
        printf("controller: %s received.\n",READY);
    }
    else{
        printf("Response from Player 1 mismatch. \n");
        exit(9);
    }


    //wait for READY from  player 2
    _error = read(_pipe_p2ToCtrl[0],_readBuffer,RESP_LEN);
    if(_error == -1){
        printf("Error while reading from _pipe_p2ToCtrl[0], PID:%d, errno%d\n\n",getpid(),errno);
        exit(10);
    }

    //check to see if the reposne from player 2 is => READY
    if(strncmp(_readBuffer,READY,strlen(READY)) == 0){
        printf("controller: %s received.\n",READY);
    }
    else{
        printf("Response from Player 2 mismatch. \n");
        exit(11);
    }

    //start computation time for P1
    time(&(_ctimeP1->_startTime));

    //send COMPUTE to Player1
    printf("controller: Sending %s to child process(%d).\n",COMPUTE,_processId[0]);
    sprintf(_writeBuffer,"%s_%i_%i",COMPUTE,PLAYER1_ID,INC);
    _error = write(_pipe_ctrlToP1[1],_writeBuffer,strlen(_writeBuffer)+1);
    if(_error == -1){
        printf("Error while writting to _pipe_ctrlToP1[1], PID:%d, errno:%d\n\n",getpid(), errno);
        exit(12);
    }

    //start computation time for P2
    time(&(_ctimeP2->_startTime));

    //send compute to Player 2
    printf("controller: Sending %s to child process(%d).\n",COMPUTE,_processId[1]);
    sprintf(_writeBuffer,"%s_%i_%i",COMPUTE,PLAYER2_ID,INC);
    _error = write(_pipe_ctrlToP2[1],_writeBuffer,strlen(_writeBuffer)+1);
    if(_error == -1){
        printf("Error while writting to _pipe_ctrlToP2[1], PID:%d, errno:%d\n\n",getpid(), errno);
        exit(13);
    }

    //wait for DONE Response from player 1
    _error = read(_pipe_p1ToCtrl[0],_readBuffer,RESP_LEN);
    if(_error == -1){
        printf("Error while reading from _pipe_p1ToCtrl[0] , PID:%d, errno%d\n\n",getpid(),errno);
        exit(14);
    }

    //check to see if the reposne from player 1 is => DONE
    if(strncmp(_readBuffer,DONE,strlen(DONE)) == 0){
        printf("controller: %s received.\n",DONE);
    }
    else{
        printf("Response from Player 1 mismatch. \n");
        exit(15);
    }

    //Measure time taken to compute
    time(&(_ctimeP1 -> _endTime));

    //wait for DONE Response from player 2
    _error = read(_pipe_p2ToCtrl[0],_readBuffer,RESP_LEN);
    if(_error == -1){
        printf("Error while reading from _pipe_p2ToCtrl[0] , PID:%d, errno%d\n\n",getpid(),errno);
        exit(16);
    }

    //check to see if the reposne from player 2 is => DONE
    if(strncmp(_readBuffer,DONE,strlen(DONE)) == 0){
        printf("controller: %s received.\n",DONE);
    }
    else{
        printf("Response from Player 2 mismatch. \n");
        exit(17);
    }

    //Measure time taken to compute
    time(&(_ctimeP2->_endTime));

    //calculate total computaion time FOR pLAYER 1
    _ctimeP1->_totalTime = difftime(_ctimeP1->_endTime,_ctimeP1->_startTime);
    printf("controller: Total Computaion Time for child process(%d): %f seconds\n",_processId[0],_ctimeP1->_totalTime);

    //calculate total computaion time  FOR PLAYER 2
    _ctimeP2->_totalTime = difftime(_ctimeP2->_endTime,_ctimeP2->_startTime);
    printf("controller: Total Computaion Time for child process(%d): %f seconds\n",_processId[1],_ctimeP2->_totalTime);


    //send PRINT to Player1
    printf("controller: Sending %s to child process(%d).\n",PRINT,_processId[0]);
    sprintf(_writeBuffer,"%s",PRINT);
    _error = write(_pipe_ctrlToP1[1],_writeBuffer,strlen(_writeBuffer)+1);
    if(_error == -1){
        printf("Error while writting to _pipe_ctrlToP1[1], PID:%d, errno:%d\n\n",getpid(), errno);
        exit(18);
    }

    //send PRINT to Player 2
    printf("controller: Sending %s to child process(%d).\n",PRINT,_processId[1]);
    sprintf(_writeBuffer,"%s",PRINT);
    _error = write(_pipe_ctrlToP2[1],_writeBuffer,strlen(_writeBuffer)+1);
    if(_error == -1){
        printf("Error while writting to _pipe_ctrlToP2[1], PID:%d, errno:%d\n\n",getpid(), errno);
        exit(19);
    }

    //wait for Player 1 to exit
    int _status1;
    waitpid(_processId[0],&_status1,0);
    if(WIFEXITED(_status1)){
        printf("controller: child(%d) terminated with exit status: %d\n",_processId[0],WEXITSTATUS(_status1));
    }

    //wait for PLayer 2 to exit
    int _status2;
    waitpid(_processId[1],&_status2,0);
    if(WIFEXITED(_status2)){
        printf("controller: child(%d) terminated with exit status: %d\n",_processId[1],WEXITSTATUS(_status2));
    }

	//free allocated memory in heapcheck
	free(_ctimeP1);
	free(_ctimeP2);
	
    printf("controller: Complete.\n");

    exit(20);
}
