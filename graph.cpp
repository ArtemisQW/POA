#include "graph.hpp"

void DAG::init(const string& sequence) {
    int n = sequence.size();

    nodes.push_back({ 0,'S', {},{{initWeight,2}} });
    int id = 1;

    for (int i = 0; i < n; i++) {
        id++;
        int pre = i == 0 ? 0 : id - 1;
        int next = i == n - 1 ? 1 : id + 1;
        nodes.push_back({ id,sequence[i],{pre},{{initWeight,next}} });

    }
    nodes.push_back({ 1,'E', {id},{} });
    refreshMap();
}

void DAG::refreshMap() {
    for (int i = 0; i < nodes.size(); i++) {
        id_node[nodes[i].id] = i;
    }
}

void DAG::construction(int* align, int len, const string& sequence) {
    int next = nodes.size() - 1;
    for (int i = len - 2; i > 0; i--) {
        if (align[i] == -1 || sequence[i - 1] != nodes[id_node[align[i]]].letter) {
            int id = nodes.size();
            int offset = align[i] == -1 ? next : id_node[align[i]];

            nodes.insert(nodes.begin() + offset, { id,sequence[i - 1], {},{} });
            align[i] = id;
            next = offset;
        }
        else {
            next = id_node[align[i]];
        }
    }
    refreshMap();
    // printVec(align,len);
    for (int i = 0; i < len - 1; i++) {
        int cur = align[i];
        int suc = align[i + 1];
        Node* curNode = &nodes[id_node[cur]];
        Edge* p = nullptr;

        for (int i = 0; i < curNode->edges.size(); i++) {
            if (curNode->edges[i].to == suc) {
                p = &curNode->edges[i];
                break;
            }
        }

        if (p == nullptr) {
            curNode->edges.push_back({ initWeight,suc });
            nodes[id_node[suc]].pre.push_back(cur);
        }
        else {
            p->weight++;
        }
    }
}

void DAG::printDag() {
    for (int i = 0; i < nodes.size(); ++i) {
        cout << "Node " << i << " (" << nodes[i].letter << "): id" << nodes[i].id << ' ';
        cout << "pre:";
        for (int pre : nodes[i].pre) {
            cout << pre << " ";
        }
        cout << "  ";
        cout << "next:";
        for (Edge next : nodes[i].edges) {
            cout << next.to << ":" << next.weight << " ";
        }
        cout << endl;
    }
    cout << endl;
}



void DAG::writeDotFile(const string& filename) {
    ofstream outFile(filename);

    if (!outFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    outFile << "digraph G {\n";
    outFile << "    rankdir=LR;\n"; // 从左到右布局
    outFile << "    node [shape=circle, style=filled, fillcolor=lightgray, fontcolor=black];\n";

    // 写入节点
    for (const auto& node : nodes) {
        outFile << "    " << node.id << " [label=<<FONT COLOR=\"red\">" << node.id
            << "</FONT><BR/>" << node.letter << ">];\n";
    }

    // 写入边
    for (const auto& node : nodes) {
        for (const auto& edge : node.edges) {
            outFile << "    " << node.id << " -> " << edge.to << " [label=\"" << edge.weight << "\"];\n";
        }
    }

    outFile << "}\n";
    outFile.close();
    //cout << "DOT file written to: " << filename << endl;
}

