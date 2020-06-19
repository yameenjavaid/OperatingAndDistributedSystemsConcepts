/*
 * @author Swapnil Acharya
 * @since 03/21/2020
 */

#ifndef _FINGERTABLE_H_
#define _FINGERTABLE_H_


#include <vector>
using namespace std;

class FingerTable{

    public:
        FingerTable(); //default constructor	 
        FingerTable(int _id); //constructor that allows id
        FingerTable(int _id, vector< pair<int,int> > _tbl); //constructor that sets  node is, and finget table
        FingerTable(int _id, vector< pair<int,int> > _tbl, FingerTable* _link);

        //setters
        void setNodeId(int _id); //method to set node id
        void setTable(vector< pair<int,int> > _tbl); //sset finger table node
        void setNextFingerTable(FingerTable* _link); //set next node address in finger table

        //getters
        int getNodeId() const; //get node IDs of the node
        vector< pair<int,int> > getTable() const; //get the finger table for this node
        FingerTable* getNext() const; //get the next feild table address

        void displayFingerTable();  //display the finget table for this node
        
    private:
        int _nodeId; //feild to hold id of a node
        vector< pair<int,int> > _table; //feild to hold feinger tble for node
        FingerTable* _next; //filed to hold next addes of the feilder table
};

#endif
