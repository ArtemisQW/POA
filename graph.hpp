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
    int weight;     // 边权
    Node* to;       // 边指向节点的指针
};

struct Node {
    int id;                 // 节点唯一标识
    char letter;            // 字符信息
    vector<Node*> pre;      // 前驱节点集合
    vector<Edge*> edges;    // 由该节点引出的边的集合
    vector<Node*> aligned_node; // 和该节点等价的节点集合
};

class DAG {
public:
    vector<Node*> nodes;         // 图中所有节点的集合

    void init(const string& sequence);      // 输入一个序列进行图的初始化

    void construction(vector<Node*> alignPair, const string& sequence);     // 根据回溯结果，将新序列加入图中

    void printDag();

    void writeDotFile(const string& filename);      // 打印图
};

#endif