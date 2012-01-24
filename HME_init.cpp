#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include "RegressionTree.h"

using namespace std;
char line[5000];

int main(int argc, char* argv[]) {
	ifstream f(argv[argc - 1]);
	vector<vector<double>*> inputMatrix;
	while (f.getline(line, 5000)) {
		stringstream sstr;
		sstr << line;
		vector<double> *row = new vector<double>();
		double tmp;
		while (sstr >> tmp) {
			row->push_back(tmp);
		}
		if (inputMatrix.size()) {
			assert(inputMatrix[0]->size() == row->size());
		}
		inputMatrix.push_back(row);
	}
	f.close();

	RegressionTree tree(inputMatrix);

	return 0;
}
