#ifndef __UPGMA__
#define __UPGMA__

#include <string>
#include "mtxutl.hpp"
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

vector<double>* initdistance;               // ��¼��ʼ�������
unordered_map<int, double>* newdistance;    // ��¼���ൽ��������֮��ľ���
set<int> unselected;                        // ��¼��δ���뵽���е����б��

double calculateDistance(const string& seq1, const string& seq2);

int calculatePos(int i, int j, int n);

// �����ʼ������󣬳�ʼ�����ൽ��������֮��ľ��룬��ʼ��unselected
pair<int, int> initDistance(vector<string>& sequences);

int updateDistance(int premin, int n);

vector<string> sortFastaByUPGMA(vector<string>& sequences);

#endif