#include <HElib/timing.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include "CMPcircuit.h"

using namespace std;

int main(int argc, char *argv[])
{
long x,y;
bool compare;

cout << "Enter the first long integer: ";
cin >> x;

cout << "Enter the second long integer: ";
cin >> y;

//create bitset from long integers

bitset<BIT_SIZE> bs1(x);
bitset<BIT_SIZE> bs2(y);

compare = CMPcircuit(bs1, bs2);

if (compare){
cout << x << " is the largest number " <<endl;
}
else{
cout << y << " is greater than or equal to " << x << endl;
}

return 0;
}


