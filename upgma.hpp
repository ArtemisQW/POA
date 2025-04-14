#ifndef __UPGMA__
#define __UPGMA__

#include <string>
#include "mtxutl.hpp"
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cmath>
using namespace std;



void generateKmerSet(vector<string>& sequences, int k);

double calculateDistance(int i, int j, int k);

int calculatePos(int i, int j, int n);

int llcs(const string& seq1, const string& seq2, int mode);

void lcs(const string& seq1, const string& seq2, int** L);

// �����ʼ������󣬳�ʼ�����ൽ��������֮��ľ��룬��ʼ��unselected
pair<int, int> initDistance(vector<string>& sequences, int k, int distancemode);

int updateDistance(int premin, int n, int mode);

vector<string> sortFastaByClustering(vector<string>& sequences, int k, int clustermode, int distancemode);
#endif