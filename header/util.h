#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <iostream>
using namespace std;

// q for quotient protocol
// cover from IO to IS in MOSI and MOESI
// q stands for three operations
static const char table_[] = {'l', 's', 'e', 'L', 'S', 'E', 'q'};

bool checkAllI(const string &state);
string action(string type, const string &oldState, int actionNum);

#endif
