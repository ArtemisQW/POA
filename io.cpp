#include "io.hpp"

vector<string> readFasta(const string& filename) {
    vector<string> sequences;  // ���ڴ洢��ȡ������
    ifstream infile(filename); // ���ļ�

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
    if (descending) { // �ӳ���������
        stable_sort(sequences.begin(), sequences.end(), [](const string& a, const string& b) {
            return a.size() > b.size();
            });
    }
    else { // �Ӷ̵�������
        stable_sort(sequences.begin(), sequences.end(), [](const string& a, const string& b) {
            return a.size() < b.size();
            });
    }
}

void writeMatrix(int** X, int row, int col, const string& filename) {
    ofstream outFile(filename);
    // ����ļ��Ƿ�ɹ���
    if (!outFile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    // ��������ļ�
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            outFile << setw(3) << X[i][j] << " ";  // ʹ�� setw ���ÿ��Ϊ 5
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

