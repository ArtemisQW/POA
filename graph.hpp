#ifndef __POA__
#define __POA__

#include "mtxutl.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string.h>
#include <fstream>

#define infinity 2147483647
#define initWeight 1

using namespace std;

struct Edge {
    int weight;  // ��Ȩ
    int to;      // ��ָ��ڵ��id
};

struct Node {
    int id;               // �ڵ�Ψһ��ʶ
    char letter;          // �ַ���Ϣ
    vector<int> pre;      // ǰ���ڵ�id
    vector<Edge> edges;   // �ɸýڵ������ıߵļ���

};

class DAG {
public:
    vector<Node> nodes;         // ͼ�����нڵ�ļ���
    unordered_map<int, int> id_node;         // �ڵ�id�������±��ӳ��

    void init(const string& sequence);      // ����һ�����н���ͼ�ĳ�ʼ��

    void refreshMap();      // ����ڵ��ˢ��ӳ���

    void construction(int* align, int len, const string& sequence);     // ���ݻ��ݽ�����������м���ͼ��

    void printDag();

    void writeDotFile(const string& filename);      // ��ӡͼ
};

#endif