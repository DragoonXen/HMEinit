#include <fstream>
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "regression_tree.h"
#include "tree_node.h"
#include <iostream>

#include "matrix_utils.h"
using std::vector;
using std::cout;
using std::endl;
using matrix_utils::operator *;
int main(int argc, char* argv[]) {
	srand(time(NULL));

	char line[5000];
	std::ifstream f(argv[argc - 1]);
	vector<vector<double>*> *input_matrix = new vector<vector<double>*>();
	while (f.getline(line, 5000)) {
		std::stringstream sstr;
		sstr << line;
		vector<double> *row = new vector<double>();
		double tmp;
		while (sstr >> tmp) {
			row->push_back(tmp);
		}
		if (input_matrix->size()) {
			assert(input_matrix->at(0)->size() == row->size());
		}
		input_matrix->push_back(row);
	}
	f.close();

	RegressionTree tree(input_matrix, 150);
	std::fstream fmodel("base_model.bin", std::ios_base::binary | std::ios_base::out);
	tree.generate_hme_model(&fmodel);
	fmodel.close();
	return 0;
}
