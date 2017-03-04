/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OpUlaz.h
 * Author: etf
 *
 * Created on June 20, 2016, 6:27 AM
 */

#ifndef OPULAZ_H
#define OPULAZ_H
#include<string>
using namespace std;

class OpUlaz {
public:
    OpUlaz(string m,unsigned long o,unsigned short r);
    OpUlaz();
    OpUlaz(const OpUlaz& orig);
    virtual ~OpUlaz();
    unsigned long getKod(){
        return opcode;
    }
    unsigned short getPerm(){
        return regpermission;
    }
    void setKod(unsigned long k){
        opcode=k;
    }
    void setPerm(unsigned short p){
        regpermission=p;
    }
private:
    string mnemonik;
    unsigned long opcode;//4B koda za svaku instrukciju
    unsigned short regpermission;//4b(donja) koja govore sme li da pristupi PC(3),LR(2),SP(1),PSW(0)
    
    
};

#endif /* OPULAZ_H */

