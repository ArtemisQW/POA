#include"upgma.hpp"

double calculateDistance(const string& seq1, const string& seq2) {
	
}

int calculatePos(int i, int j, int n) {
	if (i > j) swap(i, j);
	return ((2 * n - i - 1) * i) / 2 + j - i - 1;
}

// 计算初始距离矩阵，初始化聚类到各个序列之间的距离，初始化unselected
pair<int, int> initDistance(vector<string>& sequences) {
	int n = sequences.size();
	for (int i = 0; i < n; i++) unselected.insert(i);
	pair<int, int> p;
	initdistance = new vector<double>();
	double min = INFINITY;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			double d = calculateDistance(sequences[i], sequences[j]);
			(*initdistance).push_back(d);
			if (d < min) {
				min = d;
				p.first = i;
				p.second = j;
			}

		}

	}

	unselected.erase(p.first);
	unselected.erase(p.second);

	unordered_map<int, double>* updatedistance = new unordered_map<int, double>();
	for (int i = 0; i < n; i++) {
		if (i != p.first) (*updatedistance).insert({ i,(*initdistance)[calculatePos(i,p.first,n)] });
	}
	newdistance = updatedistance;

	return p;
}

int updateDistance(int premin, int n) {
	unordered_map<int, double>* updatedistance = new unordered_map<int, double>();
	int size = n - (*newdistance).size();
	int min = -1;
	for (int i : unselected) {
		(*updatedistance)[i] = ((*newdistance)[i] * size + (*initdistance)[calculatePos(i, premin, n)]) / (size + 1);
		if (min < 0 || (*updatedistance)[i] < (*updatedistance)[min]) min = i;
	}
	newdistance = updatedistance;
	return min;
}

vector<string> sortFastaByUPGMA(vector<string>& sequences) {
	vector<string> ans;

	// 初始化距离矩阵
	pair<int, int> min = initDistance(sequences);
	ans.push_back(sequences[min.first]);
	ans.push_back(sequences[min.second]);
	int curmin = min.second;
	for (int i = 0; i < sequences.size() - 2; i++) {
		curmin = updateDistance(curmin, sequences.size());
		ans.push_back(sequences[curmin]);
		unselected.erase(curmin);
	}
	return ans;
}