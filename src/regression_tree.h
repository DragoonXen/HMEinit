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

	RegressionTree(vector<vector<double>*> *rows, int max_nodes_count);
	RegressionTree(vector<vector<double>*> *rows);
	~RegressionTree();
	double evaluate_mean_sqr_error(vector<vector<double>*> *rows);
	void generate_hme_model(fstream* fmodel);

private:
	void init(vector<vector<double>*> *rows);
	TreeNode *root_node_;
	int max_nodes_count_;
	vector<vector<double>*> *rows_;
};

#endif /* REGRESSIONTREE_H_ */
