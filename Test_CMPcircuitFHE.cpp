#include <HElib/FHE.h>
#include <HElib/timing.h>
#include <HElib/EncryptedArray.h>
#include <NTL/lzz_pXFactoring.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <bitset>
#include "CMPcircuit.h"


int main(int argc, char **argv)
{
    /* On our trusted system we generate a new key
     * (or read one in) and encrypt the secret data set.
     */

    long m=0, p=2, r=1; // Native plaintext space
                            // Computations will be 'modulo p'
    long L=16;          // Levels
    long c=3;           // Columns in key switching matrix
    long w=64;          // Hamming weight of secret key
    long d=0;
    long security = 128;
    ZZX G;
    m = FindM(security,L,c,p, d, 0, 0);


    FHEcontext context(m, p, r);
    // initialize context
    buildModChain(context, L, c);
    // modify the context, adding primes to the modulus chain
    FHESecKey secretKey(context);
    // construct a secret key structure
    const FHEPubKey& publicKey = secretKey;
    // an "upcast": FHESecKey is a subclass of FHEPubKey

    //if(0 == d)
    G = context.alMod.getFactorsOverZZ()[0];

    secretKey.GenSecKey(w);
    // actually generate a secret key with Hamming weight w

    addSome1DMatrices(secretKey);
    cout << "Generated key..." << endl;

    EncryptedArray ea(context, G);
    // constuct an Encrypted array object ea that is
    // associated with the given context and the polynomial G

    long nslots = ea.size();
    cout << "Vector Size " << nslots << endl;;

    //const long ctxtsize = nslots;
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

    vector<long> v1;
    int j = 1;
    for (int i = 0; i< nslots ;  i++){ 
        if (i<(nslots-30)){
            v1.push_back(0);
        }
        else{
        v1.push_back(long (bs1[30-j]));
        j++;
    }
    cout << v1[i];
    }
    cout <<endl;
  

    Ctxt ct1(publicKey);
    startFHEtimer("ea.encrypt1");
    ea.encrypt(ct1, publicKey, v1);
    stopFHEtimer("ea.encrypt1");
        
    vector<long> v2;
    int k = 1;
    for(int i = 0 ; i < nslots ; i++) {
        if (i<(nslots-30)){
            v2.push_back(0);
        }
        else{
        v2.push_back(long (bs2[30-k]));
        k++;
    }
    cout << v2[i];
    }
    cout <<endl;

    Ctxt ct2(publicKey);
    startFHEtimer("ea.encrypt2");
    ea.encrypt(ct2, publicKey, v2);
    stopFHEtimer("ea.encrypt2");

    vector<long> carry;
    for(int i = 0 ; i < nslots ; i++) {
        carry.push_back(0);
    }

    Ctxt ct3(publicKey);
    startFHEtimer("ea.encrypt3");
    ea.encrypt(ct3, publicKey, carry);
    stopFHEtimer("ea.encrypt3");

    // v1.mul(v2); // c3.multiplyBy(c2) 
    // ct2.multiplyBy(ct1);              
    // CheckCtxt(ct2, "c3*=c2");
    // debugCompare(ea,secretKey,v1,ct2);

    // On the public (untrusted) system we
    // can now perform our computation

    Ctxt ctCMP = ct1;
    //Ctxt ctProd = ct1;

    startFHEtimer("comparator");
    ctCMP.CMPcircuit(ct2,ct3);
    //ctCMP > ct2;
    stopFHEtimer("comparator");

    vector<long> res;

    startFHEtimer("decryptcmp");
    ea.decrypt(ctCMP, secretKey, res);
    stopFHEtimer("decryptcmp");
    
    cout <<res.size() <<endl;
    for (unsigned int i = 0; i<res.size(); i++){
        cout<< res[i];
    }
    cout <<endl;
    cout << long(compare) << " = " << res[0];
    printAllTimers();
    cout << endl;
    cout << "All computations are modulo " << p << "." << endl;
    return 0;
}

