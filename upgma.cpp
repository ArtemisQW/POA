#include"upgma.hpp"
vector<double>* initdistance;               // 记录初始距离矩阵
unordered_map<int, double>* newdistance;    // 记录聚类到各个序列之间的距离
set<int> unselected;                        // 记录还未加入到树中的序列编号

vector<unordered_set<string>> seq_kmers;
void generateKmerSet(vector<string>& sequences, int k) {
	for (string seq : sequences) {
		unordered_set<string> kmers;
		for (int i = 0; i <= seq.length() - k; ++i) {
			kmers.insert(seq.substr(i, k));
		}
		seq_kmers.push_back(kmers);
	}
}

double calculateDistance(int i, int j, int k) {
	unordered_set<string> kmers1 = seq_kmers[i];
	unordered_set<string> kmers2 = seq_kmers[j];

	int intersection_size=0;
	unordered_set<string> union_set = kmers1;

	// 交集
	for (string kmer : kmers2) {
		if (kmers1.find(kmer) != kmers1.end()) {
			intersection_size++;
		}
	}

	// 并集
	union_set.insert(kmers2.begin(), kmers2.end());

	double similarity = (1.0 * intersection_size) /  (1.0 * union_set.size());
	double distance = 1.0 - similarity;

	return distance;
}

int llcs(const string& seq1, const string& seq2, int mode) {
	// 定义
	int m = seq1.length();
	int n = seq2.length();
	int** L = AllocateIntMtx(m + 1, n + 1);

	for (int i = 0; i <= m; i++) 
	{
		for (int j = 0; j <= n; j++)
		{
			if (i == 0 || j == 0)
			{
				L[i][j] = 0;
			}
			else if (seq1[i - 1] == seq2[j - 1]) 
			{
				L[i][j] = L[i - 1][j - 1] + 1;
			}
			else
			{
				L[i][j] = max(L[i - 1][j], L[i][j - 1]);
			}
		}
	}

	if (mode == 1) lcs(seq1, seq2, L);

	int ans = L[m][n];
	FreeIntMtx(L);

	return ans;
}

void lcs(const string& seq1, const string& seq2, int** L) {
	int i = seq1.length(), j = seq2.length();
	string lcs;

	while (i > 0 && j > 0) {
		if (seq1[i - 1] == seq2[j - 1]) {
			lcs = seq1[i - 1] + lcs;
			i--;
			j--;
		}
		else if (L[i - 1][j] > L[i][j - 1]) {
			i--;
		}
		else {
			j--;
		}
	}
	cout << lcs << endl;

}

int calculatePos(int i, int j, int n) {
	if (i > j) swap(i, j);
	return ((2 * n - i - 1) * i) / 2 + j - i - 1;
}
// 计算初始距离矩阵，初始化聚类到各个序列之间的距离，初始化unselected
pair<int, int> initDistance(vector<string>& sequences,int k, int distancemode) {
    if(distancemode == 0) generateKmerSet(sequences, k);
	int n = sequences.size();
	for (int i = 0; i < n; i++) unselected.insert(i);
	pair<int, int> p;
	initdistance = new vector<double>();
	double min = INFINITY;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			double d;
			if (distancemode == 0)  d = calculateDistance(i,j,k);
			else  d = -1 * llcs(sequences[i],sequences[j],0);

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

int updateDistance(int premin, int n,int mode) {
	
	unordered_map<int, double>* updatedistance = new unordered_map<int, double>();
	int min = -1;
	if (mode == 0) {
		for (int i : unselected) {
			int size = n - (*newdistance).size();
			(*updatedistance)[i] = ((*newdistance)[i] * size + (*initdistance)[calculatePos(i, premin, n)]) / (size + 1);
			if (min < 0 || (*updatedistance)[i] < (*updatedistance)[min]) min = i;
		}
	}
	else {
		
		for (int i : unselected) {
			(*updatedistance)[i] = fmin((*newdistance)[i], (*initdistance)[calculatePos(i, premin, n)]);
			if (min < 0 || (*updatedistance)[i] < (*updatedistance)[min]) min = i;
		}
	}	
	newdistance = updatedistance;
	return min;
}
 
// clustermode=0 UPGMA,clustermode=1 single linkage ; distancemode = 0 k-mer,distancemode = 1 lcs 
vector<string> sortFastaByClustering(vector<string>& sequences, int k, int clustermode,int distancemode) {
	vector<string> ans;

	// 初始化距离矩阵
	pair<int, int> min = initDistance(sequences,k, distancemode);
	ans.push_back(sequences[min.first]);
	ans.push_back(sequences[min.second]);
	int curmin = min.second;
	for (int i = 0; i < sequences.size() - 2; i++) {
		curmin = updateDistance(curmin, sequences.size(), clustermode);
		ans.push_back(sequences[curmin]);
		unselected.erase(curmin);
	}
	return ans;
}

//vector<string> sortFastaByUPGMA(vector<string>& sequences,int k) {
//	vector<string> ans;
//
//	// 初始化距离矩阵
//	pair<int, int> min = initDistance(sequences,k);
//	ans.push_back(sequences[min.first]);
//	ans.push_back(sequences[min.second]);
//	int curmin = min.second;
//	for (int i = 0; i < sequences.size() - 2; i++) {
//		curmin = updateDistance(curmin, sequences.size());
//		ans.push_back(sequences[curmin]);
//		unselected.erase(curmin);
//	}
//	return ans;
//}