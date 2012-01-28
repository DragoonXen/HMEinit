#include <fstream>
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "regression_tree.h"
#include "tree_node.h"

int main(int argc, char* argv[]) {
	char line[5000];
	std::ifstream f(argv[argc - 1]);
	std::vector<std::vector<double>*> *input_matrix = new std::vector<std::vector<double>*>();
	while (f.getline(line, 5000)) {
		std::stringstream sstr;
		sstr << line;
		std::vector<double> *row = new std::vector<double>();
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
	srand(time(NULL));

	RegressionTree tree(input_matrix, 150);
	std::fstream fmodel("base_model.bin", std::ios_base::binary | std::ios_base::out);
	tree.generate_hme_model(&fmodel);
	fmodel.close();
	return 0;
}
