#ifndef CACHECHECKER_H
#define CACHECHECKER_H
#include <iostream>
#include <string>
#include <vector>
#include "CacheSim.h"
using namespace std;

class CacheTester{
private:
	int nodeNum_;
	string type_;
	CacheSim cache;
	bool hasE_;
	bool hasO_;
public:
        CacheTester(int nodeNum , string type, bool showtrace = false, bool showset = false);
	void test();
	void BFS();
	void random(int length_);
	void mcjammer(int length_);
private:
	void visit_e_vertex();
	void visit_clique(int m);
	void do_write_transition(int id);
	void do_write_transition_MSI(int id);
	void do_write_transition_with_o(int id,int opos);
	void do_write_transition_has_e(int id);
	void visit_hypercube(int id, int m ,int shift);
	void visit_hypercube_with_o(int id, int m ,int shift, int opos);
	void do_write_transition_moesi(int id);
};

#endif
