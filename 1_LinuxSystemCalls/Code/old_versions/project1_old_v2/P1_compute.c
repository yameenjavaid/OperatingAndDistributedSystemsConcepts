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

typedef struct _data{
    int _id;
    int _INC;
    int _startInterval;
    int _endInterval;
    int _a;
    int _b;
    int _c;
    int _count;
}  ComputationParameters;

void getRelationIsTrueCount(ComputationParameters *_cp){

    //c => [id*INC, (id+1)*INC)
    _cp->_startInterval = _cp->_id * _cp->_INC;
    _cp->_endInterval = (_cp-> _id + 1) * _cp->_INC;
    _cp->_count = 0;

    for(_cp->_c = _cp->_startInterval; _cp->_c < _cp->_endInterval; _cp->_c++ ){

        for(_cp->_a = 0; _cp->_a <= _cp->_c; _cp->_a++){

            for(_cp->_b = 0; _cp->_b <= _cp->_c; _cp->_b++){

                int _cSquared = _cp->_c * _cp->_c;

                int _abSquared = (_cp->_a * _cp->_a) + (_cp->_b * _cp->_b);

                if(_cSquared  == _abSquared){
                    _cp->_count = _cp->_count + 1;
                }
            }
        }
    }
}

int main(int _argc, char *_argv[], char *_envp[]){

    //define integer to store id value
    int _id = 0;

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
    ComputationParameters * _cp = (ComputationParameters *)malloc(sizeof(ComputationParameters));
    _cp->_a = 0;
    _cp->_b = 0;
    _cp->_c = 0;
    _cp->_count = 0;
    _cp->_endInterval = 0;
    _cp->_id = 0;
    _cp->_INC = 0;
    _cp->_startInterval = 0;

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
            (*_cp)._id = atoi(_delimiterFinder); //convert id from string to integer

            _delimiterFinder = strtok(NULL,"_");
            (*_cp)._INC = atoi(_delimiterFinder); //convert INC from string to integer
        }
        else{
            printf("Command Mismatch: %s instead of %s, CID: %d, errno:%d\n\n",_readBuffer,COMPUTE,getpid(),errno);
            exit(3);
        }
    }

    //start computation
    getRelationIsTrueCount(_cp);

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
    printf("compute(%d): For interval [%d,%d): count = %d \n",getpid(),_cp->_startInterval,_cp->_endInterval, _cp->_count);

    //free allocated memory
    free(_cp);

    //Indicate Exit
    printf("compute(%d): Terminating Normally.\n",getpid());

    exit(10);

    return 0;
}
