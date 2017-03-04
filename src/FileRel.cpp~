/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileRel.cpp
 * Author: etf
 * 
 * Created on June 20, 2016, 11:59 AM
 */

#include "FileRel.h"

FileRel::FileRel() {
}

FileRel::FileRel(const FileRel& orig) {
}

FileRel::~FileRel() {
}

bool FileRel::readFile(string filename, deque<string>& lines) {
    string line;
    ifstream myfile(filename);
    if (myfile.is_open()) {//u project3(ili koji vec broj) folderu
        while (getline(myfile, line)) {
            if (checkOnlyLabel(line) == true) {//ako je labela sama u redu, procitaj jos jedan red
                string temp = "";
                getline(myfile, temp);
				if (temp.find(".end") != string::npos || temp.find(".bss") != string::npos ||
					temp.find(".data") != string::npos || temp.find(".text") != string::npos) {//jeste nesto od tih pa treba posebna linija
					lines.push_back(line);
					lines.push_back(temp);
					continue;
				}
				else {//nije data,end,bss,text
					line += " " + temp;
					lines.push_back(line);
					continue;
				}
            }
			lines.push_back(line);
         }
        myfile.close();
    } else cout << "Unable to open file-read mode"+filename;

    return 0;
}

void FileRel::writeToFile(string filenameout, RelocTableQ& relokacije,KodoviQ& kodovi, SymTable& simboli) {
    ofstream myfile(filenameout);
    if (myfile.is_open()) {
		myfile << "tabele\n";
		myfile << simboli;
		
        myfile<<"tabele relokacije\n";
		myfile << relokacije.ispisi();
       
        myfile<<"kodovi instrukcija po sekcijama\n";
		myfile << kodovi.ispisi();
		myfile.close();
    } else cout << "Unable to open file -writeRegular"+filenameout;
}

bool FileRel::checkOnlyLabel(string line) {
    // Skip delimiters at beginning.
    string::size_type lastPos = line.find_first_not_of(" ", 0);
    // Find first "non-delimiter".
    string::size_type pos = line.find_first_of(":", lastPos);//labele ima
    if (pos != string::npos) {//ima labele
        lastPos = line.find_first_not_of(" ", pos+1);	//prvi koji nije razmak nakon znaka :
        string::size_type lastPos1 = line.find_first_not_of("\n", pos+1);//a da nije \n
        if (lastPos != std::string::npos && lastPos1 != std::string::npos) {
            return false; //labela nije sama
		}
		else return true;//labela je sama
    }
    return false; //nije labela 
}

void FileRel::writeErrToFile(string filename, string error) {
	string outfile;
	size_t poz = filename.find(".txt");
	outfile = filename.substr(0, poz);
	outfile = outfile + "Error.txt";
	ofstream myfile(outfile);
	if (myfile.is_open()) {
		myfile << "!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
		myfile << error;
		myfile << "!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
		myfile.close();
		exit(1);
	}
	else cout << "Unable to open file -writeError"+filename;
}
