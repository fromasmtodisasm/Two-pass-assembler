/* 
 * File:   OpCodeTable.cpp
 * Author: etf
 * 
 * Created on May 24, 2016, 8:30 AM
 */

#include "OpCodeTable.h"

OpCodeTable::OpCodeTable() {//optabela.insert(pair<string, OpUlaz*>("ime", op)); to je konstruktor za pair
	//moraju da budu pokazivaci na OpUlaz jer nemam kopirajuci konstruktor/op= stagod, a lokalni su
	optabela.insert(pair<string, OpUlaz*>("int", new OpUlaz("int", 0xe0000000, 0x0)));
//----------------------------------------------   
    optabela.insert(pair<string, OpUlaz*>("add", new OpUlaz("add", 0xe1000000, 0xe)));
    optabela.insert(pair<string, OpUlaz*>("addf", new OpUlaz("addf", 0xf1000000, 0xe))); 
    
    optabela.insert(pair<string, OpUlaz*>("sub", new OpUlaz("sub", 0xe2000000, 0xe)));
    optabela.insert(pair<string, OpUlaz*>("subf", new OpUlaz("subf", 0xf2000000, 0xe))); 
    
    optabela.insert(pair<string, OpUlaz*>("mul", new OpUlaz("mul", 0xe3000000, 0x0))); 
    optabela.insert(pair<string, OpUlaz*>("mulf", new OpUlaz("mulf", 0xf3000000, 0x0)));
    
    optabela.insert(pair<string, OpUlaz*>("div", new OpUlaz("div", 0xe4000000, 0x0))); 
    optabela.insert(pair<string, OpUlaz*>("divf", new OpUlaz("divf", 0xf4000000, 0x0))); 
   
    optabela.insert(pair<string, OpUlaz*>("cmp", new OpUlaz("cmp", 0xe5000000, 0x0))); 
    optabela.insert(pair<string, OpUlaz*>("cmpf", new OpUlaz("cmpf", 0xf5000000, 0x0))); 
//-----------------------------------------------
    optabela.insert(pair<string, OpUlaz*>("and", new OpUlaz("and", 0xe6000000, 0x2))); 
    optabela.insert(pair<string, OpUlaz*>("andf", new OpUlaz("andf", 0xf6000000, 0x2))); 
    
    optabela.insert(pair<string, OpUlaz*>("or", new OpUlaz("or", 0xe7000000, 0x2))); 
    optabela.insert(pair<string, OpUlaz*>("orf", new OpUlaz("orf", 0xf7000000, 0x2)));

    optabela.insert(pair<string, OpUlaz*>("not", new OpUlaz("not", 0xe8000000, 0x2))); 
    optabela.insert(pair<string, OpUlaz*>("notf", new OpUlaz("notf", 0xf8000000, 0x2))); 
    
    optabela.insert(pair<string, OpUlaz*>("test", new OpUlaz("test", 0xe9000000, 0x2))); 
    optabela.insert(pair<string, OpUlaz*>("testf", new OpUlaz("testf", 0xf9000000, 0x2))); 
//-----------------------------------------------
    optabela.insert(pair<string, OpUlaz*>("ldr", new OpUlaz("ldr", 0xea000400, 0xf))); 
        
    optabela.insert(pair<string, OpUlaz*>("str", new OpUlaz("str", 0xea000000, 0xf))); //bit 9 sa leva =0 za str
      
    optabela.insert(pair<string, OpUlaz*>("call", new OpUlaz("call", 0xec000000, 0xf)));

    optabela.insert(pair<string, OpUlaz*>("in", new OpUlaz("in", 0xed008000, 0x0)));  //bit 9 sa leva =1 za in
        
    optabela.insert(pair<string, OpUlaz*>("out", new OpUlaz("out", 0xed000000, 0x0))); 

	optabela.insert(pair<string, OpUlaz*>("mov", new OpUlaz("mov", 0xee000000, 0xf)));
    optabela.insert(pair<string, OpUlaz*>("movf", new OpUlaz("movf", 0xfe000000, 0xf))); 
    
    optabela.insert(pair<string, OpUlaz*>("shr", new OpUlaz("shr", 0xee000000, 0xf))); 
	optabela.insert(pair<string, OpUlaz*>("shrf", new OpUlaz("shrf", 0xfe000000, 0xf)));
    
    optabela.insert(pair<string, OpUlaz*>("shl", new OpUlaz("shl", 0xee000100, 0xf)));   
    optabela.insert(pair<string, OpUlaz*>("shlf", new OpUlaz("shlf", 0xfe000100, 0xf))); 
    
    optabela.insert(pair<string, OpUlaz*>("ldch", new OpUlaz("ldch", 0xef080000, 0x0)));  
       
    optabela.insert(pair<string, OpUlaz*>("ldcl", new OpUlaz("ldcl", 0xef000000, 0x0)));
    
    optabela.insert(pair<string, OpUlaz*>("ldc", new OpUlaz("ldc", 0x12345678, 0x08))); //nije im to zaista kod, samo sam stavila da popuni mesto
   
//-----------------------------------------------------------
//-----------------------------------------------------------
	condtabela["eq"] = 0x0;
	condtabela["ne"] = 0x1;
	condtabela["gt"] = 0x2;
	condtabela["ge"] = 0x3;
	condtabela["lt"] = 0x4;
	condtabela["le"] = 0x5;
	condtabela["al"] = 0x7;//bezuslovno,podrazumeva se
    //br 7 koristimo za slucaj kada nema uslova
}

OpCodeTable::~OpCodeTable() {
    
}

long OpCodeTable::getOPCode(string name, string cond) {//ne radi upisivanje kodova operanada
	OpUlaz* temp = optabela[name];
	unsigned long temp1 = temp->getKod();
	temp1 = temp1 & 0x1fffffff;//da ocistimo onu 7icu sa prva 3 bita da bi posle lepo lepili vrednost za uslov(ako je naknadno stavljen uslov(podrzava i da ga nema, i da ima al tj bezusl oznaku)
	unsigned long temp2 = condtabela[cond];
	temp2 = temp2<<29;
	temp1 = temp1 | temp2;
	return temp1;
	//return optable[name]; ili ovo?? ne znam sta od ova 2 vraca vrednost a sta samo pokazivac
}

bool OpCodeTable::checkRegPermitted(string name, string reg) {
	bool res = true;
	OpUlaz* temp = optabela[name];
	unsigned long perm = temp->getPerm();
	if (reg.compare("r16") == 0) {
		res = (8 & perm) != 0 ? true : false;
	}
	else if (reg.compare("r17") == 0) {
		res = (4 & perm) != 0 ? true : false;
	}
	else if (reg.compare("r18") == 0) {
		res = (2 & perm) != 0 ? true : false;
	}
	else if (reg.compare("r19") == 0) {
		res = (1 & perm) != 0 ? true : false;
	}

	return res;
}//vraca true ako je dozvoljeno
