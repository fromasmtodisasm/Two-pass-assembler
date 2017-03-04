/* 
 * File:   SymUlaz.cpp
 * Author: etf
 * 
 * Created on May 24, 2016, 7:50 AM
 */

#include "SymUlaz.h"

SymUlaz::SymUlaz(string n, string t, long of, string l, string vel) {
	naziv = n;
	Sek = t;
	offset = of;
	lg = l;
	velicina = vel;
}

SymUlaz::SymUlaz(const SymUlaz& orig) {
}

SymUlaz::~SymUlaz() {
}

void SymUlaz::SetRdbr(int rdbr) {
    this->rdbr = rdbr;
}

int SymUlaz::GetRdbr() const {
    return rdbr;
}

void SymUlaz::SetLg(char lg) {
    this->lg = lg;
}

string SymUlaz::GetLg() const {
    return lg;
}

void SymUlaz::SetOffset(long offset) {
    this->offset = offset;
}

long SymUlaz::GetOffset() const {
    return offset;
}

void SymUlaz::SetSek(string tipSek) {
    this->Sek = tipSek;
}

string SymUlaz::GetSek() const {
    return Sek;
}

void SymUlaz::SetNaziv(string naziv) {
    this->naziv = naziv;
}

string SymUlaz::GetNaziv() const {
    return naziv;
}

void SymUlaz::SetVelicina(string vel) {
	this->velicina = vel;
}

string SymUlaz::GetVelicina() const {
	return velicina;
}

string SymUlaz::ispis() {//setw(20) left
	string ret = "";
	ret+=GetNaziv()+" \t"+GetSek()+" \t"+
		""+to_string(GetOffset())+" \t"+GetLg()+" \t"+
		""+to_string(GetRdbr())+"\t"+ GetVelicina()+"";
	return ret;
}

ostream& operator<<(ostream& out, SymUlaz& symU) {
	
	out << std::setw(10) << left << symU.GetNaziv();
	out << std::setw(10) << left << symU.GetSek();
	out << std::setw(10) << left << to_string(symU.GetOffset());
	out << std::setw(10) << left << symU.GetLg();
	out << std::setw(10) << left << to_string(symU.GetRdbr());
	out << std::setw(10) << left << symU.GetVelicina();
	out << "\n";
	
	return out;
}