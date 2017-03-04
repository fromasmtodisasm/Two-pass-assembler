
/* 
 * File:   FileRel.h
 * Author: etf
 *
 * Created on June 20, 2016, 11:59 AM
 */

#ifndef FILEREL_H
#define FILEREL_H
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include"KodoviQ.h"
#include"RelocTable.h"
#include"SymTable.h"
using namespace std;

class FileRel {
public:
    static void writeToFile(string filename, RelocTableQ& relokacije,KodoviQ& kodovi, SymTable& symT);

	static void writeErrToFile(string filename, string error);

    static bool readFile(string filename, deque<string>& lines);
    //^procita sav kod u lines vektor, tako da je jedan element vektora jedna linija koda(ni jedna labela ne stoji sama)

    static bool checkOnlyLabel(string line);
    //^da proveri je li u pitanju samo labela u tom redu. ako jeste, mora jos jedan red da procita

private:
    FileRel(const FileRel& orig);
	FileRel(); //treba li mi uopste konstruktor? necu imati objekte
	virtual ~FileRel();
};

#endif /* FILEREL_H */

