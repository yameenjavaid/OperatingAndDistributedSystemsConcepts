/**
 * @author Swapnil Acharya
 * @since 01/23/2020
 * @file P1_compute.c
*/

//include custom files
#include "P1.h"

//include standard library
#include <stdio.h>
#include <stdlib.h>

//include required linux headers
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

//include other headers
#include <string.h>


int getRelationIsTrueCount(int _start, int _end){

    //c => [id*INC, (id+1)*INC)
	// a, b are integers in the interval (0,c)
    

	//intialie variables
	int a = 0, b =0, c = 0;
	int _count = 0; //variable to hold the number of count when c^2 = a^2 + b^2 is true
	
	
    for(c = _start; c < _end; c++ ){

        for(a = 1; a < c; a++){

            for(b = 1; b < c; b++){

                int _cSquared = c * c;

                int _abSquared = (a * a) + (b * b);

                if(_cSquared  == _abSquared){
					
                    _count = _count + 1;
					
                }
            }
        }
    }
	
	return _count;
}

int main(int _argc, char *_argv[], char *_envp[]){


    char _readBuffer[CMD_LEN];
    char _writeBuffer[CMD_LEN];

    //get stream pipes
    int _readPipe = atoi(_argv[0]);
    int _writePipe = atoi(_argv[1]);


    //sending READY to parent process
    printf("compute(%d): Sending %s to parent process. \n",getpid(), READY);
    int _error = write(_writePipe,READY,strlen(READY)+1);
    if(_error == -1){
        printf("Error writting to pipe, CID: %d, errno:%d\n\n",getpid(),errno);
        exit(1);
    }

    //define computaion parameters
    int _id = 0; //variable to hold id value
    int _INC = 0; //variable too hold increment value
    int _startInterval = 0;
	int _endInterval = 0;
	int _count = 0;

    //wait for compute from parent process
    _error = read(_readPipe,_readBuffer,CMD_LEN);
    if(_error == - 1){
        printf("Error reading from pipe, CID:%d, errno:%d\n\n",getpid(),errno);
        exit(2);
    }
    else{

        if(strncmp(_readBuffer,COMPUTE,strlen(COMPUTE)) == 0){
            printf("compute(%d): %s recevied.\n",getpid(),COMPUTE);

            //extract data from buffer
            char * _delimiterFinder;
            _delimiterFinder = strtok(_readBuffer,"_"); //extract compute

            _delimiterFinder = strtok(NULL,"_"); //extract id
            _id = atoi(_delimiterFinder); //convert id from string to integer

            _delimiterFinder = strtok(NULL,"_");
            _INC = atoi(_delimiterFinder); //convert INC from string to integer
        }
        else{
            printf("Command Mismatch: %s instead of %s, CID: %d, errno:%d\n\n",_readBuffer,COMPUTE,getpid(),errno);
            exit(3);
        }
    }

    //start computation
	//c => [id*INC, (id+1)*INC)
	_startInterval = _id * _INC;
	_endInterval = (_id + 1) * _INC;
	_count = getRelationIsTrueCount(_startInterval,_endInterval);

    //send DONE TO Controller
    printf("compute(%d): Sending %s to parent process. \n",getpid(), DONE);
    _error = write(_writePipe,DONE,strlen(DONE)+1);
    if(_error == -1){
        printf("Error writting to pipe, CID: %d, errno:%d\n\n",getpid(),errno);
        exit(4);
    }


    //wait for print command from parent process
    _error = read(_readPipe,_readBuffer,CMD_LEN);
    if(_error == - 1){
        printf("Error reading from pipe, CID:%d, errno:%d\n\n",getpid(),errno);
        exit(5);
    }
    else{

        if(strncmp(_readBuffer,PRINT,strlen(PRINT)) == 0){
            printf("compute(%d): %s recevied.\n",getpid(),PRINT);
        }
        else{
            printf("Command Mismatch: %s instead of %s, CID: %d, errno:%d\n\n",_readBuffer,COMPUTE,getpid(),errno);
            exit(6);
        }
    }

    //print compute parameters and resuls
    printf("compute(%d): For interval [%d,%d): count = %d \n",getpid(),_startInterval,_endInterval,_count);


    //Indicate Exit
    printf("compute(%d): Terminating Normally.\n",getpid());

    exit(10);

    return 0;
}
