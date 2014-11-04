# Copyright (C) 2012,2013 IBM Corp.
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
# 
CC = g++
#
LIBDIR = /usr/local/lib/fhe.a
CFLAGS = -g -O2 -Wfatal-errors -Wshadow -Wall -I/usr/local/include 

#  If you get compilation errors, you may need to add -std=c++11 or -std=c++0x
#CFLAGS = -g -O2  -Wfatal-errors -Wshadow -Wall -I/usr/local/include -std=c++11 -lstdc++
#CFLAGS = -g -O2  -Wfatal-errors -Wshadow -Wall -I/usr/local/include -std=c++0x

#  To get the old DoubleCRT implementation, add NO_ALT_CRT flag
#CFLAGS= -g -O2 -Wfatal-errors -Wshadow -Wall -I/usr/local/include -DNO_ALT_CRT

#$(info HElib requires NTL version 6.1.0 or higher, see http://shoup.net/ntl)
#$(info If you get compilation errors, try to add/remove -std=c++11 in Makefile)
#$(info )

LD = g++
AR = ar
ARFLAGS=ruv
GMP=-lgmp 
LDLIBS = -L/usr/local/lib -lntl  $(GMP) -lm


#FHELIBDIR = /home/evripidis/HElib/src
#HEADER = EncryptedArray.h FHE.h Ctxt.h CModulus.h FHEContext.h PAlgebra.h SingleCRT.h DoubleCRT.h NumbTh.h bluestein.h IndexSet.h timing.h IndexMap.h replicate.h hypercube.h matching.h powerful.h permutations.h polyEval.h

#SRC = KeySwitching.cpp EncryptedArray.cpp FHE.cpp Ctxt.cpp CModulus.cpp FHEContext.cpp PAlgebra.cpp SingleCRT.cpp DoubleCRT.cpp NumbTh.cpp PAlgebraMod.cpp bluestein.cpp IndexSet.cpp timing.cpp replicate.cpp hypercube.cpp matching.cpp powerful.cpp BenesNetwork.cpp permutations.cpp PermNetwork.cpp OptimizePermutations.cpp eqtesting.cpp polyEval.cpp
#SRC = Test_vectoraddmul.cpp
#OBJ = EncryptedArray.o FHE.o Ctxt.o CModulus.o FHEContext.o PAlgebra.o SingleCRT.o DoubleCRT.o NumbTh.o bluestein.o IndexSet.o timing.o KeySwitching.o PAlgebraMod.o
HEADER = CMPcircuit.h
OBJ = CMPcircuit.o Test_vectoraddmul.o Test_CMPcircuit.o Test_CMPcircuitFHE.o Test_CMPcircuitFHEcomponent.o
EXECPROGS = Test_CMPcircuit_x Test_vectoraddmul_x Test_CMPcircuitFHE_x Test_CMPcircuitFHEcomponent_x
#OBJ = NumbTh.o timing.o bluestein.o PAlgebra.o  CModulus.o FHEContext.o IndexSet.o DoubleCRT.o SingleCRT.o FHE.o KeySwitching.o Ctxt.o EncryptedArray.o replicate.o hypercube.o matching.o powerful.o BenesNetwork.o permutations.o PermNetwork.o OptimizePermutations.o eqtesting.o polyEval.o

#TESTPROGS = Test_PAlgebra_x Test_DoubleCRT_x Test_CModulus_x Test_FHE_x Test_Arrays_x
#TESTPROGS = Test_General_x Test_Replicate_x Test_LinPoly_x Test_matmul_x Test_Powerful_x Test_Permutations_x Test_Timing_x Test_PolyEval_x Test_vectoraddmul_x


#all: fhe.a

#check: Test_General_x
#	./Test_General_x R=1 m=255
#	./Test_General_x R=1 k=10 p=2 r=1 d=1
#	./Test_General_x R=1 k=10 p=2 r=2 d=1
#	./Test_General_x R=1 k=10 p=2 r=1 d=2
#	./Test_General_x R=1 k=10 p=3 r=1 d=1
#	./Test_General_x R=2 k=10 p=3 r=2 d=1 c=3
#	./Test_General_x R=1 k=10 p=3 r=1 d=2 c=3

#test: $(TESTPROGS)

#fhe.a: $(OBJ)
#	$(AR) $(ARFLAGS) fhe.a $(OBJ)

#all: CMPcircuit.o Test_vectoraddmul.o
#	$(CC) $(CFLAGS) -o CMPcircuit CMPcircuit.o $(LIBDIR) $(LDLIBS)
#	$(CC) $(CFLAGS) -o Test_vectoraddmul Test_vectoraddmul.o $(LIBDIR) $(LDLIBS)

all: $(EXECPROGS)

obj: $(OBJ)
%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) -c $<
./%_x: %.o CMPcircuit.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LIBDIR) $(LDLIBS)

clean:
	rm -f *.o *_x *_x.exe *.a core.*
#	rm -rf *.dSYM
#	rm -r Test_vectoraddmul CMPcircuit

#info:
#	: HElib require NTL version 6.1.0 or higher
#	: Compilation flags are 'CFLAGS=$(CFLAGS)'
#	: If errors occur, try adding/removing '-std=c++11' in Makefile
#	:
