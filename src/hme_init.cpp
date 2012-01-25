#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include "regression_tree.h"

using namespace std;
char line[5000];

int main(int argc, char* argv[]) {
	ifstream f(argv[argc - 1]);
	vector<vector<double>*> input_matrix;
	while (f.getline(line, 5000)) {
		stringstream sstr;
		sstr << line;
		vector<double> *row = new vector<double>();
		double tmp;
		while (sstr >> tmp) {
			row->push_back(tmp);
		}
		if (input_matrix.size()) {
			assert(input_matrix[0]->size() == row->size());
		}
		input_matrix.push_back(row);
	}
	f.close();

	RegressionTree tree(input_matrix);

	return 0;
}
