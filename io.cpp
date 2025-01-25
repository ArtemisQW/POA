#include "io.hpp"

vector<string> readFasta(const string& filename) {
    vector<string> sequences;  // 用于存储读取的序列
    ifstream infile(filename); // 打开文件

    if (!infile.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return sequences;
    }

    string line;
    string curSequence;

    while (getline(infile, line)) {
        if (line.empty()) {
            continue;
        }

        if (line[0] == '>') {
            if (!curSequence.empty()) {
                sequences.push_back(curSequence);
                curSequence.clear();
            }
        }
        else {
            curSequence += line;
        }
    }

    if (!curSequence.empty()) {
        sequences.push_back(curSequence);
    }

    infile.close();
    // printf("read file finished");
    return sequences;
}

void sortFastaByLength(vector<string>& sequences, bool descending) {
    if (descending) { // 从长到短排序
        stable_sort(sequences.begin(), sequences.end(), [](const string& a, const string& b) {
            return a.size() > b.size();
            });
    }
    else { // 从短到长排序
        stable_sort(sequences.begin(), sequences.end(), [](const string& a, const string& b) {
            return a.size() < b.size();
            });
    }
}

void writeMatrix(int** X, int row, int col, const string& filename) {
    ofstream outFile(filename);
    // 检查文件是否成功打开
    if (!outFile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    // 输出矩阵到文件
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            outFile << setw(3) << X[i][j] << " ";  // 使用 setw 设置宽度为 5
        }
        outFile << endl;
    }

    outFile << endl;
    outFile.close();
}

void writeCons(const string& cons, const string& filename) {
    ofstream outfile(filename);

    if (!outfile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    outfile << cons;
    outfile.close();
}

void printVec(int* array, int len) {
    for (int i = 0; i < len; i++) {
        cout << array[i] << ' ';
    }
    cout << endl;
}

void printMatrix(int** X, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cout << X[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

