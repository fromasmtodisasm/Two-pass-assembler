/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: etf
 *
 * Created on May 24, 2016, 7:19 AM
 */

#include <cstdlib>
#include <iostream>
#include "rad.h"
#include <string>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    string filenamein(argv[1]);
	string filenameout(argv[2]);
	rad r;
    bool ret;
    ret = r.obaProlaza(filenamein,filenameout);
    //vidi sta treba da se rucno pobije od struktura
    return ret;
}

