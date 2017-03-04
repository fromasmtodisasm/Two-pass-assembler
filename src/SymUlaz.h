/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SymUlaz.h
 * Author: etf
 *
 * Created on May 24, 2016, 7:50 AM
 */

#ifndef SYMULAZ_H
#define SYMULAZ_H
#include <string>
#include <iomanip>
#include<iostream>
using namespace std;

class SymUlaz {
public:
     friend class SymTable;
	 SymUlaz(string n, string t, long of, string l,string vel="/");
   
    virtual ~SymUlaz();
    
    
    int GetRdbr() const;
    void SetLg(char lg);
    string GetLg() const;
    void SetOffset(long offset);
    long GetOffset() const;
    void SetSek(string tipSek);
    string GetSek() const;
    void SetNaziv(string naziv);
    string GetNaziv() const;
	void SetVelicina(string vel);
	string GetVelicina() const;
	friend ostream& operator<<(ostream& out, SymUlaz& symU);
	string ispis();

	

private:
	SymUlaz(const SymUlaz& orig);
	SymUlaz() {};//MISLIM DA PO LOGICI ZADATKA NE BIH SMELA DA IMAM OVAJ KONSTRUKTOR
	
    string naziv;
    string Sek;//naziv sekcije
    long offset;//u odnosu na pocetak sekcije
    string lg;//lokali/globalni
    int rdbr;//u tabeli
	string velicina;//samo za sekciju
    //samo pri dodavanju u tabelu simbola moze da se menja od strane tabele jer samo ona ima BR
    void SetRdbr(int rdbr);
};

#endif /* SYMULAZ_H */

