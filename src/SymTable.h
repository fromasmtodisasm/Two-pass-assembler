/* 
 * File:   SymTable.h
 * Author: etf
 *
 * Created on May 24, 2016, 7:22 AM
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <string>
#include <iostream>
#include <iomanip>
#include <list>

#include "SymUlaz.h"
using namespace std;
	typedef list<SymUlaz*>::iterator LstIterator;
    
class SymTable {
public:
	SymTable();
    ~SymTable();

	bool checkSymInTable(string name);//false ako simbol nije u tabeli

	void putSymInTable(SymUlaz* sym);


	SymUlaz* getSymFromTable(string name);//nullptr ako nije nasao

	friend ostream& operator<<(ostream& out, SymTable& symT);

	string ispisi();

    size_t getTableSize() {
        return tabela.size();
    }
	LstIterator getTableBegin(){
        return tabela.begin();
    }
	LstIterator getTableEnd(){
        return tabela.end();
    }

	string getSekRdbr(string sekname);

private:
	static int BR;

	SymTable(const SymTable& orig);

    list <SymUlaz*> tabela;

};

#endif /* SYMTABLE_H */

