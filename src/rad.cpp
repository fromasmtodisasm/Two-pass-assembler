/*
 * File:   rad.cpp
 * Author: etf
 *
 * Created on May 25, 2016, 3:23 AM
 */

#include "rad.h"

rad::rad() {
}

void rad::setFilename(string filename) {
	this->INfilename = filename;
}

string rad::getFilename() {
	return INfilename;
}

rad::rad(const rad& orig) {
}

rad::~rad() {
}


void rad::dealWithImmed(string& deo, unsigned long& opcode, string& kod) {
	unsigned long immed = std::stoi(deo);
	opcode = opcode | immed;
	//littleEndian(opcode, 4);
	kod = kod + decToHex(opcode, 4) + " ";
}
void rad::dealWithReg(string& deo, string& name, unsigned long& reg) {//name je ime instrukcije
	if (deo.at(0) != 'r')FileRel::writeErrToFile(OUTfilename, "mora biti registar");
	size_t poz = deo.find("r") + 1;
	reg = std::stoi(deo.substr(poz)); //skidam mu r od rX,x je broj
	if (reg < 0 || reg > 19) {//ne znam ni kako moze da bude <0, al ajde
		FileRel::writeErrToFile(OUTfilename, "taj registar " + deo + "ne postoji");
	}
	if (!komande.checkRegPermitted(name, deo))FileRel::writeErrToFile(OUTfilename, "mora biti registar");
	//treba li da proverim nesto kod immed?u kojoj formi je zapisan immed?
}
void rad::PcRelCall(string& deo, unsigned long& opcode, string& relok, string& kod) {
	if (!simboli.checkSymInTable(deo))FileRel::writeErrToFile(OUTfilename, "nepostojeci simbol");
	SymUlaz* simbol = simboli.getSymFromTable(deo);
	long off;
	int radi = 1;
	string refrdbr;
	if (simbol->GetLg() == "g") {
		off = 0;	
		refrdbr = to_string(simbol->GetRdbr());
	}
	else {//lokalni
		off = simbol->GetOffset();
		refrdbr = simboli.getSekRdbr(simbol->GetSek());
		if (simbol->GetSek() == secCurr)radi = 0;
		 //lokalni
	}					
	if(radi==1)relok = relok + to_string(getCnt() + 1) + "\t shr: 5" + "\t PCRel19 \t" + refrdbr + "\t noSign \n";
						//+2 da bi pokazivalo bas na mesto 
	off = loccnt - off;
	off = off & 0x0007ffff;//odseca na deo dozvoljen za tu instrukciju
	opcode = opcode | off;
	//littleEndian(opcode, 4);
	kod = kod + decToHex(opcode, 4) + " ";
}


void rad::dealWithInstr(string name, vector<string>& parcad, string& kod, string& relok, string uslov = "") {
	//nema potrebe proveravati da li postoji ta instrukcija, to je vec prvi prolaz uradio
	//ime je cisto ime instrukcije, dok se u parcadi nalazi sa jos uvek nalepljenim ime_uslov 

	if (name == "int") {
		if (parcad.size() != 2)FileRel::writeErrToFile(OUTfilename, "pogresan broj parametara za int");
		string deo = parcad[1];
		if (!checkIfStringIsInt(deo)) FileRel::writeErrToFile(OUTfilename, "pogresan parametar za int");
		unsigned long opcode=0;
		if (uslov == "")opcode = komande.getOPCode(name);
		else opcode = komande.getOPCode(name, uslov);

		if (!checkIfStringIsInt(deo)) FileRel::writeErrToFile(OUTfilename, "int ne dozvoljava simbole");
			//cist broj(immed)
			unsigned long immed = std::stoi(deo);
			opcode = opcode | (immed<<20);
			//littleEndian(opcode, 4);
			kod = kod + decToHex(opcode, 4) + " ";
			return;

	}
	else if (name == "add" || name == "addf" || name == "sub" ||
		name == "subf" || name == "mul" || name == "mulf" || name == "div" || name == "divf"
		|| name == "cmp" || name == "cmpf") {

		if (parcad.size() != 3)FileRel::writeErrToFile(OUTfilename, "pogresan broj parametara za arit skup instrukcija");

		unsigned long opcode;
		if (uslov == "")opcode = komande.getOPCode(name);
		else opcode = komande.getOPCode(name, uslov);

		string deo = parcad[1]; //dst reg
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		unsigned long dst = 0;
		dealWithReg(deo, name, dst);
		dst = (dst << 19);
		opcode = opcode | dst;
		deo = parcad[2]; //src ili immed
		
		//ako je reg
		unsigned long src;
		if (deo.at(0) == 'r') {
			src = std::stoi(deo.substr(deo.find("r")+1)); //skidam mu r od rX,x je broj
			if (src < 0 || src > 19) {//ne znam ni kako moze da bude <0, al ajde
				FileRel::writeErrToFile(OUTfilename, "taj registar " + deo + "ne postoji");
			}
			if (!komande.checkRegPermitted(name, deo))FileRel::writeErrToFile(OUTfilename, "nedopusten registar");
			//treba li da proverim nesto kod immed?u kojoj formi je zapisan immed?
			opcode = opcode | (src<<13);
			//littleEndian(opcode, 4);
			kod = kod + decToHex(opcode, 4) + " ";
			//za registre mi ne treba relokacija?     
			return;
		}//deo ispod ovoga se ne desava ako se javi drugi reg zbog return ovog iznad
		//immed
		opcode = opcode | 0x00040000;//oznaka da je sa simb/imed
		if (!checkIfStringIsInt(deo)) FileRel::writeErrToFile(OUTfilename, "arit instr ne dozvoljavaju simbole");
			//cist broj
			dealWithImmed(deo, opcode, kod);	

	}
	else if (name == "and" || name == "andf" || name == "or" || name == "orf" ||
		name == "not" || name == "notf" || name == "test" || name == "testf") {

		if (parcad.size() != 3)FileRel::writeErrToFile(OUTfilename, "pogresan broj parametara za logicki skup instr");

		unsigned long opcode;
		if (uslov == "")opcode = komande.getOPCode(name);
		else opcode = komande.getOPCode(name, uslov);

		string deo = parcad[1]; //dst reg
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		unsigned long dst;
		dealWithReg(deo, name, dst);

		deo = parcad[2]; //src reg
		unsigned long src;
		dealWithReg(deo, name, src);

		opcode = opcode | dst << 19 | src << 14;
		//littleEndian(opcode, 4);
		kod = kod + decToHex(opcode, 4) + " ";
		return;

	}
	else if (name == "ldr" || name == "str" ) {
		if (parcad.size() != 5)FileRel::writeErrToFile(OUTfilename, "pogresan broj parametara za ldc");

		unsigned long opcode;
		if (uslov == "")opcode = komande.getOPCode(name);
		else opcode = komande.getOPCode(name, uslov);

		string deo = parcad[1]; // reg a, za njega proveravamo uslove za reg
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		unsigned long dst;
		if (deo == "r19")FileRel::writeErrToFile(OUTfilename, "ldr/str ne mogu imati r19 za regA");
		dealWithReg(deo, name, dst);

		deo = parcad[2]; //reg r, ovaj moze da bude PSW
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		unsigned long src;
		dealWithReg(deo, name, src);

		deo = parcad[3];
		//immed f
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		if (!checkIfStringIsInt(deo)) FileRel::writeErrToFile(OUTfilename, "f mora biti obican broj");
		int f = std::stoi(deo);
		if (f == 1 || f > 5)FileRel::writeErrToFile(OUTfilename, "nedozvoljena vrednost za f");

		opcode = opcode | dst << 19 | src << 14 | f << 11;

		deo = parcad[4];
		//immed ili simbol
		//immed
		if (checkIfStringIsInt(deo)) {//cist broj
			dealWithImmed(deo, opcode, kod);
			return;
		}
		//simbol

		/*dealWithSymb(deo, opcode, relok, kod,0x000003ff);//odsecanje 18 bita 
		//littleEndian(opcode, 4);
		kod = kod + decToHex(opcode, 4) + " ";
		return;*/
	}
	else if (name == "call") {
		if (parcad.size() != 3)FileRel::writeErrToFile(OUTfilename, "pogresan broj parametara za ldc");

		unsigned long opcode;
		if (uslov == "")opcode = komande.getOPCode(name);
		else opcode = komande.getOPCode(name, uslov);

		string deo = parcad[1]; //dst reg
		unsigned long dst;
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		dealWithReg(deo, name, dst);
		opcode = opcode | dst << 19;

		deo = parcad[2]; //immed
		if (checkIfStringIsInt(deo)) {//cist broj
			dealWithImmed(deo, opcode, kod);
			return;
		}
		else {
			//simbol
			PcRelCall(deo, opcode, relok, kod);
			return;
		}
	}
	else if (name == "in"  || name == "out" ) {
		if (parcad.size() != 3)FileRel::writeErrToFile(OUTfilename, "pogresan broj parametara za in/out");

		unsigned long opcode;
		if (uslov == "")opcode = komande.getOPCode(name);
		else opcode = komande.getOPCode(name, uslov);

		string deo = parcad[1]; //dst reg
		unsigned long dst;
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		dealWithReg(deo, name, dst);

		deo = parcad[2]; //SRC REG
		unsigned long src;
		dealWithReg(deo, name, src);


		opcode = opcode | dst << 20 | src << 16;
		//littleEndian(opcode, 4);
		kod = kod + decToHex(opcode, 4) + " ";
		return;

	}
	else if (name == "mov" || name == "movf" || name == "shl" || name == "shlf" ||
		name == "shr" || name == "shrf") {
		if (parcad.size() < 3)FileRel::writeErrToFile(OUTfilename, "pogresan broj parametara za ldc");
		if (parcad.size() > 4) FileRel::writeErrToFile(OUTfilename, "mov/shl/shr/f ima previse parametara");

		unsigned long opcode;
		if (uslov == "")opcode = komande.getOPCode(name);
		else opcode = komande.getOPCode(name, uslov);

		string deo = parcad[1]; //dst reg
		unsigned long dst;
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		dealWithReg(deo, name, dst);

		deo = parcad[2]; //src reg
		unsigned long src;
		if (name != "mov" || name != "movf")deo = deo.substr(0, deo.find(",") - 0);//skida zarez,mov ima jedan manje parametar pa nema zareza
		dealWithReg(deo, name, src);

		opcode = opcode | dst << 19 | src << 14;
		if (name == "mov" || name == "movf") {
			//littleEndian(opcode, 4);
			kod = kod + decToHex(opcode, 4) + " ";
			return;
		}
		//shiftovi samo:
		deo = parcad[3];
		//immed
		if (!checkIfStringIsInt(deo)) FileRel::writeErrToFile(OUTfilename, "mov/shr/shl ne podrzavaju simbole");
			//cist broj
			unsigned long immed = std::stoi(deo);
			opcode = opcode | (immed<<9);
			//littleEndian(opcode, 4);
			kod = kod + decToHex(opcode, 4) + " ";
			return;
		//imed vec ima u sebi ugradjen concat

	}
	else if (name == "ldch" || name == "ldcl") {
		if (parcad.size() != 3)FileRel::writeErrToFile(OUTfilename, "pogresan broj parametara za ldch/l");

		unsigned long opcode;
		if (uslov == "")opcode = komande.getOPCode(name);
		else opcode = komande.getOPCode(name, uslov);

		string deo = parcad[1]; //DST REG
		unsigned long dst;
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		dealWithReg(deo, name, dst);
		opcode = opcode | dst << 20 ;
		
		deo = parcad[2];
		//IMMED
		if (checkIfStringIsInt(deo)) {//cist broj
			if (name == "ldch") { 
				unsigned long d = std::stol(deo);
				d = d >> 16;
				deo = to_string(d);
			}
			else {
				unsigned long d = std::stol(deo);
				d = d << 16;
				d = d >> 16;
				deo = to_string(d);
			}
			dealWithImmed(deo, opcode, kod);//dodaje opcode, relok nije potreban
			return;
		}
		//SIMBOL, ne desava se ako je pozvan iz ldc
		unsigned long vrsimbola;//izmenice ga kad vidi da li je l/g
		string tiprel = "Abs16H";
		if (name == "ldcl")tiprel = "Abs16L";
		ldcAbs16Rel(parcad, relok, tiprel, vrsimbola);//dodaje relokaciju ali ne i opcode
		if (name == "ldch") {		
			vrsimbola = vrsimbola >> 16;	
		}
		else {
			vrsimbola = vrsimbola << 16;
			vrsimbola = vrsimbola >> 16;
		}
		opcode = opcode | vrsimbola;
		//littleEndian(opcode, 4);
		kod = kod + decToHex(opcode, 4) + " ";
		return;
	}
	else if (name == "ldc") {
		if (parcad.size() != 3)FileRel::writeErrToFile(OUTfilename, "pogresan broj parametara za ldc");
		unsigned long vrsimbola;
		if (checkIfStringIsInt(parcad[2])) {//cist broj
			unsigned long vrsimbola = stol(parcad[2]);
			name = "ldch";
			parcad[2] = to_string(vrsimbola >> 16);
			dealWithInstr(name, parcad, kod, relok, uslov);
			name = "ldcl";
			vrsimbola = vrsimbola << 16;//odstranjuje gornjih 16b
			parcad[2] = to_string(vrsimbola >> 16);//priprema donjih 16b
			dealWithInstr(name, parcad, kod, relok, uslov);
			incCnt(8);
			return;//ne treba relokacija jer nije simbol,u kod ce ga upisati ldch i ldcl
		}//simbol
		else {//posto moramo da odsecemo c, opet ce ga ldcl i ldch gledati kao immed, pa ovde moram da stvorim relokacije
				ldcAbs16Rel(parcad, relok, "Abs16H", vrsimbola);
				incCnt(4);//priprema za naredni poziv podinstrukcije
				ldcAbs16Rel(parcad, relok, "Abs16L",vrsimbola);
				ldcSummon16s(name, parcad, vrsimbola, uslov, kod, relok);
				incCnt(4);

			}

		}

}
void rad::ldcSummon16s(string& name, vector<string>& parcad,unsigned long& vrsimbola,string uslov, string& kod, string& relok) {
	name = "ldch";
	parcad[2] = to_string(vrsimbola >> 16);
	dealWithInstr(name, parcad, kod, relok, uslov);
	name = "ldcl";
	vrsimbola = vrsimbola << 16;//odstranjuje gornjih 16b
	parcad[2] = to_string(vrsimbola >> 16);//priprema donjih 16b
	dealWithInstr(name, parcad, kod, relok, uslov);
}
void rad::ldcAbs16Rel(vector<string>& parcad, string& relok, string tiprel, unsigned long& vrsimbola) {
	if (!simboli.checkSymInTable(parcad[2]))FileRel::writeErrToFile(OUTfilename, "nepostojeci simbol");
	SymUlaz* ulaz = simboli.getSymFromTable(parcad[2]);
	//aps pa moze da pristupi simbolu iz bilo koje sekcije,pa i ext i publ i lok
	string ref = "";
	vrsimbola = 0;//za globalne(public/ext), sluzi za 
	
	if (ulaz->GetLg() == "l") {
		vrsimbola = ulaz->GetOffset();
		ref = simboli.getSekRdbr(ulaz->GetSek());//lokalni, referisemo sekciju
	}
	else ref = to_string(ulaz->GetRdbr());//globalni, referisemo njega samog

	string offcnt = to_string(getCnt() + 2);//+2 da bi pokazivalo gde je smesten tacno
	relok = relok + offcnt + "\t shr: 0" + "\t" + tiprel + "\t" + ref + "\t" + "\n";
}


void rad::priprema() {
	for (LineIterator Lit = lines.begin(); Lit != lines.end(); Lit++) {//prolazi sve linije
		vector<string> parcad; //Lit++ na 2 mesta zbog onog Lit-- koje ne moze
		Tokenize(*Lit, parcad); //izdelio liniju pomocu njenih razmaka
		string deo0;

		deo0 = parcad[0];
		if (deo0.find(".") == string::npos) continue;
		if (deo0.compare(".end") == 0) {
			if (parcad.size() > 1)FileRel::writeErrToFile(OUTfilename, "nedozvoljen sadrzaj nakon .end");
			status0 = 0; //zavrseno sa end,pravilno
			return; //KRAJ KODA,izlazimo iz petlje svih linija
		}
		size_t poz1 = deo0.find_first_of(".");
		size_t poz2 = deo0.find_last_of(".");
		string simb;
		simb = deo0.substr(poz2 + 1); //ime podsekcije(otfikarice i tacku)
		if (poz1 != poz2) {//znamo da ima bar 1 tacka. ako se ova 2 poklope, zaista postoji samo jedna tacka,nema podnaziva
			deo0 = deo0.substr(0, poz2); //ime sekcije
		}
		if (deo0 == ".text" || deo0 == ".data" || deo0 == ".bss") {//pocinje sekcija
			if (parcad.size() > 1)FileRel::writeErrToFile(OUTfilename, "nedozvoljen sadrzaj nakon deklaracije sekcije");
			if (simboli.checkSymInTable(simb) == true)FileRel::writeErrToFile(OUTfilename, "dvostruka deklaracija sekcije simbola");
			SymUlaz* ulaz = new SymUlaz(simb, deo0, 0, "l");
			simboli.putSymInTable(ulaz);


		}
	}

}

void rad::prolazI() {

	for (LineIterator Lit = lines.begin(); Lit != lines.end(); ) {//prolazi sve linije
		vector<string> parcad; //Lit++ na 2 mesta zbog onog Lit-- koje ne moze
		Tokenize(*Lit, parcad); //izdelio liniju pomocu njenih razmaka
		string deo0;

		deo0 = parcad[0];


		if (deo0 == ".end") {
			SymUlaz* ulaz = simboli.getSymFromTable(secCurr);
			ulaz->SetVelicina(to_string(loccnt));

			status1 = 0; //zavrseno sa end,pravilno
			return; //KRAJ KODA,izlazimo iz petlje svih linija
		}

		//pocinje nova sekcija(ne zavisi od UND)
		if (deo0.find(".text") != string::npos || deo0.find(".data") != string::npos || deo0.find(".bss") != string::npos) {//ima bar jedna tacka
			size_t poz1 = deo0.find_first_of(".");
			size_t poz2 = deo0.find_last_of(".");
			string simb;
			simb = deo0.substr(poz2 + 1); //ime podsekcije(otfikarice i tacku)

			if (poz1 != poz2) {//znamo da ima bar 1 tacka. ako se ova 2 poklope, zaista postoji samo jedna tacka
				deo0 = deo0.substr(0, poz2); //ime sekcije
			}
			if (deo0 == ".text" || deo0 == ".data" || deo0 == ".bss") {//pocinje sekcija

				SymUlaz* ulaz = simboli.getSymFromTable(secCurr);
				ulaz->SetVelicina(to_string(loccnt));
				setSek(simb);
				resetCnt();

				Lit++;
				continue;
			}
			else { FileRel::writeErrToFile(OUTfilename, "neispravan format naziva sekcije"); }
		}

		if (deo0 == ".public" || deo0 == ".extern") { //zavisi od UND 
			if (getSek() != "UND") FileRel::writeErrToFile(OUTfilename, "ne sme vise public i extern, sekcije su pocele"); 

			for (int iter = 1; iter != parcad.size(); iter++) {
				string simb = parcad[iter];
				simb = simb.substr(0, simb.find(",") - 0);
				//na pocetku je tabela prazna, ali svejedno treba da proverimo da li 2 puta deklarise nesto public ili extern
				if (simboli.checkSymInTable(simb) == true)FileRel::writeErrToFile(OUTfilename, "dvostruka deklaracija javnog/ext simbola");
				SymUlaz *ulaz = new SymUlaz(simb, "UND", 0, "g");
				simboli.putSymInTable(ulaz);
			}

			Lit = lines.erase(Lit); //priprema za II prolaz, njemu nece trebati public i extern direktive
			continue;

		}

		if (getSek() == "UND")FileRel::writeErrToFile(OUTfilename, "direktive poput long itd, i komande obicne ne smeju u UND");
		string::size_type test = deo0.find(":", 0);
		if (string::npos != test) {//ako imamo labelu

			test = deo0.find_first_not_of(" ", test + 1); //da li je pravilno napisana labela(to parce bi trebalo da se zavrsava dvotackom)
			if (string::npos != test) FileRel::writeErrToFile(OUTfilename, "lose napisana labela!");
			size_t lok = deo0.find(":");

			deo0 = deo0.substr(0, lok); //uzeli cist naziv labele, bez dvotacke
			if (simboli.checkSymInTable(deo0) == true) {//postoji u tabeli, dakle globalan je
				SymUlaz* refUlaz = simboli.getSymFromTable(deo0);
				refUlaz->SetOffset(getCnt());
				refUlaz->SetSek(getSek());
			}
			else {//novi simbol
				SymUlaz* ulaz = new SymUlaz(deo0, getSek(), getCnt(), "l");
				simboli.putSymInTable(ulaz);
			}
			string deo1 = parcad[1]; //dohvatamo komandu nakon labele
			deo0 = deo1; //priprema za obradu komande
			if (deo0 == ".align" || deo0 == ".skip" || deo0.find(".bss")!=string::npos 
				|| deo0.find(".data") != string::npos || deo0.find(".text") != string::npos)FileRel::writeErrToFile(OUTfilename, "ne moze labela uz " + deo0 + "");
						 //ovo ispod je priprema za II prolaz, njemu ne trebaju labele
			string newline = *Lit;
			size_t krajrazmaka = newline.find_first_not_of(" ", newline.find(":") + 1);
			newline = newline.substr(krajrazmaka, (*Lit).size()); //pokupi sve od razmaka koji je nakon : pa do kraja
			*Lit = newline;
			vector<string>::iterator vit = parcad.begin();//MORA ITERATOR, ne moze int
			parcad.erase(vit);
		}
		if (deo0.find(".") != string::npos) {
			if (deo0 == ".long") {
				long1(parcad);
				Lit++;
				continue;
			}
			else if (deo0 == ".word") {
				word1(parcad);
				Lit++;
				continue;
			}
			else if (deo0 == ".char") {
				char1(parcad);
				Lit++;
				continue;
			}
			else if (deo0 == ".skip") {
				if (parcad.size() < 2)FileRel::writeErrToFile(OUTfilename, "nedovoljno parametara za word/char");
				if (parcad.size() > 3)FileRel::writeErrToFile(OUTfilename, "skip komanda ima previse parametara");
				std::string pom = parcad[1];
				pom = pom.substr(0, pom.find(",") - 0);
				if (!checkIfStringIsInt(pom))FileRel::writeErrToFile(OUTfilename, "nevalidni parametri skip komande");
				int inc = std::stoi(pom, nullptr, 10);
				incCnt(inc);
				Lit++;
				continue;
			}
			else if (deo0 == ".align") {// .align alignment[, [fill][, max]] 
				if (parcad.size() > 4)FileRel::writeErrToFile(OUTfilename, "previse parametara za align");
				align1(parcad, Lit);
				continue;
			}
			else {
				FileRel::writeErrToFile(OUTfilename, "od direktiva uz labelu mogu ici samo long,word,byte");
			}
		}
		else {//instrukcije obicne u pitanju
			if (deo0.find("_") != string::npos) {
				string uslov = deo0.substr((deo0.find("_") + 1)); //odvaja deo od '_' nadalje
				deo0 = deo0.substr(0, deo0.find("_") - 0);
				if (!komande.checkCondTable(uslov))FileRel::writeErrToFile(OUTfilename, "pogresan uslov komande");
			}
			if (!komande.checkOpTable(deo0))FileRel::writeErrToFile(OUTfilename, "pogresno ime komande");
			if (deo0 == "ldc")incCnt(8); //prevodi se kao 2 instrukcije
			else {
				incCnt(4);
			}//sve ostaleinstrukcije su duzine 4B
		}
		Lit++;
		continue;
	}

}

void rad::prolazII() {
	resetCnt();
	resetSek();
	string kod = "";
	string relok = "";

	for (LineIterator Lit = lines.begin(); Lit != lines.end(); Lit++) {//prolazi sve linije
		vector<string> parcad;
		Tokenize(*Lit, parcad); //izdelio liniju pomocu njenih razmaka

		string deo0 = parcad[0];

		if (deo0.find(".") != string::npos) {//DIREKTIVE u pitanju
			if (deo0 == ".end") {
				saveTables(kod, relok);//cuvamo kod i relok stare sekcije
				status2 = 0; //zavrseno sa end,pravilno, ne treba nista dopisati nigde
				return; //KRAJ KODA,izlazimo iz petlje svih linija
			}


			//tacku jos imaju sekcije i one druge direktive, a one ne smeju biti ovde
			size_t poz1 = deo0.find_first_of(".");
			size_t poz2 = deo0.find_last_of(".");
			string simb;
			simb = deo0.substr(poz2 + 1); //ime podsekcije(otfikarice i tacku)
			if (poz1 != poz2) {//znamo da ima bar 1 tacka. ako se ova 2 poklope, zaista postoji samo jedna tacka
				deo0 = deo0.substr(0, poz2); //ime sekcije
			}
			if (deo0 == ".text" || deo0 == ".data" || deo0 == ".bss") {//pocinje sekcija

				saveTables(kod, relok);//cuvamo kod i relok stare sekcije
				setSek(simb);//nova sekcija
				resetCnt();//loccnt nove sekcije je 0
				kod = "sekcija: " + simb + "\n";
				relok = "sekcija: " + simb + "\n";
				//gotovo cuvanje stare sekcije i spremanje za novu
				continue;

			}
			else if (!deo0.compare(".long")) {//mogu biti samo long,word itd.
				 //prvi prolaz vec mora da proveri ispravnost ove komande
				long2(parcad, kod, relok);
				continue;
			}
			else if (!deo0.compare(".word")) {//ne dozvoljava simbole
				word2(parcad, kod, relok);
				continue;
			}
			else if (!deo0.compare(".char")) {//ne dozvoljava simbole
				char2(parcad, kod);
				continue;
			}
			else if (!deo0.compare(".skip")) {
				skip2(parcad, kod);
				continue;
			}
			else if (!deo0.compare(".align")) {
				align2(parcad, kod);
				continue;
			}
			//ako je bila direktiva, one dole provere nas se ne ticu
			continue;
		}//kraj provere direktiva

		//sad su cisto komande.

		if (deo0.find("_") != string::npos) {
			string uslov = deo0.substr(deo0.find("_") + 1); //odvaja deo od '_' nadalje(samo uslov))
			deo0 = deo0.substr(0, deo0.find("_") - 0); //samo komanda
			dealWithInstr(deo0, parcad, kod, relok, uslov); //prvi prolaz je proverio vec da li su naziv komande i uslova validni. treba proveriti ostatak
		}
		else {
			dealWithInstr(deo0, parcad, kod, relok);
		}

		if (deo0 != "ldc")incCnt(4);//ldc u sebi sam inkrementira
		//sve ostale instrukcije su duzine 4B
	}//kraj Lit petlje

}

bool rad::obaProlaza(string fileimein, string filenameout) {
	FileRel::readFile(fileimein, lines);
		INfilename = fileimein;
		OUTfilename = filenameout;
		priprema();
		prolazI();
		prolazII();

		FileRel::writeToFile(OUTfilename, relokacije, kodovi, simboli);
		if (!(status0 && status1 && status2))return 0;//oba prolaza zavrsila sa end
		else return 1;

}

void rad::skip2(vector<string>& parcad, string& kod) {
	std::string pom = parcad[1];
	pom = pom.substr(0, pom.find(",") - 0);
	int inc = std::stoi(pom, nullptr, 10);
	incCnt(inc);
	string fill = "00";
	if (parcad.size() == 3) {
		string f = parcad[2];
		unsigned long dec = stol(f);
		if (dec < 32 || dec>126)FileRel::writeErrToFile(OUTfilename, "nedozvoljena fill vrednost za skip");
		f = decToHex(dec, 1);
		if (f != ",")fill = f;
	}
	while (inc != 0) {
		kod = kod + fill + " ";
		inc--;
	}
}

void rad::align1(vector<string>& parcad, LineIterator& Lit) {
	if (parcad.size() < 2)FileRel::writeErrToFile(OUTfilename, "nedovoljno parametara za word/char");
	if (parcad.size() > 4)FileRel::writeErrToFile(OUTfilename, "previse parametara za word/char");
	string aln = parcad[1];
	aln = aln.substr(0, aln.find(",") - 0);

	if ((getCnt() % stoi(aln)) > 0) {//ako treba da se poravnava
		int inc = stoi(aln) - getCnt() % stoi(aln);//koliko treba bajtova da se doda
		if (parcad.size() == 4) {//ako imamo i max parametar
			string max = parcad[3];
			max = max.substr(0, max.find(",") - 0);
			if (inc > stoi(max)) {
				Lit++;
				return;
			}
		}
		incCnt(inc);
	}
	Lit++;
	return;
}


void rad::align2(vector<string>& parcad, string& kod){
	string fill = "00";
	string aln = parcad[1];
	aln = aln.substr(0, aln.find(",") - 0);

	if ((getCnt() % stoi(aln)) > 0) {
		int inc = stoi(aln) - getCnt() % stoi(aln);
		if (parcad.size() == 4) {//imamo sva 3 parametra(ili , za 2gi ako nije unet)
			string max = parcad[3];
			//nema zareza jer je poslednji parametar pa ne treba ni da proveravamo
			if (inc > stoi(max)) {
				return;
			}
		}
		incCnt(inc);
		if (parcad.size() > 2) {
			string f = parcad[2];
			if (parcad.size() == 4) f = f.substr(0, f.find(",") - 0);//nije poslednji param pa ima zarez
			if (f != "") {
				unsigned long dec = stol(f);
				if (dec < 32 || dec>126)FileRel::writeErrToFile(OUTfilename, "nedozvoljena fill vrednost za align");
				fill = decToHex(dec, 1);
			}
		}
		while (inc != 0) {
			kod = kod + fill + " ";
			inc--;
		}
	}
}

void rad::word1(vector<string>& parcad){
	if (parcad.size() < 2)FileRel::writeErrToFile(OUTfilename, "nedovoljno parametara za char");
	for (int iter = 1; iter != parcad.size(); iter++) {
		string simb = parcad[iter];
		simb = simb.substr(0, simb.find(",") - 0);//skida zarez
		
		if (checkIfStringIsInt(simb)) incCnt(2);//ako je immed
		else	incCnt(4);//ako je simb
	}

}
void rad::word2(vector<string>& parcad, string& kod, string& relok){
	for (int vit = 1; vit != parcad.size(); vit++) {
		string simb = parcad[vit];
		simb = simb.substr(0, simb.find(",") - 0);//skida zarez (ovo je sad immed/simb)
		unsigned long opcode = 0;

			if (!checkIfStringIsInt(simb)) {//ulazi ako je simbol
				if (!simboli.checkSymInTable(simb))FileRel::writeErrToFile(OUTfilename, "nepostojeci simbol");
				SymUlaz* ulaz = simboli.getSymFromTable(simb);
				//aps pa moze da pristupi simbolu iz bilo koje sekcije,pa i ext i publ i lok
				string ref = "";
				int vrsimbola = 0;//za globalne(public/ext)
				if (ulaz->GetLg() == "l") {
					vrsimbola = ulaz->GetOffset();
					ref = simboli.getSekRdbr(ulaz->GetSek());//lokalni, referisemo sekciju
				}
				else ref = to_string(ulaz->GetRdbr());//globalni, referisemo njega samog
				string offcnt = to_string(getCnt());
				relok = relok + offcnt + "\t Abs32 \t" + ref + "\t" + "noSign \n";
				
				opcode = vrsimbola;
				incCnt(4);
				littleEndian(opcode, 4);
				kod = kod + decToHex(opcode, 4) + " ";
			}
			else {//broj
				opcode = std::stol(simb);
				incCnt(2);
				littleEndian(opcode, 2);
				kod = kod + decToHex(opcode, 2) + " ";
			}
			
		
		
	}

}

void rad::char1(vector<string>& parcad) {
	if (parcad.size() < 2)FileRel::writeErrToFile(OUTfilename, "nedovoljno parametara za char");
	for (int iter = 1; iter != parcad.size(); iter++) {
		string simb = parcad[iter];
		simb = simb.substr(0, simb.find(",") - 0);//skida zarez
		if (!checkIfStringIsInt(simb)) FileRel::writeErrToFile(OUTfilename, "char ne podrzavaju simbole");
		incCnt(1);
	}
}

void rad::char2(vector<string>& parcad, string& kod) {
	for (int vit = 1; vit != parcad.size(); vit++) {
		string deo = parcad[vit];
		//immed
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		unsigned long opcode;
		if (!checkIfStringIsInt(deo))FileRel::writeErrToFile(OUTfilename, "char ne podrzava simbole");
		opcode = std::stoi(deo);
		string instr = parcad[0];
		if (".char" == instr && (opcode < 32 || opcode>126))FileRel::writeErrToFile(OUTfilename, "nedozvoljene vrednosti za char");
		littleEndian(opcode, 1);
		string hex = decToHex(opcode, 1);
		kod = kod + hex + " ";

		incCnt(1);
	}

}

void rad::long1(vector<string>& parcad) {
	if (parcad.size() < 2)FileRel::writeErrToFile(OUTfilename, "nedovoljno parametara za word/char");
	for (int iter = 1; iter != parcad.size(); iter++) {
		//treba da poveca inc za onoliko 4 koliko ima delova odvojenih razmakom(izraz generise samo jedanput 4!)

		incCnt(4);
	}
}

void rad::long2(vector<string>& parcad, string& kod, string& relok) {
	for (int vit = 1; vit != parcad.size(); vit++) {
		string deo = parcad[vit];
		deo = deo.substr(0, deo.find(",") - 0);//skida zarez
		unsigned long opcode = 0;

		do {
			char znak = '-';
			size_t nextZnak1;
			size_t nextZnak2;
			if (deo.at(0) != '-')znak = '+';
			if (deo.at(0) == '+' || deo.at(0) == '-')deo = deo.substr(1);//odseca znak
			nextZnak1 = deo.find("+");
			nextZnak2 = deo.find("-");
			nextZnak1 = nextZnak1 < nextZnak2 ? nextZnak1 : nextZnak2;//koji je blizi(ako uopste postoji)(npos je neki ogroman broj)
			string simb = deo.substr(0, nextZnak1);//uzima simbol/immed; radi ispravno i kad primi npos za nextZnak1
			if (nextZnak1 == string::npos)deo = "";
			else deo = deo.substr(nextZnak1);//priprema za naredni obrt do whilea; ne radi ispravno ako primi npos

			if (!checkIfStringIsInt(simb)) {//ulazi ako je simbol
				if (!simboli.checkSymInTable(simb))FileRel::writeErrToFile(OUTfilename, "nepostojeci simbol");
				SymUlaz* ulaz = simboli.getSymFromTable(simb);
				//aps pa moze da pristupi simbolu iz bilo koje sekcije,pa i ext i publ i lok
				string ref = "";
				int vrsimbola = 0;//za globalne(public/ext)
				if (ulaz->GetLg() == "l") {
					vrsimbola = ulaz->GetOffset();
					ref = simboli.getSekRdbr(ulaz->GetSek());//lokalni, referisemo sekciju
				}
				else ref = to_string(ulaz->GetRdbr());//globalni, referisemo njega samog
				string offcnt = to_string(getCnt());
				relok = relok + offcnt + "\t Abs32 \t" + ref + "\t" + znak + "\n";
				
				
				if (znak == '+') opcode += vrsimbola;
				else opcode -= vrsimbola;

			}
			else {//broj
				if (znak == '+') opcode += std::stol(simb);
				else opcode -= std::stol(simb);
			}
			
		} while (deo.length() > 0);
	
		littleEndian(opcode, 4);
		kod = kod + decToHex(opcode, 4) + " ";
		incCnt(4);
	}
}

//------------------------------------------------------------------------------
void rad::insertBreaks(string& str) {
	int length = str.length();
	int sek = secCurr.length();
	int ins = (length - (7 + 1 + 1 + 1 + sek)) / 60;
	int pom = 7 + 1 + 1 + 1 + secCurr.length() + 60;//sekcija+razmak+dvotacka+novired+imesekcije+parce koje formatiramo 
	while (ins > 0) {
		str.insert(pom, "\n");
		pom += 60 + 1;
		--ins;
	}
}
string rad::decToHex(unsigned long& code, int inc) {
	std::stringstream ss;
	ss << std::hex << code;
	string hex(ss.str());
	if (hex.length() == 8 && inc == 4) {
		hex.insert(2, " ");
		hex.insert(5, " ");
		hex.insert(8, " ");
	}
	else if (hex.length() < 8 && inc == 4) {
		int vrti = 8 - hex.length();
		while (vrti) {
			hex.insert(0, "0");
			vrti--;
		}
		hex.insert(2, " ");
		hex.insert(5, " ");
		hex.insert(8, " ");
	}
	else if (hex.length() == 4 && inc == 2) {
		hex.insert(2, " ");
	}
	else if (hex.length() == 3 && inc == 2) {//stringovi nemaju term karakter
		hex.insert(0, "0");
		hex.insert(2, " ");//razmak izmedju bajtova
	}
	else if (hex.length() == 2 && inc == 2) {
		hex.insert(0, "00 ");
	}
	else if (hex.length() == 1 && inc == 2) {
		hex.insert(0, "00 0");
	}
	else if (hex.length() == 1) {
		hex.insert(0, "0");
	}
	return hex;
}
void rad::littleEndian(unsigned long& code, int inc) {//shift ulevo za 3 bajta
	if (inc == 4) {//32b
		code = ((code >> 24) & 0x000000FF) | ((code >> 8) & 0x0000FF00) | ((code << 8) & 0x00FF0000) | ((code << 24) & 0xFF000000);
	}
	else if (inc == 2) {//16b		
		code = ((code >> 8) & 0x00FF) | ((code << 8) & 0xFF00);
	}
	else {
		//kod bajta se nista ne menja
	}
}

void rad::saveTables(string& kod, string& relok) {//pohranjuje podatke trenutne sekcije
	insertBreaks(kod);
	relokacije.put(relok);
	kodovi.put(kod);

}
					//sta rasparcavamo		u sta smestamo rasparcano	na osnovu cega rasparcavamo
void rad::Tokenize(const string& str, vector<string>& tokens, const string& delimiters) {
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

bool rad::checkIfStringIsInt(const string& s) {

	{
		if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

		char * p;
		strtol(s.c_str(), &p, 10);

		return (*p == 0); //ako je *p==0, onda string jeste int
	}//dakle vraca true ako je int
}
