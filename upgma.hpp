#ifndef __UPGMA__
#define __UPGMA__

#include <string>
#include "mtxutl.hpp"
#include <vector>
#include <set>

using namespace std;

vector<int>* distance;
vector<int>* newdistance;
set<int> unselected;

int calculateDistance(const string& seq1, const string& seq2);

pair<int, int> initDistance(vector<string>& sequences);


#endif