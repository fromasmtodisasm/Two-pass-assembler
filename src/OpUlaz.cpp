/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OpUlaz.cpp
 * Author: etf
 * 
 * Created on June 20, 2016, 6:27 AM
 */

#include "OpUlaz.h"

OpUlaz::OpUlaz(string m, unsigned long o, unsigned short r){
            mnemonik=m; opcode=o; regpermission=r;
        }

OpUlaz::OpUlaz() {
}

OpUlaz::OpUlaz(const OpUlaz& orig) {
}

OpUlaz::~OpUlaz() {
}

