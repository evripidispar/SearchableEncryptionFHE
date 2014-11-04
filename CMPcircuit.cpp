#include <HElib/timing.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include "CMPcircuit.h"

using namespace std;


bool CMP1bit(bool x, bool y, bool c){

bool c_new;

c_new = (((x^c)&(c^y))^x);

return c_new;
}

bool CMPcircuit(bitset<30> bsx, bitset<30> bsy){

bool c = 0;

for (int i = 0; i< (int)  bsx.size();  i++){

cout << bsx[i]; 
cout << bsy[i] <<endl;

c = (((bsx[i]^c)&(c^bsy[i]))^bsx[i]);
//c = CMP1bit(bsx[i], bsy[i], c);

}

return c;
} 

//int main(int argc, char *argv[])
//{
//long x,y;
//bool compare;

//cout << "Enter the first long integer: ";
//cin >> x;

//cout << "Enter the second long integer: ";
//cin >> y;

//create bitset from long integers

//bitset<30> bs1(x);
//bitset<30> bs2(y);

//compare = CMPcircuit(bs1, bs2);

//if (compare){
//cout << x << " is the largest number " <<endl;
//}
//else{
//cout << y << " is greater than or equal to " << x << endl;
//}

//return 0;
//}


