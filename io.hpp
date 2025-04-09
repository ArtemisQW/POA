#ifndef _IO_
#define _IO_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <filesystem> 
#include <algorithm>


using namespace std;


vector<string> readFasta(const string& filename);

void sortFastaByLength(vector<string>& sequences, bool descending);

void writeMatrix(int** X, int row, int col, const string& filename);

void writeCons(const string& cons, const string& filename);

void printVec(int* array, int len);

void printMatrix(int** X, int row, int col);

bool createDirectory(const std::string& path);

#endif 
