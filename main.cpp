#include "mtxutl.hpp"
#include "graph.hpp"
#include "io.hpp"
#include <unordered_map>
#include <cmath>
#include <queue>

using namespace std;

int g, h, match, mismatch;

vector<Node*> Alignment(DAG& dag, const string& sequence, int mode, const string& filename) {
    unordered_map<Node*, int> nodePosition;
    for (int i = 0; i < dag.nodes.size(); i++) {
        nodePosition[dag.nodes[i]] = i;
    }

    int m = dag.nodes.size();
    int n = sequence.size();


    int** C = AllocateIntMtx(m, n + 1);
    int** D = AllocateIntMtx(m, n + 1);
    int** I = AllocateIntMtx(m, n + 1);

    C[0][0] = 0;
    int t = ::g;
    for (int j = 1; j <= n; j++) {
        t += ::h;
        C[0][j] = t;
        D[0][j] = t + (::g);
    }

    t = ::g;

    for (int i = 1; i < m; i++) {
        Node* curNode = dag.nodes[i];

        // ��ʼ��C[i][0]��I[i][0]
        int min = infinity;
        for (Node* preNode : curNode->pre) {
            min = fmin(min, C[nodePosition[preNode]][0]);
        }
        min = min == 0 ? t : min;
        C[i][0] = min + ::h;
        I[i][0] = C[i][0] + ::g;

        // ����
        for (int j = 1; j <= n; j++) {

            I[i][j] = fmin(I[i][j - 1], C[i][j - 1] + ::g) + ::h;
            int w = curNode->letter == sequence[j - 1] ? (::match) : (::mismatch);

            int k = 0;
            for (Node* preNode : curNode->pre) {
                int p = nodePosition[preNode];
                if (k == 0) {
                    D[i][j] = fmin(D[p][j], C[p][j] + ::g) + ::h;
                    C[i][j] = fmin(C[p][j - 1] + w, fmin(D[i][j], I[i][j]));
                }
                else {
                    D[i][j] = fmin(D[i][j], fmin(D[p][j], C[p][j] + ::g) + ::h);
                    C[i][j] = fmin(C[i][j], fmin(C[p][j - 1] + w, fmin(D[i][j], I[i][j])));
                }
                k++;
            }
        }
    }

    vector<Node*> alignPair;

    int i = m - 1, j = n;

    if (mode == 1) writeMatrix(C, m, n + 1, filename);
    // printMatrix(D,m,n+1);
    // printMatrix(I,m,n+1);

    while (i > 0 && j > 0) {
        Node* curNode = dag.nodes[i];

        if (C[i][j] == I[i][j]) { // Seq[j]��Ӧ��λ
            alignPair.insert(alignPair.begin(), nullptr);
            j--;
        }
        else {
            int w = curNode->letter == sequence[j - 1] ? (::match) : (::mismatch);
            for (Node* preNode : curNode->pre) {
                int p = nodePosition[preNode];
                if (C[i][j] == C[p][j - 1] + w) {
                    alignPair.insert(alignPair.begin(), dag.nodes[i]);
                    i = p;
                    j--;
                    break;
                }
                else if (C[i][j] == D[i][j] && (D[i][j] == D[p][j] + ::h || D[i][j] == C[p][j] + ::g + ::h)) {
                    i = p;
                    break;
                }
            }

        }
    }
    alignPair.insert(alignPair.begin(), dag.nodes[0]);
    FreeIntMtx(C);
    FreeIntMtx(D);
    FreeIntMtx(I);

    return alignPair;
}

string HeaviestBundlingCons(DAG& dag) {
    int n = dag.nodes.size();   // ͼG�еĽڵ�����
    string Cons = "";           // �洢���ɵĹ�ʶ����

    int* score = AllocateIntVec(n);             // �����洢�ڵ�÷ֵ�����
    memset(score, 0, n * sizeof(int));          // ��ʼ���ڵ�÷�Ϊ0

    int* D = AllocateIntVec(n);                 // �����洢�ڵ���ȵ�����
    for (int i = 0; i < n; i++) {                  // ��¼ÿ���ڵ�ĳ���            
        Node* curNode = dag.nodes[i];
        D[curNode->id] = curNode->edges.size();
    }

    vector<Node*> out;                           // ÿ���ڵ�����Ȩ�غ�̽ڵ�           
    for (int i = 0; i < n; i++) {
        out.push_back(nullptr);
    }

    queue<Node*> Q;                               // ���ڽڵ�����Ķ���
    Node* E = dag.nodes[dag.nodes.size() - 1];
    Q.push(E);                                  // ���ڵ�E������У��ڵ�E��idΪ1            


    while (!Q.empty()) {
        Node* curNode = Q.front();
        Q.pop();
        int cur = curNode->id;
       
        int wmax = -1;

        for (int i = 0; i < curNode->edges.size(); i++) {
            Node* sucNode = curNode->edges[i]->to;
            int suc = sucNode->id;
            int w = curNode->edges[i]->weight;
            if (w > wmax || (w == wmax && score[suc] > score[out[cur]->id])) {
                wmax = w;
                out[cur] = sucNode;                  // �������Ȩ�صĺ�̽ڵ�
            }
        }
        if(cur != 1) score[cur] += wmax + score[out[cur]->id];


        for (int i = 0; i < curNode->pre.size(); i++) {
            Node* preNode = curNode->pre[i];
            int p = preNode->id;
            D[p] = D[p] - 1;
            if (D[p] == 0) {                     // p�ڵ�ĺ�̽ڵ��ѱ�������
                Q.push(preNode);                       // p�ڵ�������
            }
        }

    }


    Node* node = out[0];

    while (node != E) {
        Cons += node->letter;  // �������Ȩ�غ��ϵ�ڵ����ɹ�������
        node = out[node->id];
    }

    // FreeIntVec(score);
    // FreeIntVec(D);
    // FreeIntVec(out);

    return Cons;
}



int main(int argc, char* argv[]) {

    // �����������Ƿ���ȷ
    if (argc != 8) {
        cerr << "Usage: " << argv[0]
            << " <int g> <int h> <int match> <int mismatch> <int mode> <input_filename> <output_foldername>" << endl;
        return 1;
    }

    // ��������
    ::g = stoi(argv[1]);                    // ��һ������������϶��������
    ::h = stoi(argv[2]);                    // �ڶ�������������϶�������
    ::match = stoi(argv[3]);                // ��������������ƥ��÷�
    ::mismatch = stoi(argv[4]);             // ���ĸ�����������ƥ��÷�

    int mode = stoi(argv[5]);               // ���ģʽ����
    string input_filename = argv[6];        // �����ļ���
    string output_foldername = argv[7];     // ����ļ�����

    // ��ʼ�� DAG
    DAG dag;
    vector<string> sequences = readFasta(input_filename);
    if (sequences.size() > 0) dag.init(sequences[0]);

    if (mode == 0) {
        for (int i = 1; i < sequences.size(); i++) {
            vector<Node*> alignPair = Alignment(dag, sequences[i] + 'E', mode, output_foldername + "/" + to_string(i) + ".txt");
            dag.construction(alignPair, sequences[i]);
        }
        dag.writeDotFile(output_foldername + "/result.dot");
        writeCons(HeaviestBundlingCons(dag), output_foldername + "/consensus.txt");
    }
    else if (mode == 1) {
        for (int i = 1; i < sequences.size(); i++) {
            vector<Node*> alignPair = Alignment(dag, sequences[i] + 'E', mode, output_foldername + "/" + to_string(i) + ".txt");
            dag.construction(alignPair, sequences[i]);
            dag.writeDotFile(output_foldername + "/" + to_string(i) + ".dot");
        }
    }
}

