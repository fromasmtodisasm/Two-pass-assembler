/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SymTable.cpp
 * Author: etf
 * 
 * Created on May 24, 2016, 7:22 AM
 */

#include "SymTable.h"

int SymTable::BR = 0;

SymTable::SymTable() {
	SymUlaz *sym = new SymUlaz("UND", "UND", 0, "l");
	sym->rdbr = BR++;
	tabela.push_back(sym);
}

SymTable::SymTable(const SymTable& orig) {
}

SymTable::~SymTable() {
}

bool SymTable::checkSymInTable(string name) {
	for (LstIterator iter = getTableBegin(); iter != getTableEnd(); iter++) {
		if ((*iter)->GetNaziv() == name)return true;//simbol u tabeli
		else continue;
	}return false;//prosao celu listu, nije nasao poklapanje, simbol nije u tabeli
}

string SymTable::getSekRdbr(string sekname) {
	for (LstIterator iter = getTableBegin(); iter != getTableEnd(); iter++) {
		if ((*iter)->GetNaziv() == sekname)return to_string((*iter)->GetRdbr());
		else continue;
	}//prosao celu listu, nije nasao poklapanje, simbol nije u tabeli
	return "0";
}

void SymTable::putSymInTable(SymUlaz* sym) {
	sym->SetRdbr(BR++);
	tabela.push_back(sym);
}


SymUlaz* SymTable::getSymFromTable(string name) {
	for (LstIterator iter = getTableBegin(); iter != getTableEnd(); iter++) {
		if ((*iter)->GetNaziv() == name)return *iter;//simbol u tabeli
		else continue;
	}return nullptr;//prosao celu listu, nije nasao poklapanje, simbol nije u tabeli
}

string SymTable::ispisi() {
	size_t len = getTableSize();
	string ret = "===================tabela simbola========================\n";
	ret += "naziv \t tip \t ofset \t lg \t rdbr \n";

	for (LstIterator iter = getTableBegin(); iter != getTableEnd(); iter++) {

		SymUlaz* ulaz = *iter;
		ret += ulaz->ispis();
		//ret += "\n";//ne treba jer SymUlaz vec stavlja jedan
	}

	ret += "==================================================\n";
	return ret;
}

ostream& operator<<(ostream& out, SymTable& symT) {
	size_t len = symT.getTableSize();
	out<<"===================tabela simbola========================\n";
	out << std::setw(10) << left << "naziv";
	out << std::setw(10) << left << "tip";
	out << std::setw(10) << left << "offset";
	out << std::setw(10) << left << "lok/glob";
	out << std::setw(10) << left << "rdbr";
	out << std::setw(10) << left << "velicina sekcije";
	out << "\n";
	for (LstIterator iter = symT.getTableBegin(); iter != symT.getTableEnd(); iter++) {

		SymUlaz* ulaz = *iter;
		out<<*ulaz;
		//out << "\n";//ne treba jer SymUlaz vec stavlja jedan
	}

	out<< "==================================================\n";
	return out;
}