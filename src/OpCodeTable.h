/* 
 * File:   OpCodeTable.h
 * Author: etf
 *
 * Created on May 24, 2016, 8:30 AM
 */

#ifndef OPCODETABLE_H
#define OPCODETABLE_H
#include <string>
#include<map>
#include"OpUlaz.h"
using namespace std;

class OpCodeTable {
public:
    OpCodeTable();
    ~OpCodeTable();

    bool checkOpTable(string name) {
        if (optabela.find(name) == optabela.end()) {
            return false;
        }
        return true;
    }

    bool checkCondTable(string name) {
        if (condtabela.find(name) == condtabela.end()) {
            return false;
        }
        return true;
    }

    long getOPCode(string name) {
		return optabela[name]->getKod();
        //za slucaj kada nema uslova, u prva 3 bita se nalazi 110 tj 6
    }

	long getOPCode(string name, string cond);

	bool checkRegPermitted(string name, string reg);//vraca true ako je dozvoljeno

   
private:

    OpCodeTable(const OpCodeTable& orig) {
    };
    
	map <string, OpUlaz*> optabela; 
    map <string, unsigned short> condtabela;
};

#endif /* OPCODETABLE_H */

