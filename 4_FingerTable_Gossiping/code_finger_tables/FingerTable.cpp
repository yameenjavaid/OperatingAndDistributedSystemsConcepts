#include "FingerTable.h"
#include <stdio.h>
#include <vector>
#include <iterator>


FingerTable::FingerTable(){
    _nodeId = 0;
    _next = NULL;
}

FingerTable::FingerTable(int _id){
    _nodeId = _id;
    _next = NULL;
}

FingerTable::FingerTable(int _id, vector< pair<int,int> > _tbl){
    _nodeId = _id;
    _table = _tbl;
    _next = NULL;
}

FingerTable::FingerTable(int _id, vector< pair<int,int> > _tbl, FingerTable* _link){
    _nodeId = _id;
    _table = _tbl;
    _next = _link;
}


void FingerTable::setNodeId(int _id){
    _nodeId = _id;
}

void FingerTable::setTable(vector< pair<int,int> > _tbl){
    _table = _tbl;
}

void FingerTable::setNextFingerTable(FingerTable* _link){
    _next = _link;
}



int FingerTable::getNodeId() const{
    return _nodeId;
}

vector< pair<int,int> > FingerTable::getTable() const{
    return _table;
}
            
FingerTable* FingerTable::getNext() const{
    return _next;
}


void FingerTable::displayFingerTable(){
    
    std::vector< pair<int,int> >::iterator _ptr;

    printf("Node: %i\n",_nodeId);
    
     printf("---------------------\n");
    for(_ptr = _table.begin(); _ptr < _table.end(); _ptr++){
        printf("%i   |  %i\n", (*_ptr).first, (*_ptr).second);
       
    }
    printf("---------------------\n");
    printf("\n");
}