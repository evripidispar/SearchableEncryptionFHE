#ifndef _CMPcircuit_H_
#define _CMPcircuit_H_

#include <iostream>
#include <bitset>
#include <sstream>

using namespace std;

//extern bool c;

//compute 1-bit comparator

bool CMP1bit(bool x, bool y,bool c);

//compute the comparison of two long integers

bool CMPcircuit(bitset<30> bsx, bitset<30> bsy);


#endif // _CMPcircuit_H_
