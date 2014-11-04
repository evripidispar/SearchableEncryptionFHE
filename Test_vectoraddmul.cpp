#include <HElib/FHE.h>
#include <HElib/timing.h>
#include <HElib/EncryptedArray.h>
#include <NTL/lzz_pXFactoring.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>

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

    vector<long> v1;
    for(int i = 0 ; i < nslots; i++) {
        v1.push_back(1);
       //v1.push_back(i*2);
    }
    Ctxt ct1(publicKey);
    startFHEtimer("ea.encrypt1");
    ea.encrypt(ct1, publicKey, v1);
    stopFHEtimer("ea.encrypt1");
        
    vector<long> v2;
    Ctxt ct2(publicKey);
    for(int i = 0 ; i < nslots; i++) {
        v2.push_back(1);
        //v2.push_back(i*3);
    }
    startFHEtimer("ea.encrypt2");
    ea.encrypt(ct2, publicKey, v2);
    stopFHEtimer("ea.encrypt2");

    // v1.mul(v2); // c3.multiplyBy(c2) 
    // ct2.multiplyBy(ct1);              
    // CheckCtxt(ct2, "c3*=c2");
    // debugCompare(ea,secretKey,v1,ct2);

    // On the public (untrusted) system we
    // can now perform our computation

    Ctxt ctSum = ct1;
    Ctxt ctProd = ct1;

    startFHEtimer("sum");
    ctSum += ct2;
    stopFHEtimer("sum");
    //ctProd *= ct2;
    startFHEtimer("product");
    ctProd *= ct2;
    //ctProd.multiplyBy(ct2);
    stopFHEtimer("product");
    //ea.mat_mul(ctProd,ct2);
    vector<long> res;

    startFHEtimer("decryptsum");
    ea.decrypt(ctSum, secretKey, res);
    stopFHEtimer("decryptsum");
    //cout << "All computations are modulo " << p << "." << endl;
    for (unsigned int i = 0; i<res.size(); i++){
        cout<< res[i];
    }

    for(unsigned int i = 0; i < res.size(); i++) {
        cout << v1[i] << " + " << v2[i] << " = " << res[i] << endl;
    }

    startFHEtimer("decryptproduct");
    ea.decrypt(ctProd, secretKey, res);
    stopFHEtimer("decryptproduct");
    for (unsigned int i = 0; i<res.size(); i++){
        cout<< res[i];
    }
    
    for(unsigned int i = 0; i < res.size(); i++) {
        cout << v1[i] << " * " << v2[i] << " = " << res[i] << endl;
    }
    printAllTimers();
    cout << endl;
    cout << "All computations are modulo " << p << "." << endl;
    return 0;
}


