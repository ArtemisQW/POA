#include "mtxutl.hpp"
#include "graph.hpp"
#include "io.hpp"
#include <unordered_map>
#include <cmath>
#include <queue>


#define infinity 2147483647
#define initWeight 1

using namespace std;

int g, h, match, mismatch;

int* Alignment(DAG& dag, const string& sequence, int mode, const string& filename) {
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
        Node cur = dag.nodes[i];

        // 初始化C[i][0]和I[i][0]
        int min = infinity;
        for (int k = 0; k < cur.pre.size(); k++) {
            int p = dag.id_node[cur.pre[k]];
            min = fmin(min, C[p][0]);
        }
        min = min == 0 ? t : min;
        C[i][0] = min + ::h;
        I[i][0] = C[i][0] + ::g;

        // 递推
        for (int j = 1; j <= n; j++) {

            I[i][j] = fmin(I[i][j - 1], C[i][j - 1] + ::g) + ::h;
            int w = cur.letter == sequence[j - 1] ? (::match) : (::mismatch);

            for (int k = 0; k < cur.pre.size(); k++) {
                int p = dag.id_node[cur.pre[k]];
                if (k == 0) {
                    D[i][j] = fmin(D[p][j], C[p][j] + ::g) + ::h;
                    C[i][j] = fmin(C[p][j - 1] + w, fmin(D[i][j], I[i][j]));
                }
                else {
                    D[i][j] = fmin(D[i][j], fmin(D[p][j], C[p][j] + ::g) + ::h);
                    C[i][j] = fmin(C[i][j], fmin(C[p][j - 1] + w, fmin(D[i][j], I[i][j])));
                }
            }
        }
    }

    int* align = AllocateIntVec(n + 1);
    memset(align, -1, (n + 1) * sizeof(int));
    align[0] = 0;

    int i = m - 1, j = n;

    if (mode == 1) writeMatrix(C, m, n + 1, filename);
    // printMatrix(D,m,n+1);
    // printMatrix(I,m,n+1);

    while (i > 0 && j > 0) {
        Node cur = dag.nodes[i];

        if (C[i][j] == I[i][j]) { // Seq[j]对应空位
            j--;
        }
        else {
            int w = cur.letter == sequence[j - 1] ? (::match) : (::mismatch);
            for (int k = 0; k < cur.pre.size(); k++) {
                int p = dag.id_node[cur.pre[k]];
                if (C[i][j] == C[p][j - 1] + w) {
                    align[j] = dag.nodes[i].id;
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

    FreeIntMtx(C);
    FreeIntMtx(D);
    FreeIntMtx(I);

    return align;
}

string HeaviestBundlingCons(DAG& dag) {
    int n = dag.nodes.size();   // 图G中的节点数量
    string Cons = "";           // 存储生成的共识序列

    int* score = AllocateIntVec(n);             // 创建存储节点得分的数组
    memset(score, 0, n * sizeof(int));          // 初始化节点得分为0

    int* D = AllocateIntVec(n);                 // 创建存储节点出度的数组
    for (int i = 0; i < n; i++) {                  // 记录每个节点的出度            
        Node cur = dag.nodes[i];
        D[cur.id] = cur.edges.size();
    }

    int* out = AllocateIntVec(n);               // 每个节点的最大权重后继节点
    memset(out, -1, n * sizeof(int));

    queue<int> Q;                               // 用于节点遍历的队列
    Q.push(1);                                  // 将节点E放入队列，节点E的id为1            


    while (!Q.empty()) {
        int cur = Q.front();
        Q.pop();
        Node curNode = dag.nodes[dag.id_node[cur]];

        int wmax = -1;

        for (int i = 0; i < curNode.edges.size(); i++) {
            int suc = curNode.edges[i].to;
            int w = curNode.edges[i].weight;
            if (w > wmax || (w == wmax && score[suc] > score[out[cur]])) {
                wmax = w;
                out[cur] = suc;                  // 设置最大权重的后继节点
            }
        }
        score[cur] += wmax + score[out[cur]];


        for (int i = 0; i < curNode.pre.size(); i++) {
            int p = curNode.pre[i];
            D[p] = D[p] - 1;
            if (D[p] == 0) {                     // p节点的后继节点已被遍历过
                Q.push(p);                       // p节点进入队列
            }
        }

    }


    int node = out[0];

    while (node != 1) {
        Cons += dag.nodes[dag.id_node[node]].letter;  // 根据最大权重后继系节点生成共有序列
        node = out[node];
    }

    // FreeIntVec(score);
    // FreeIntVec(D);
    // FreeIntVec(out);

    return Cons;
}



int main(int argc, char* argv[]) {

    // 检查参数个数是否正确
    if (argc != 8) {
        cerr << "Usage: " << argv[0]
            << " <int g> <int h> <int match> <int mismatch> <int mode> <input_filename> <output_foldername>" << endl;
        return 1;
    }

    // 解析参数
    ::g = stoi(argv[1]);                    // 第一个参数——间隙开启代价
    ::h = stoi(argv[2]);                    // 第二个参数——间隙延伸代价
    ::match = stoi(argv[3]);                // 第三个参数——匹配得分
    ::mismatch = stoi(argv[4]);             // 第四个参数——不匹配得分

    int mode = stoi(argv[5]);               // 输出模式参数
    string input_filename = argv[6];        // 输入文件名
    string output_foldername = argv[7];     // 输出文件夹名

    // 初始化 DAG
    DAG dag;
    vector<string> sequences = readFasta(input_filename);
    if (sequences.size() > 0) dag.init(sequences[0]);

    if (mode == 0) {
        for (int i = 1; i < sequences.size(); i++) {
            int* align = Alignment(dag, sequences[i] + 'E', mode, output_foldername + "/" + to_string(i) + ".txt");
            dag.construction(align, sequences[i].size() + 2, sequences[i]);
            FreeIntVec(align);
        }
        dag.writeDotFile(output_foldername + "/result.dot");
        writeCons(HeaviestBundlingCons(dag), output_foldername + "/consensus.txt");
    }
    else if (mode == 1) {
        for (int i = 1; i < sequences.size(); i++) {
            int* align = Alignment(dag, sequences[i] + 'E', mode, output_foldername + "/" + to_string(i) + ".txt");
            dag.construction(align, sequences[i].size() + 2, sequences[i]);
            FreeIntVec(align);
            dag.writeDotFile(output_foldername + "/" + to_string(i) + ".dot");
        }
    }
}

