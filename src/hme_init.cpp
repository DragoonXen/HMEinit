#include "regression_tree.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <vector>

using std::vector;
using std::cout;
using std::endl;
using std::string;

int main(int argc, char* argv[]) {
	cout << "usage: " << endl << "-d : enable debug output" << endl
			<< "-s : enable skip on minimum leaf rows achivement" << endl
			<< "-m [double] : set minimum  leaf rows depends on parameters (multiplier)" << endl
			<< "-data [string] : data filename" << endl << "-model [string] : model filename"
			<< endl << "-n [unsigned integer] : max nodes in tree" << endl;
	string data_file = "data.learn";
	string model_file = "base_model.bin";
	uint max_nodes = 150;
	double min_rows_depends_on_params_count_multiplyer = 4;
	bool skip_on_min_rows_achieve = false;
	bool debug_output = false;
	for (int i = 0; i != argc; i++) {
		if (strcmp("-d", argv[i]) == 0) {
			debug_output = true;
		} else if (strcmp("-s", argv[i]) == 0) {
			skip_on_min_rows_achieve = true;
		} else if (strcmp("-m", argv[i]) == 0) {
			++i;
			assert(i < argc);
			std::stringstream sstr;
			sstr << argv[i];
			sstr >> min_rows_depends_on_params_count_multiplyer;
		} else if (strcmp("-data", argv[i]) == 0) {
			++i;
			assert(i < argc);
			data_file = argv[i];
		} else if (strcmp("-model", argv[i]) == 0) {
			++i;
			assert(i < argc);
			model_file = argv[i];
		} else if (strcmp("-n", argv[i]) == 0) {
			++i;
			assert(i < argc);
			std::stringstream sstr;
			sstr << argv[i];
			sstr >> max_nodes;
		}
	}

	char line[5000];
	std::ifstream f(data_file.c_str());
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

	RegressionTree tree(input_matrix, max_nodes,
			min_rows_depends_on_params_count_multiplyer * input_matrix->at(0)->size(),
			skip_on_min_rows_achieve, debug_output);
	std::fstream fmodel(model_file.c_str(), std::ios_base::binary | std::ios_base::out);
	tree.generate_hme_model(&fmodel);
	fmodel.close();
	return 0;
}
