#include "graph.hpp"

void DAG::init(const string& sequence) {
    int n = sequence.size();

    Node* preNode = new Node{ 0, 'S', {}, {}, {} };
    nodes.push_back(preNode);


    for (int i = 0; i < n; i++) {

        Node* currentNode = new Node{ i+2, sequence[i], {preNode}, {}, {} };
        nodes.push_back(currentNode);
        Edge* e = new Edge{ initWeight, currentNode};
        preNode->edges.push_back(e);
        preNode = currentNode;

    }
    Node* currentNode = new Node{ 1, 'E', {preNode}, {}, {} };
    nodes.push_back(currentNode);
    Edge* e = new Edge{ initWeight, currentNode };
    preNode->edges.push_back(e);

}
//bool DAG::contain(Node* a, Node* b) {
//    if (a == b) return true;
//    for (Node* alignnode : b->aligned_node) {
//        if (a == alignnode) return true;
//    }
//    return false;
//}

void DAG::construction(vector<Node*> alignPair, const string& sequence) {
    int pos = 1;
    //for (int i = 1; i < alignPair.size() - 1; i++) {
    for (int i = 1; i < sequence.size() + 1; i++) {
        if (alignPair[i] == nullptr) { // 插入
            int id = nodes.size();
            Node* newNode = new Node{ id, sequence[i - 1], {}, {}, {} };
            nodes.insert(nodes.begin() + pos, newNode);
            alignPair[i] = newNode;
        }
        else if (sequence[i - 1] != alignPair[i]->letter) { // 错配
            int flag = 0;
            for (Node* alignnode : alignPair[i]->aligned_node) { // 检查是否已经错配过
                if (alignnode->letter == sequence[i - 1]) { // 错配过
                    alignPair[i] = alignnode;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) { // 没错配过 // 容易出错的重点逻辑
                int id = nodes.size();
                vector<Node*> vec(alignPair[i]->aligned_node);
                vec.push_back(alignPair[i]);
                Node* newNode = new Node{ id, sequence[i - 1], {}, {}, vec };
                nodes.insert(nodes.begin() + pos, newNode);
                int aligned_size = alignPair[i]->aligned_node.size();
                for (int j = 0; j < aligned_size; j++) {
                    alignPair[i]->aligned_node[j]->aligned_node.push_back(newNode);
                }
                alignPair[i]->aligned_node.push_back(newNode);
                /*for (Node* node : alignPair[i]->aligned_node) {
                    node->aligned_node.push_back(newNode);
                }*/
                alignPair[i] = newNode;
            }
        }
        int offset = 1;
        //while ((i+offset < alignPair.size()) && alignPair[i + offset] == nullptr) offset++;
        while ((i+offset < sequence.size()+2) && alignPair[i + offset] == nullptr) offset++;
        while (alignPair[i + offset] != nodes[pos]) pos++;

        //if (i + offset == alignPair.size()) {
        //    pos = nodes.size();
        //}
        //else {
        //    //while (alignPair[i + offset] != nodes[pos]) pos++;
        //    for (int k = 0; k < nodes.size(); k++) {
        //        if (contain(nodes[k], alignPair[i + offset])) {
        //            pos = k;
        //            break;
        //        }
        //    }
        //}


       /* int minpos = 0;
        while (alignPair[i + offset] != nodes[pos+minpos]) minpos++;
        for (Node* alignnode : alignPair[i + offset]->aligned_node) {
            int x = 0;
            while (alignnode != nodes[pos + x]) x++;
            minpos = fmin(x, minpos);
        }
        pos += minpos;*/

    }
  
    //for (int i = 0; i < alignPair.size() - 1; i++) {   
    for (int i = 0; i < sequence.size() + 1; i++) { 
        Node* curNode = alignPair[i];
        Node* sucNode = alignPair[i + 1];
        Edge* e = nullptr;
        for (int i = 0; i < curNode->edges.size(); i++) {
            if (curNode->edges[i]->to == sucNode) {
                e = curNode->edges[i];
                break;
            }
        }
        if (e == nullptr) {
            curNode->edges.push_back(new Edge{ initWeight, sucNode });
            sucNode->pre.push_back(curNode);
        }
        else {
            e->weight++;
        }
    }

    //unordered_map<Node*, int> nodePosition;
    //for (int i = 0; i < nodes.size(); i++) {
    //    nodePosition[nodes[i]] = i;
    //}
    //for (int i = alignPair.size() - 1; i > 0; i--) {
    //    Node* curNode = alignPair[i];
    //    Node* preNode = alignPair[i - 1];
    //    int curNodePos = nodePosition[curNode];
    //    int preNodePos = nodePosition[preNode];
    //    if (curNodePos <  preNodePos) {
    //        nodes.erase(nodes.begin() + curNodePos);
    //        nodes.insert(nodes.begin() + preNodePos, curNode);
    //        for (int j = 0; j < nodes.size(); j++) {
    //            nodePosition[nodes[j]] = j;
    //        }
    //    }
    //}


    // **拓扑排序调整 nodes 顺序**
    vector<Node*> sortedNodes;
    unordered_map<Node*, int> inDegree;
    queue<Node*> q;

    // 统计入度
    for (Node* node : nodes) {
        inDegree[node] = node->pre.size();
        if (inDegree[node] == 0) {
            q.push(node);
        }
    }

    // Kahn 拓扑排序
    while (!q.empty()) {
        Node* node = q.front();
        q.pop();
        sortedNodes.push_back(node);
        for (Edge* edge : node->edges) {
            Node* nextNode = edge->to;
            if (--inDegree[nextNode] == 0) {
                q.push(nextNode);
            }
        }
    }

    // 替换原来的 nodes
    nodes = sortedNodes;


}

void DAG::printDag() {
    for (int i = 0; i < nodes.size(); ++i) {
        cout << "Node " << i << " (" << nodes[i]->letter << "): id" << nodes[i]->id << ' ';
        cout << "pre:";
        for (Node* pre : nodes[i]->pre) {
            cout << pre->id << " ";
        }
        cout << "  ";
        cout << "next:";
        for (Edge* next : nodes[i]->edges) {
            cout << next->to->id<< ":" << next->weight << " ";
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
        outFile << "    " << node->id << " [label=<<FONT COLOR=\"red\">" << node->id
            << "</FONT><BR/>" << node->letter << ">];\n";
    }

    // 写入边
    for (const auto& node : nodes) {
        for (const auto& edge : node->edges) {
            outFile << "    " << node->id << " -> " << edge->to->id << " [label=\"" << edge->weight << "\"];\n";
        }
    }

    outFile << "}\n";
    outFile.close();
    //cout << "DOT file written to: " << filename << endl;
}

