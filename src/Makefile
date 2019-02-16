ROOT	= ..
INCLUDE	= $(ROOT)/header
CC	= g++
CCFLAGS	= -g -O0 -std=c++11 -c -I$(INCLUDE)

all:	util.o CacheSim.o CacheTester.o main.o $(ROOT)/bin/CacheChecker

clean:
	rm -f *.o $(ROOT)/bin/CacheChecker

util.o:	util.cpp $(INCLUDE)/util.h
	$(CC) $(CCFLAGS) util.cpp

CacheSim.o: CacheSim.cpp $(INCLUDE)/CacheSim.h
	$(CC) $(CCFLAGS) CacheSim.cpp
	
CacheTester.o: CacheTester.cpp $(INCLUDE)/CacheTester.h
	$(CC) $(CCFLAGS) CacheTester.cpp

main.o:	main.cpp
	$(CC) $(CCFLAGS) main.cpp

$(ROOT)/bin/CacheChecker:	util.o CacheSim.o CacheTester.o main.o
	$(CC) -g -O0 main.o util.o CacheSim.o CacheTester.o -o $(ROOT)/bin/CacheChecker
		
