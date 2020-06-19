/*
 * @author Swapnil Acharya
 * @since 4/4/20
 */
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <math.h>
#include "FingerTable.h"

#define IDENTIFIER_SPACE 19

/*
 * This function return the closeset sucessor to a given node for 
 * finger table construction
 * @param _idList NodeList
 * @param _currentSucessor the nodeid whoe sucessor is to be found
 * @return return the sucessor
 */ 
int getClosetSucessor(std::vector<int> _idList, int _nodeId){
    
	
    std::vector<int>::iterator _ptr;
    _ptr = _idList.begin();
    for(_ptr = _idList.begin(); _ptr<_idList.end(); _ptr++){
        if(*_ptr > _nodeId){ 
            return *_ptr; //return a sucessor than nodeId       
	 }
    }
    return _idList.front(); //if the node is the last node list it to the first node
}

/*
 * This function generates fingertable for a given nodeId
 * @param _nodeId node if for whose finger table is to be generated
 * @param _idList list of nodeIds
 * @return fingertable
 */ 
std::vector< pair<int,int> > generateFingerTable(int _nodeId, std::vector<int> _idList){
    
    std::vector< pair<int,int> > _aTable;
    
    //generate sucessor
    int i =0;
    int _sucessor = 0;

    for(i = 1; i <= IDENTIFIER_SPACE; i++){

	//get sucessor
	//sucessor = (nodeId + 2^(i-1)) Mod 2^(m)
        _sucessor = 0;
        _sucessor = _nodeId +  (int)( pow(2,(i-1)) );
        _sucessor = _sucessor % (int)(pow(2,IDENTIFIER_SPACE));

	//if the sucessor is on the node list pussh tat node is the table
        if(binary_search(_idList.begin(), _idList.end(),_sucessor)){
            _sucessor = _sucessor;
        }
	//else get the close sucessor 
        else{
            _sucessor = getClosetSucessor(_idList,_sucessor);
        }

        _aTable.push_back(make_pair(i,_sucessor));
    }

    return _aTable; //returnt he generated finger table
}


int main(){

    std::vector<int> _ids; //vector to hold node ids

    //read file
    FILE * _fp;
    _fp = fopen("nodeIDs","r"); //open NodeIds file for read
    int _temp = 0;

    //read nodeIDs from fiel to vector
    while(!feof(_fp)){
        _temp = 0;
        fscanf(_fp,"%i",&_temp);
        _ids.push_back(_temp);
    }
    _ids.pop_back(); //remove the last endline read into vector
    fclose(_fp); //close the file pointer

    //sort vector containing node ids
    sort(_ids.begin(),_ids.end());

    //start generating finger tables
    FingerTable* _head;
    FingerTable* _currPtr;
    std::vector<int>::iterator _ptr;
    
    //generate finger table for all node ids and put then in acircular linked list
    for(_ptr = _ids.begin(); _ptr <_ids.end(); _ptr++){
        
        if(*_ptr == _ids.front()){
            std::vector< pair<int,int> > _tempTable = generateFingerTable(*_ptr,_ids);
            FingerTable* _temp = new FingerTable(*_ptr,_tempTable);
            _head = _temp;
            _currPtr = _temp;
            _temp = NULL;
        }
        else{
            std::vector< pair<int,int> > _tempTable = generateFingerTable(*_ptr,_ids);
            FingerTable* _temp = new FingerTable(*_ptr,_tempTable);
            _currPtr->setNextFingerTable(_temp);
            _currPtr = _temp;
            _temp = NULL;
        }
    }
	
	//linking last item's next to first item(head)
    _currPtr->setNextFingerTable(_head);
	_currPtr  = NULL;
	
	
    printf("Finger Table Generation Completed\n\n");

    //search for specified finger table with nid
    int _nid = 0;

    //menu based finger table
    //prmopt user for node id and print the finger table for nodeid	
    while(1){
		
		printf("Press ctrl + c to exit\n");
        printf("Finger Table For Node: ");
        scanf("%i",&_nid);
        printf("\n");
        
        _currPtr = _head;
        while(_currPtr != NULL){
            
            if(_currPtr->getNodeId() == _nid){
                _currPtr->displayFingerTable();
				FingerTable* _tempPtr = _currPtr->getNext();
				int _nextNodeId = _tempPtr->getNodeId();
				//printf("Next Node In Chain: %i\n\n",_nextNodeId);
                break;
            }
            else{
                _currPtr = _currPtr->getNext();
            }
        }

    }


    return 0;
}
