/* 
 * File:   rad.h
 * Author: etf
 *
 * Created on May 25, 2016, 3:23 AM
 */

#ifndef RAD_H
#define RAD_H
#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <sstream>

#include "SymTable.h"
#include "OpCodeTable.h"
#include "RelocTable.h"
#include "KodoviQ.h"
#include "FileRel.h"

typedef deque<string>::iterator LineIterator;
typedef vector<string>::iterator VIterator;

using namespace std;

class rad {
public:
    rad();

    virtual ~rad();

	void incCnt(int inc) {
		loccnt += inc;
	}

    void resetCnt() {
        loccnt = 0;
    }

    int getCnt() {
        return loccnt;
    }

    string getSek() {
        return secCurr;
    }

    void setSek(string sek) {
        secCurr = sek;
    }

    void resetSek() {
        secCurr = "UND";
    }

    void Tokenize(const string& str, vector<string>& tokens, const string& delimiters=" ");
    //^da podeli liniju na reci-svaki clan vektora je jedna rec(pomocu pretrage razmaka)

	void priprema();
    void prolazI();
    void prolazII(); //ne treba mi prosledjivanje parametra al ajd nek bude tu
    bool obaProlaza(string filenamein, string filenameout);
    
    bool checkLine(vector<string>& parcad, SymUlaz& ulaz);
    //proverava ispravnost komande 

    bool checkIfStringIsInt(const string& s);

	void dealWithInstr(string name, vector<string>& parcad, string& kod, string& relok, string uslov);
    void dealWithImmed(string& deo, unsigned long& opcode, string& kod);
	void dealWithReg(string& deo, string& name, unsigned long& reg);
	void PcRelCall(string& deo, unsigned long& opcode, string& relok, string& kod);

	void saveTables(string& kod, string& relok);//pohranjuje podatke trenutne sekcije

	void ldcSummon16s(string& name, vector<string>& parcad, unsigned long& vrsimbola, string uslov, string& kod, string& relok);
	void ldcAbs16Rel(vector<string>& parcad, string& relok, string tiprel, unsigned long& vrsimbola);

	void word1(vector<string>& parcad);
	void word2(vector<string>& parcad, string& kod, string& relok);
	void long1(vector<string>& parcad);
	void long2(vector<string>& parcad, string& kod, string& relok);
	void char1(vector<string>& parcad);
	void char2(vector<string>& parcad, string& kod);
	void align1(vector<string>& parcad, LineIterator& lit);
	void align2(vector<string>& parcad, string& kod);
	void skip2(vector<string>& parcad, string& kod);

    void setFilename(string filename);
	string getFilename();

	void littleEndian(unsigned long& code, int inc);
	string decToHex(unsigned long& code, int inc);
	void insertBreaks(string& str);

private:
    rad(const rad& orig); //ne treba mi


     string secCurr = "UND"; //trenutna sekcija, dok je UND, mocice da se koriste public,extern
     int loccnt = 0; //location counter

     deque<string> lines; //sadrzi sve linije koda
     
     
     SymTable simboli;
     OpCodeTable komande;
     RelocTableQ relokacije;
     KodoviQ kodovi;
     
	 bool status0 = 1;
     bool status1 = 1; //promenimo na 0 ako je prolazak kroz instrukcije zavrsen sa .end(uspesno)
     bool status2 = 1;//drugi prolaz
     string sekbinKom = "";
     string INfilename;//ime fajla iz kog citamo
	 string OUTfilename;



};

#endif /* RAD_H */

