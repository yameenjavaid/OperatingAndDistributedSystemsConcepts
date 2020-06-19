/*
 * @Author Swapnil Acharya
 * @since 4/4/2020
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/*
 * Struct to minit a node is epidemic protocol
 * Stuct constains follwing feilds
 * _nodeID: to store id of a node
 * _isInfected: boolean variable to indicated whtere the node is infected or not
 * _isRemoved: boolean variable to indicated where this node can spread infection or not
 */
typedef struct{
	int _nodeId;
	bool _isInfected;
	bool _isRemoved;
} Node;


#define MAX_NODES	1000000 //total nodes used for this simulaton
#define MAX_ROUNDS	1000 //total rounds useddd in this simulation


double PSTOP[5] = {0.2,0.4,0.6,0.8,1.00}; //probalbities where a node will stop spreading infection

/*
 * This function returns the count of infected nodes in a given nodeList
 * @ param _nodeList List of Nodes
 * @ return count of susceptible nodes OR nodes that have not been infected
 */
int getNumberOfIgnorantNodes(Node*  _nodeList){
	int _count = 0;
	int i =0;
	for(i = 0; i < MAX_NODES; i++){

		//if the node is infected then increment the count
		if(  (*(_nodeList+ i))._isInfected == false ){
			_count = _count + 1;
		}
	}	
	return _count; //return the count of infected nodes
}


/* This function initialized list of nodes 
 * @param _nodeList List of nodes 
 * @return true if the inifialization is sucessfull
 */
bool initializeNodes(Node*  _nodeList){
	int i = 0;
	for(i = 0; i < MAX_NODES;i++){
		(*(_nodeList+i))._nodeId = i; //give nodeid value of node index
		(*(_nodeList+i))._isInfected = false; //initialilly set node as susceptible
		(*(_nodeList+i))._isRemoved = false; //initially set node so that it can continue to gossip
	}
	
	srand(time(0)); //seed random to be current time
	int _infectToIndex = rand() % MAX_NODES; //get a random index 
	(*(_nodeList+_infectToIndex))._isInfected = true; //infect the randomly selected node

	return true;
}



int main(int _argc, char *_argv[]){

	//create Nodes by dynaically allocation
	Node* _nodeList = (Node *)malloc(MAX_NODES * sizeof(Node));

	//seed random
	srand(time(0));
	
	//start infection by rumor spreading
	printf("\n\nStarted Demonstration of Epidemic Protcol,\n by spreading disease via Rumor Spreading\n\n");	

	printf("Total Nodes: %i Total ROunds: %i\n\n",MAX_NODES,MAX_ROUNDS);
	
	int i = 0;
	int j = 0;
	int k = 0;
	float _prob = 0.00000;
	double _fractionIgnorant = 0.0000;
	int _ignorantNodes = 0;
	int _indexToInfect = 0;	

	//times to find run time
	time_t _startTime, _endTime;
	time(&_startTime); //get current time
	
	for(k = 0; k < 5; k++){ //oop for PSTOP values
		
		initializeNodes(_nodeList); //intialize list of nodes
		
		for(i=0; i < MAX_ROUNDS; i++){ //do for maxmum number of rounds
			
			//EVERY NODE SELCTS RANDOM NODE TO EXCHANGE INFO
			for(j = 0; j < MAX_NODES; j++){ 
				
				if( (  (*(_nodeList+j))._isInfected == true) && ( (*(_nodeList+j))._isRemoved == false ) ){
				
					_indexToInfect = rand() % MAX_NODES; //select a random index for node to infect
					
					//if a radomlyy selected node is not infected, then infec the node
					if( (*(_nodeList+_indexToInfect))._isInfected == false){
						(*(_nodeList + _indexToInfect))._isInfected = true; 
					}
					//else if a node is already infected then calculate probability that it will stop spreading rumour  
					else{
						_prob = (double) (rand() % MAX_NODES) / (double) MAX_NODES; //get a probability betwwen 0 and 1
						if(  _prob < PSTOP[k] ) {  //if the calcualted probility is less thatn the given probability
							(*(_nodeList + j))._isRemoved = true;  //then stop the node from spreading rumors
						}
					}
				}

			}
		}
		
		_ignorantNodes = getNumberOfIgnorantNodes(_nodeList); //get number of suceptible nodes
		_fractionIgnorant = (double)_ignorantNodes/(double)MAX_NODES; //get the faction os suceptible nodes
		printf("PSTOP: %0.2f | S: %f | IgnorantNodes: %i\n",PSTOP[k],_fractionIgnorant,_ignorantNodes); //display the results
	}	

	//display elapsed time
	time(&_endTime);
	double _elapsedTime = difftime(_endTime,_startTime);
	printf("ElapedTIme: %0.3f seconds\n",_elapsedTime);	

	//free memory allocated in heap for Node List
	free(_nodeList);
	
	printf("\nDesmostration of Epidemic Protocol  Complete\n\n");

	return 0;
}
