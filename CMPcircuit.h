#ifndef _CMPcircuit_H_
#define _CMPcircuit_H_

#include <iostream>
#include <bitset>
#include <sstream>

#define BIT_SIZE 6

using namespace std;

//extern bool c;

//compute 1-bit comparator

bool CMP1bit(bool x, bool y,bool c);

//compute the comparison of two long integers

bool CMPcircuit(bitset<BIT_SIZE> bsx, bitset<BIT_SIZE> bsy);


#endif // _CMPcircuit_H_
