
/* 
 * File:   KodoviQ.h
 * Author: etf
 *
 * Created on June 21, 2016, 3:34 AM
 */

#ifndef KODOVIQ_H
#define KODOVIQ_H
#include<deque>
#include<string>
#include <iostream>
using namespace std;
typedef deque<string>::iterator QIterator;

class KodoviQ {
public:
    KodoviQ();

    virtual ~KodoviQ();

    void put(string ulaz) {
        KjuKodKlan.push_back(ulaz);
    }

    string get() {
        string ret = KjuKodKlan.front();
        KjuKodKlan.pop_front();
        return ret;
    }

    size_t getQSize()const{
        return KjuKodKlan.size();
    }

	QIterator getQBegin() {
        return KjuKodKlan.begin();
    }

	QIterator getQEnd()  {
        return KjuKodKlan.end();
    }

	
	string ispisi();

private:
    KodoviQ(const KodoviQ& orig);

    deque<string> KjuKodKlan; //stringovi oblika:(naziv sekcije \n kodovi)
};

#endif /* KODOVIQ_H */

