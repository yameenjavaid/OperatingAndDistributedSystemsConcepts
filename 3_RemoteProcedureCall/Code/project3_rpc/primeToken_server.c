/**
*@since 3/1/2020
*@file primeToken_server.c
*This file demonstrates the server part of RPC based client-server communication
**/

#include "primeToken.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
* This function prints the details of token 
*@param _tkn The token structure whose detail is to be printed
*@return nothing
*/
void printTokenDetails(Token _tkn){
	printf("\n\n Token In Server\n");
	printf("Server:-- Hops: %i\n",_tkn._numberOfHops);
	printf("Server:-- Time: %i\n",_tkn._timeOfCreation);
	printf("Server:-- Next Int: %i\n",_tkn._nextInteger);
	printf("Server:-- Num Prime: %i\n",_tkn._numberOfPrimesFound);
	printf("Server:-- Highest: %i\n",_tkn._highestPrimeFound);
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
		
       if(_num % i == 0) { //if number is divisible break
          _flag = false;
          break;
       }
    }
    return _flag;
}


Token * start_prime_computation_1_svc(Token * _token, struct svc_req * req){


	//It is impertative that the return identifier referenced
	//be of type type static, as opposed to local. Local identifiers
	//are allocated on stack, and a reference to their contents would be
	//invalid once the function returns -- John shapely gray ch 9 rpc
	
	//create token
	static Token _tkn;

	
	//initialize token by oopying the value sent by client to server's local token
	_tkn._numberOfHops = _token->_numberOfHops; //copy 
	_tkn._timeOfCreation = _token->_timeOfCreation;
	_tkn._nextInteger = _token->_nextInteger;
	_tkn._numberOfPrimesFound = _token->_numberOfPrimesFound;
	_tkn._highestPrimeFound = _token->_highestPrimeFound;


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
			
			//increment hop count
			_tkn._numberOfHops = _tkn._numberOfHops + 1; 
			
			//exit loop
			_isPrime = true;
		}
		else{
			
			_isPrime = false;
			
			//increment index for next number to be checked for prime
			_currentInteger = _currentInteger + 1;
		}
	}
	
	//printTokenDetails(_tkn);

	return (&_tkn); //return the local token to client
}