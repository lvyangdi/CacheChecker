#ifndef CACHESIM
#define CACHESIM
#include <iostream>
#include <unordered_set>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "util.h"
using namespace std;

class CacheSim{
private:
	int nodeNum_;
	string type_;
	unordered_set<string> visited;
	unordered_set<string> edges;
	string states_;
	string initialState_;
	int totalActions_;
	string dumpInfo;
	bool dumpHashSet_;
	bool showTrace_;

public:
	CacheSim(int nodeNum , string type, bool showtrace, bool showset);
	void nextAction(int nAction);
	void summary();
	void dump(unordered_set<string> h);
	void dumpToFile(string file, string data);
	void BFS();
	void random(int length_);
	void mcjammer(int length_);
	void showTrace() {showTrace_ = !showTrace_;}
	void showSet() {dumpHashSet_ = !dumpHashSet_;}
};
#endif
