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
    //int i = 0;
    int j = 1;
    for (int l = 0; l< nslots ;  l++){ 
        if (l<(nslots-BIT_SIZE)){
            v1.push_back(0);
        }
        else{
        v1.push_back(long (bs1[BIT_SIZE-j]));
        j++;
    }
    cout << v1[l];
    }
    cout <<endl;
  	
  	vector<long> v2;
    int k = 1;
    for(int i = 0 ; i < nslots ; i++) {
        if (i<(nslots-BIT_SIZE)){
            v2.push_back(0);
        }
        else{
        v2.push_back(long (bs2[BIT_SIZE-k]));
        k++;
    }
    cout << v2[i];
    }
    cout <<endl;

    
    vector<long> zerovector(nslots,0);
    vector<long> zerovector1(nslots-1,0);
    
    vector<Ctxt> Ctxtset1;
  	vector<Ctxt> Ctxtset2;
    
    startFHEtimer("ea.encrypt1");
    cout << "Bit size " << BIT_SIZE <<endl; 
    unsigned int i=0;
    
    while(i<BIT_SIZE){
    	long tmp1 = v1.back();
    	v1.pop_back();
    	zerovector1.push_back(tmp1);
    	
    	Ctxt tmpCtxt1(publicKey);
    	//startFHEtimer("ea.encrypt1");
    	ea.encrypt(tmpCtxt1, publicKey, zerovector1);
    	//stopFHEtimer("ea.encrypt1");
    	
    	Ctxtset1.push_back(tmpCtxt1); 
    	i++;
    	zerovector1.pop_back();  		
    }
    
    cout <<" Size of zerovector1 " << zerovector1.size() <<endl;
    cout <<" Size of Ctxtset1 " << Ctxtset1.size() <<endl;
    
    stopFHEtimer("ea.encrypt1");
    
	
	
	startFHEtimer("ea.encrypt2");
    i=0;
    
    while(i<BIT_SIZE){
    	long tmp2 = v2.back();
    	v2.pop_back();
    	zerovector1.push_back(tmp2);
    	
    	Ctxt tmpCtxt2(publicKey);
    	//startFHEtimer("ea.encrypt1");
    	ea.encrypt(tmpCtxt2, publicKey, zerovector1);
    	//stopFHEtimer("ea.encrypt1");
    	
    	Ctxtset2.push_back(tmpCtxt2); 
    	i++;
    	zerovector1.pop_back();  		
    }
    
    cout <<" Size of zerovector1 " << zerovector1.size() <<endl;
    cout <<" Size of Ctxtset2 " << Ctxtset2.size() <<endl;
    
    stopFHEtimer("ea.encrypt2");


    Ctxt tmpCtxt3(publicKey);
    startFHEtimer("ea.encrypt3");
    ea.encrypt(tmpCtxt3, publicKey, zerovector);
    stopFHEtimer("ea.encrypt3");

    // On the public (untrusted) system we
    // can now perform our computation
	
	Ctxt ctCMP =  Ctxtset1[0];
    Ctxt ctCarry = tmpCtxt3;
    //Ctxt ctProd = ct1;

    startFHEtimer("comparator");
    
    i = 0;
    while(i < Ctxtset1.size()){
    	cout << "flag";
    	ctCMP = Ctxtset1[i];
    	ctCMP.CMPcircuit(Ctxtset2[i],ctCarry);
		cout << "Test" << endl;
		ctCarry = ctCMP;
		i++;
    }
    
   	stopFHEtimer("comparator");

    vector<long> res;

    startFHEtimer("decryptsum");
    ea.decrypt(ctCarry, secretKey, res);
    stopFHEtimer("decryptsum");
    
    cout <<res.size() <<endl;
    i = 0;
    while (i<res.size()){
        	cout<< res[i];
        	i++;
    	}
    cout <<endl;
    cout << long(compare) << " = " << res[res.size()-1];
    printAllTimers();
    cout << endl;
    cout << "All computations are modulo " << p << "." << endl;
    return 0;
}

