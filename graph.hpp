#ifndef __GRAPH__
#define __GRAPH__

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

struct Node;

struct Edge {
    int weight;     // ��Ȩ
    Node* to;       // ��ָ��ڵ��ָ��
};

struct Node {
    int id;                 // �ڵ�Ψһ��ʶ
    char letter;            // �ַ���Ϣ
    vector<Node*> pre;      // ǰ���ڵ㼯��
    vector<Edge*> edges;    // �ɸýڵ������ıߵļ���
    vector<Node*> aligned_node; // �͸ýڵ�ȼ۵Ľڵ㼯��
};

class DAG {
public:
    vector<Node*> nodes;         // ͼ�����нڵ�ļ���

    void init(const string& sequence);      // ����һ�����н���ͼ�ĳ�ʼ��

    void construction(vector<Node*> alignPair, const string& sequence);     // ���ݻ��ݽ�����������м���ͼ��

    void printDag();

    void writeDotFile(const string& filename);      // ��ӡͼ
};

#endif