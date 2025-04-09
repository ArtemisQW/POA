#include "mtxutl.hpp"
#include "graph.hpp"
#include "io.hpp"
#include "upgma.hpp"
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <queue>
#include <filesystem>

using namespace std;

int g = 6, h = 2, match = 0, mismatch = 4;

bool isGap(Node* curnode, Node* prenode) {
    for (Node* node : curnode->pre) {
        if (node == prenode) return false;
    }
    return true;
}

//int calculateScore(const vector<Node*>& alignPair, const string& sequence) {
//    int score = 0;
//    bool in_gap = false; // ����Ƿ��� gap ����
//
//    for (int i = 0; i < alignPair.size(); i++) {
//        if (alignPair[i+1] == nullptr || (alignPair[i] != nullptr && isGap(alignPair[i+1],alignPair[i]))) {  // gap
//            if (!in_gap) {
//                score += ::g + ::h; 
//                in_gap = true; 
//            }
//            else {
//                score += ::h; 
//            }
//        }
//        else {
//            in_gap = false; // �˳� gap
//            if (alignPair[i+1]->letter == sequence[i]) {
//                score += ::match;  
//            }
//            else {
//                score += ::mismatch;
//            }
//        }
//    }
//    return score;
//}

// calculateScore ������� construction ֮ǰ
int calculateScore(const vector<Node*>& alignPair, const string& sequence) {
    int score = 0;
    bool in_gap = false; // ����Ƿ��� gap ����
    int pos = sequence.length();
    for (int i = 0; i < sequence.length() - 1; i++) {
        if (alignPair[i] == nullptr) {  // gap
            if (!in_gap) {
                score += ::g + ::h;
                in_gap = true;
            }
            else {
                score += ::h;
            }
        }
        else {
            in_gap = false; // �˳� gap
            if (alignPair[i]->letter == sequence[i]) {
                score += ::match;
            }
            else {
                score += ::mismatch;
            }
        }

        if (alignPair[i] != nullptr && alignPair[i + 1] != nullptr && isGap(alignPair[i + 1], alignPair[i])) {
            score += ::g;
            while (isGap(alignPair[i + 1], alignPair[pos])) {
                pos++;
                score += ::h;
            }
            pos++;
            score += ::h;
        }
    }

    return score;
}

vector<Node*> Alignment(DAG& dag, const string& sequence, bool score_output, const string& filename) {
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
            // cout << nodePosition[preNode] << endl;
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
                    k = 1;
                }
                else {
                    D[i][j] = fmin(D[i][j], fmin(D[p][j], C[p][j] + ::g) + ::h);
                    C[i][j] = fmin(C[i][j], fmin(C[p][j - 1] + w, fmin(D[i][j], I[i][j])));
                }
            }
        }
    }

    vector<Node*> alignPair;
    int i = m - 1, j = n;
    if (score_output) cout << C[i][j] << endl;
    // writeMatrix(C, m, n + 1, filename);  // ���C����
    // printMatrix(D,m,n+1);
    // printMatrix(I,m,n+1);

    // type = 0 -> C  type = 1 -> D  type = 2 -> I
    int type = 0;
    int offset = 0;
    while (i > 0 && j > 0) {
        Node* curNode = dag.nodes[i];
        switch (type) {
        case 0: {
            int flag = 0;

            int w = curNode->letter == sequence[j - 1] ? (::match) : (::mismatch);
            for (Node* preNode : curNode->pre) {

                int p = nodePosition[preNode];
                if (C[i][j] == C[p][j - 1] + w) {
                    alignPair.insert(alignPair.begin(), dag.nodes[i]);
                    i = p;
                    j--;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                if (C[i][j] == D[i][j]) type = 1;
                else if (C[i][j] == I[i][j]) type = 2;
            }
            break;
        }
        case 1:
            
            for (Node* preNode : curNode->pre) {
                int p = nodePosition[preNode];
                if (D[i][j] == D[p][j] + ::h) {
                    alignPair.insert(alignPair.end() - offset, dag.nodes[i]);
                    offset++;
                    type = 1;
                    i = p;
                    break;
                }
                if (D[i][j] == C[p][j] + ::h + ::g) {
                    alignPair.insert(alignPair.end() - offset, dag.nodes[i]);
                    offset++;
                    type = 0;
                    i = p;
                    break;
                }
            }
            break;
        case 2:
            alignPair.insert(alignPair.begin(), nullptr);
            if (I[i][j] != I[i][j-1] + ::h) {
                type = 0;
            }
            j--;
            break;
        }
    }

    //while (i > 0 && j > 0) {
    //    Node* curNode = dag.nodes[i];

    //    if (C[i][j] == I[i][j]) { // Seq[j]��Ӧ��λ
    //        alignPair.insert(alignPair.begin(), nullptr);
    //        j--;
    //    }
    //    else {
    //        int w = curNode->letter == sequence[j - 1] ? (::match) : (::mismatch);
    //        for (Node* preNode : curNode->pre) {
    //            int p = nodePosition[preNode];
    //            if (C[i][j] == C[p][j - 1] + w) {
    //                alignPair.insert(alignPair.begin(), dag.nodes[i]);
    //                i = p;
    //                j--;
    //                break;
    //            }
    //            else if (C[i][j] == D[i][j] && (D[i][j] == D[p][j] + ::h || D[i][j] == C[p][j] + ::g + ::h)) {
    //                i = p;
    //                break;
    //            }

    //        }

    //    }
    //}
    while (i > 0) { 
        for (Node* preNode : dag.nodes[i]->pre) {
            int p = nodePosition[preNode];
            if (C[i][j] == C[p][j] + ::h || C[i][j] == C[p][j] + ::h + ::g) {
                alignPair.insert(alignPair.end() - offset, dag.nodes[i]);
                offset++;
                type = 1;
                i = p;
                break;
            }
        }
    }
    while (j > 0) { alignPair.insert(alignPair.begin(), nullptr); --j; }
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



//int main(int argc, char* argv[]) {
//
//    // �����������Ƿ���ȷ
//    if (argc != 8) {
//        cerr << "Usage: " << argv[0]
//            << " <int g> <int h> <int match> <int mismatch> <int mode> <input_filename> <output_foldername>" << endl;
//        return 1;
//    }
//
//    // ��������
//    ::g = stoi(argv[1]);                    // ��һ������������϶��������
//    ::h = stoi(argv[2]);                    // �ڶ�������������϶�������
//    ::match = stoi(argv[3]);                // ��������������ƥ��÷�
//    ::mismatch = stoi(argv[4]);             // ���ĸ�����������ƥ��÷�
//
//    int mode = stoi(argv[5]);               // ���ģʽ����
//    string input_filename = argv[6];        // �����ļ���
//    string output_foldername = argv[7];     // ����ļ�����
//
//    // ��ʼ�� DAG
//    DAG dag;
//    vector<string> sequences = readFasta(input_filename);
//    sequences = sortFastaByClustering(sequences, 6, 0,0);
//    // sortFastaByLength(sequences, false);
//    if (sequences.size() > 0) dag.init(sequences[0]);
//
//    if (mode == 0) {
//        for (int i = 1; i < sequences.size(); i++) {
//            vector<Node*> alignPair = Alignment(dag, sequences[i] + 'E', mode, output_foldername + "/" + to_string(i) + ".txt");
//            cout << "score��" << calculateScore(alignPair, 'S' + sequences[i] + 'E') << endl;
//            dag.construction(alignPair, sequences[i]);
//        }
//        dag.writeDotFile(output_foldername + "/result.dot");
//        writeCons(HeaviestBundlingCons(dag), output_foldername + "/consensus.txt");
//    }
//    else if (mode == 1) {
//        for (int i = 1; i < sequences.size(); i++) {
//            vector<Node*> alignPair = Alignment(dag, sequences[i] + 'E', mode, output_foldername + "/" + to_string(i) + ".txt");
//            cout << "score��" << calculateScore(alignPair, 'S' + sequences[i] + 'E') << endl; 
//            dag.construction(alignPair, sequences[i]);
//            dag.writeDotFile(output_foldername + "/" + to_string(i) + ".dot");
//        }
//    }
//}

void Help() {
    cout <<
        "usage: POA <input.fasta> <output.fasta> [options ...]\n"
        "\n"
        "  <input.fasta>\n"
        "      �����ļ���FASTA ��ʽ�������ļ���������Ϊ��һ������\n"
        "  <output.fasta>\n"
        "      �����ʶ���е�FASTA�ļ������ļ�������������Ϊ�ڶ�������\n"
        "\n"
        "  options:\n"
        "    -g <int>\n"
        "      default: 6\n"
        "      ��϶�������ۣ�����Ϊ�Ǹ�������\n"
        "    -h <int>\n"
        "      default: 2\n"
        "      ��϶������ۣ�����Ϊ�Ǹ�������\n"
        "    -m <int>\n"
        "      default: 0\n"
        "      ƥ�����������Ϊ�Ǹ�������\n"
        "    -mis <int>\n"
        "      default: 4\n"
        "      �������������Ϊ�Ǹ�������\n"
        "    -dot <path>\n"
        "      ���DOT��ʽͼ�ļ���·�����ļ��б�����ڣ�\n"
        "    -score\n"
        "      ����ȶԹ��̵����յ÷֣�����Ҫ���������\n"
        "    -s <int> [<int>]\n"
        "      ������������ʽ����ѡһ����������������\n"
        "        1 - �����г�����������\n"
        "        2 - �����г��Ƚ�������\n"
        "        3 - ʹ��k-mer������룬UPGMA����\n"
        "        4 - ʹ��LLCS������룬UPGMA����\n"
        "        5 - ʹ��k-mer������룬single linkage����\n"
        "        6 - ʹ��LLCS������룬single linkage����\n"
        "      �����򷽷�Ϊ3��5����ѡ��2����������ָ��k-mer��kֵ\n"
        "    -help\n"
        "      ��ӡ������Ϣ\n"
        "\n"
        "  ʾ��:\n"
        "    POA input.fasta output.fasta -g 5 -h 2 -s 3 6 -dot result.dot -score\n"
        "\n";

}

int main(int argc, char* argv[]) {
    if(argc < 2) cerr << "The number of parameters is incorrect" << endl;
    else if (argc == 2) {
        string test = argv[1];
        if (test == "-help") {
            Help();
            return 1;
        }
        else cerr << "The number of parameters is incorrect" << endl;
    }
    
    // Ĭ�ϲ���
    string input_filename = argv[1];        // �����ļ���
    string consensus_output = argv[2];      // �����ʶ�����ļ���
    string dot_output = "";
    int sort_method = 0;                   // ������Ĭ��ֵ
    int k = 6;                             // k-mer��k
    bool score_output = false;
    bool isdot_output = false;
    bool issort = false;
    // ��������
    for (int i = 3; i < argc; i++) {
        string arg = argv[i];
       
        if (arg == "-score") {
            score_output = true;
        }
        else if (arg == "-g" && i + 1 < argc) {
            int x = stoi(argv[++i]);
            if (x < 0) {
                cerr << "Error: gap_open is non-negative" << endl; 
                return -1;
            }
            ::g = x;
        }
        else if (arg == "-h" && i + 1 < argc) {
            int x = stoi(argv[++i]);
            if (x < 0) { 
                cerr << "Error: gap_extend is non-negative" << endl; 
                return -1; 
            }
            ::h = x;
        }
        else if (arg == "-m" && i + 1 < argc) {
            int x = stoi(argv[++i]);
            if (x < 0) {
                cerr << "Error: match is non-negative" << endl;
                return -1;
            }
               
            ::match = x;
        }
        else if (arg == "-mis" && i + 1 < argc) {
            int x = stoi(argv[++i]);
            if (x < 0) {
                cerr << "Error: mismatch is non-negative" << endl;
                return -1;
            }
            ::mismatch = x;
        }
        else if (arg == "-s" && i + 1 < argc) {
            issort = true;
            sort_method = stoi(argv[++i]);
            if (sort_method < 1 || sort_method > 6) {
                cerr << "Error: The sorting method number should be between 1 and 6" << endl;
                return 1;
            }

            if ((sort_method == 3 || sort_method == 5) && i + 1 < argc && argv[i + 1][0] != '-') {
                k = stoi(argv[++i]);
                if (k <= 0) {
                    cerr << "Error: k must be a positive integer." << endl;
                    return 1;
                }
            }
        }
        else if (arg == "-dot" && i + 1 < argc) {
            isdot_output = true;
            dot_output = argv[++i];
        }
        else {
            std::cerr << "unknown parameter:" << arg << std::endl;
            return 1;
        }
    }


  
    // ���ж���
    DAG dag;
    vector<string> sequences = readFasta(input_filename);
    
    // ����
    if (issort) {
        switch (sort_method) {
        case 1:
            cout << "Sort by sequence length in descending order" << endl;
            sortFastaByLength(sequences, false);
            break;
        case 2:
            cout << "Sort by sequence length in ascending order" << endl;
            sortFastaByLength(sequences, true);
            break;
        case 3:
            cout << "Sort by k-mer+UPGMA" << endl;
            sequences = sortFastaByClustering(sequences, k, 0, 0);
            break;
        case 4:
            cout << "Sort by LLCS+UPGMA" << endl;
            sequences = sortFastaByClustering(sequences, k, 0, 1);
            break;
        case 5:
            cout << "Sort by k-mer+single_linkage" << endl;
            sequences = sortFastaByClustering(sequences, k, 1, 0);
            break;
        case 6:
            cout << "Sort by LLCS+UPGMA" << endl;
            sequences = sortFastaByClustering(sequences, k, 1, 1);
            break;
        default:
            cerr << "Error: The sorting method number should be between 1 and 6" << endl;
            return 1;
            break;
        }
    }
    else {
        cout << "Sequences unsorted" << endl;
    }
    
    // ��ʼ��
    if (sequences.size() > 0) dag.init(sequences[0]);


    for (int i = 1; i < sequences.size(); i++) {
        vector<Node*> alignPair = Alignment(dag, sequences[i] + 'E', score_output, "");
        dag.construction(alignPair, sequences[i]);
    }
    if(isdot_output) dag.writeDotFile(dot_output);
    writeCons(HeaviestBundlingCons(dag), consensus_output);
 
    

    return 0;
}