#ifndef __UPGMA__
#define __UPGMA__

#include <string>
#include "mtxutl.hpp"
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

vector<double>* initdistance;               // 记录初始距离矩阵
unordered_map<int, double>* newdistance;    // 记录聚类到各个序列之间的距离
set<int> unselected;                        // 记录还未加入到树中的序列编号

double calculateDistance(const string& seq1, const string& seq2);

int calculatePos(int i, int j, int n);

// 计算初始距离矩阵，初始化聚类到各个序列之间的距离，初始化unselected
pair<int, int> initDistance(vector<string>& sequences);

int updateDistance(int premin, int n);

vector<string> sortFastaByUPGMA(vector<string>& sequences);

#endif