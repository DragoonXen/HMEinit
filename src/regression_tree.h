/*
 * RegressionTree.h
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#ifndef REGRESSIONTREE_H_
#define REGRESSIONTREE_H_
#include "tree_node.h"

#include <vector>
#include <fstream>

using std::vector;
using std::fstream;

class RegressionTree {

public:
	RegressionTree(vector<vector<double>*> *rows, uint max_nodes_count, uint min_rows_per_leaf,
			bool skip_on_min_rows_achieve = false, bool debug_output = true);
	RegressionTree(vector<vector<double>*> *rows, uint max_nodes_count,
			bool skip_on_min_rows_achieve = false);
	RegressionTree(vector<vector<double>*> *rows, bool skip_on_min_rows_achieve = false);
	~RegressionTree();
	double evaluate_mean_sqr_error(vector<vector<double>*> *rows);
	void generate_hme_model(fstream* fmodel);

private:
	void init(vector<vector<double>*> *rows, bool debug_output = false);
	TreeNode *root_node_;
	uint max_nodes_count_;
	vector<vector<double>*> *rows_;
	bool skip_on_min_rows_achieve_;
	uint min_rows_per_leaf_;
};

#endif /* REGRESSIONTREE_H_ */
