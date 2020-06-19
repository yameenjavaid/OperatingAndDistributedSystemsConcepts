/**
*@since 3/1/2020
*@file primeToken_client.c
* This file demonstrates the client part of RPC based client-server 
* communication
**/

#include "primeToken.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>


/**
* This function prints the details of token after being processed by client/
* before being passed to server.
*@param _tkn The token structure whose detail is to be printed
*@return nothing
*/
void printTokenDetailsBefore(Token _tkn){
	printf("\n\n Token Before Server\n");
	printf("Client:-- Hops: %i\n",_tkn._numberOfHops);
	printf("Client:-- Time: %i\n",_tkn._timeOfCreation);
	printf("Client:-- Next Int: %i\n",_tkn._nextInteger);
	printf("Client:-- Num Prime: %i\n",_tkn._numberOfPrimesFound);
	printf("Client:-- Highest: %i\n",_tkn._highestPrimeFound);
	printf("\n\n");
}

/**
* This function prints the details of token after being processed by server/
* before being passed to client.
*@param _tkn The token structure whose detail is to be printed
*@return nothing
*/
void printTokenDetailsAfter(Token _tkn){
	printf("\n\n Token After Server\n");
	printf("Client:-- Hops: %i\n",_tkn._numberOfHops);
	printf("Client:-- Time: %i\n",_tkn._timeOfCreation);
	printf("Client:-- Next Int: %i\n",_tkn._nextInteger);
	printf("Client:-- Num Prime: %i\n",_tkn._numberOfPrimesFound);
	printf("Client:-- Highest: %i\n",_tkn._highestPrimeFound);
	printf("\n\n");
}


/**
* This function checks where a given number si prime or not
*@param _num This parameter is checked if its prime or not
*@return true if the param a is prime, false otherwise
*/
bool isPrimeNumber(int _num){
    bool _flag = true;
	int i = 2;
	//only need to scan half of the number 
	//because if it is no divible by any number
	//lest than or equal its half, its prime
    for(i = 2; i <= _num / 2; i++) {
       if(_num % i == 0) {
          _flag = false;
          break;
       }
    }
    return _flag;
}


int main(int _argc, char * _argv[], char * _envp[]){
	
	//if server is not specifed then exit
	if(_argc < 2){
		perror("Client: Server Not Specified");
		exit(1);
	}
	
	CLIENT *_client; //client struct provided by rpc
	Token *_returnValue = (Token*)malloc(sizeof(Token));  //token to hold return 
	char *_server; //vaiable to hold server sent as argument to client executable
	
	//get specified server
	_server = _argv[1];
	
	//generate client handle to call the server and set communication protocol to tcp
	if ( (_client = clnt_create(_server, PRIME_COMPUTATION,PCOMP_1,"tcp")) == (CLIENT*)NULL ){
		clnt_pcreateerror(_server); //if there is an errnor creating client then display the error then exit
		exit(2);
	}
	
	//instantiate token
	Token _tkn;
	
	//initialize token
	_tkn._numberOfHops = 0;
	_tkn._timeOfCreation = 0;
	_tkn._nextInteger = 2;
	_tkn._numberOfPrimesFound = 0;
	_tkn._highestPrimeFound = 0;
	
	//create final elapsed time variable
	time_t _startTime, _endTime;
	
	//get time of token creation (before first hop)
	time(&(_tkn._timeOfCreation));
	
	
	//start pime computation
	int _startIndex = 0;
	bool _initialHop = true;
	time(&(_startTime)); //start timer
	
	printf("\n\nClient: Started Token Hopping Client-Server System with Limit = %i\n\n",LIMIT);
	
	while(_startIndex < LIMIT/2){
		
		//get the next integer to start finding next prime
		int _currentInteger = _tkn._nextInteger;
		
		//find the next prime and update token
		bool _isPrime = false;
		while(_isPrime == false){
			
			//get wether the given number is prime or not by calling "isPrimeNumber()" function
			if(isPrimeNumber(_currentInteger) == true){
				
				//set next integer to be one more than the value of the last prime
				_tkn._nextInteger = _currentInteger + 1;
				
				//increment the number of prime numbers found by 1
				_tkn._numberOfPrimesFound = _tkn._numberOfPrimesFound + 1;
				
				//update the highest prime
				if(_currentInteger > _tkn._highestPrimeFound){
					_tkn._highestPrimeFound = _currentInteger;
				}
				
				if(_initialHop == true){
					_initialHop = false;
				}
				else{
					//increment hop count
					_tkn._numberOfHops = _tkn._numberOfHops + 1; 
				}
				
				//exit loop
				_isPrime = true;
			}
			else{
				_isPrime = false;
				
				//increment index for next number to be checked for prime
				_currentInteger = _currentInteger + 1;
			}

		}
		
		//reset loop
		_isPrime = false;
		
		//printTokenDetailsBefore(_tkn);

		//pass token to server to find next prime
		_returnValue = start_prime_computation_1(&_tkn,_client);
		
		//copy returned token value from server to local token
		_tkn._numberOfHops =_returnValue->_numberOfHops;
		_tkn._timeOfCreation =_returnValue->_timeOfCreation;
		_tkn._nextInteger =_returnValue->_nextInteger;
		_tkn._numberOfPrimesFound =_returnValue->_numberOfPrimesFound;
		_tkn._highestPrimeFound =_returnValue->_highestPrimeFound;
	
		//printTokenDetailsAfter(_tkn);
		
		//increase loop index
		_startIndex = _startIndex + 1; 
	}
	
	//calculate elapsed time
	time(&_endTime); // stop timer
	double _elapsedTimeSeconds = difftime(_endTime, _startTime); //get elapsed time
	double _elapsedTimeMilliseconds = _elapsedTimeSeconds * 1000; //elpaed time from seconds to milliseconds
	
	//calculate hops per second
	double _hopsPerSecond = (double)_tkn._numberOfHops / _elapsedTimeSeconds;
	
	//convert token's time of create into a display able format
	char _buff[20];
	time_t _tknCreation = (time_t)_tkn._timeOfCreation;
	strftime(_buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&_tknCreation));
	
	
	
	printf("Client: Hops Limit Reached!!!...Now Rest from Hopping\n\n");
	printf("\n\n");

	
	printf("----------------------------------------------------\n");
	printf("Client: Other Statistics\n");
	printf("----------------------------------------------------\n");
	printf("LIMIT: %i\n",LIMIT); //display the limit used to repeatedly pass token back and forth
	printf("Token Time of Creation: %i\n", _tkn._timeOfCreation); //display token's time of creation in seconds
	printf("Token Time of Creation in format: %s\n", _buff);//display token's time of creation in YY/MM/DD- HH/mm/ss format
	printf("Hop Start Time : %i seconds\n", _startTime); //display hop start time
	printf("Hop End Time : %i seconds\n", _endTime); //display hop end time
	printf("The Elapsed Time: %f seconds\n",_elapsedTimeSeconds);//display hop elapsed time
	printf("\n\n");
	
	//print summary statistics
	printf("----------------------------------------------------\n");
	printf("Client: Summary Statistics\n");
	printf("----------------------------------------------------\n");
	printf("Count of the number of hops: %i\n",_tkn._numberOfHops+1); //display number of hops, since loop starts at index 0, +1 is done to accomodate that
	printf("The number of prime numbers that were found starting from 2: %i\n",_tkn._numberOfPrimesFound); //display number of primes found
	printf("Highest Prime Number Found: %i\n",_tkn._highestPrimeFound); //display highest prime found
	printf("The Elapsed Time: %f Milliseconds\n",_elapsedTimeMilliseconds); //display elapsed time in millisconds
	printf("The number of hops(messages) per second: %f \n",_hopsPerSecond); //displays hops per seconds
	
	
	printf("\n\nClient Done, Please Remeber to Kill Server using PS and KILL\n\n");
	
	return 0;
}