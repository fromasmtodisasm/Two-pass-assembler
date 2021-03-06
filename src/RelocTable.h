/* 
 * File:   RelocTable.h
 * Author: etf
 *
 * Created on May 29, 2016, 11:10 AM
 */

#ifndef RELOCTABLE_H
#define RELOCTABLE_H

#include <deque>
#include <iostream>
#include <string>
using namespace std;
typedef deque<string>::iterator QIterator;

class RelocTableQ {
public:
    RelocTableQ();
   
    virtual ~RelocTableQ();
    
    void put(string ulaz){
        RelQue.push_back(ulaz);
    }
    string get(){
        string ret= RelQue.front();
        RelQue.pop_front();
        return ret;
    }
     size_t getQSize()const {
        return  RelQue.size();
    }
	 QIterator getQBegin(){
        return  RelQue.begin();
    }
	 QIterator getQEnd(){
        return  RelQue.end();
    }
    
	 

	 string ispisi();
	
private:
	RelocTableQ(const RelocTableQ& orig);
    deque <string> RelQue; 
};

#endif /* RELOCTABLE_H */

