
/* 
 * File:   KodoviQ.cpp
 * Author: etf
 * 
 * Created on June 21, 2016, 3:34 AM
 */

#include "KodoviQ.h"

KodoviQ::KodoviQ() {
}

KodoviQ::KodoviQ(const KodoviQ& orig) {
}

KodoviQ::~KodoviQ() {
}

string KodoviQ::ispisi() {

	size_t len = getQSize();
	string ret = "=================== kodovi ========================\n";

	for (deque<string>::iterator iter = getQBegin(); iter != getQEnd(); iter++) {
		if (iter == getQBegin()) continue;
		string ulaz = *iter;
		ret += ulaz;
		ret += "\n";
		ret += "----------------------------------------------------\n";
	}

	ret += "==================================================\n";
	return ret;
}



