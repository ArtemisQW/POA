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
    int weight;  // 边权
    int to;      // 边指向节点的id
};

struct Node {
    int id;               // 节点唯一标识
    char letter;          // 字符信息
    vector<int> pre;      // 前驱节点id
    vector<Edge> edges;   // 由该节点引出的边的集合

};

class DAG {
public:
    vector<Node> nodes;         // 图中所有节点的集合
    unordered_map<int, int> id_node;         // 节点id到集合下标的映射

    void init(const string& sequence);      // 输入一个序列进行图的初始化

    void refreshMap();      // 插入节点后刷新映射表

    void construction(int* align, int len, const string& sequence);     // 根据回溯结果，将新序列加入图中

    void printDag();

    void writeDotFile(const string& filename);      // 打印图
};

#endif