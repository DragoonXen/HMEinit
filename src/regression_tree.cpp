/*
 * RegressionTree.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "regression_tree.h"
#include <assert.h>
#include <queue>
#include <limits.h>
#include <iostream>

using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

RegressionTree::RegressionTree(vector<vector<double>*> *rows, int max_nodes_count) {
	assert(max_nodes_count > 0);
	if ((max_nodes_count & 1) == 0) { //regression tree cann't have even number of nodes
		--max_nodes_count;
	}
	max_nodes_count_ = max_nodes_count;
	init(rows);
}

RegressionTree::RegressionTree(vector<vector<double>*> *rows) {
	max_nodes_count_ = INT_MAX;
	init(rows);
}

RegressionTree::~RegressionTree() {
	for (uint i = 0; i != rows_->size(); i++) {
		delete (rows_->at(i));
	}
	delete (rows_);
	delete (root_node_);
}

double RegressionTree::evaluate_mean_sqr_error(vector<vector<double>*> *rows) {
	double sum_sqr_error = 0;

	for (uint i = 0; i != rows->size(); i++) {
		double sqr_error = root_node_->evaluate_row(rows->at(i)) - rows->at(i)->at(0);
		sqr_error *= sqr_error;
		sum_sqr_error += sqr_error;
	}
	return sum_sqr_error / rows->size();
}

void RegressionTree::init(vector<vector<double>*> *rows) {
	rows_ = new vector<vector<double>*>(rows->begin(), rows->end());
	vector<vector<double>*> *learn_rows = new vector<vector<double>*>(rows_->begin(), rows_->end());

	root_node_ = new TreeNode(learn_rows);

	std::priority_queue<pair<double, TreeNode*> > queue;
	queue.push(make_pair(root_node_->sum_sqr_improvement(), root_node_));

	int nodes_count = 1;
	while ((queue.size() > 0) && (nodes_count < max_nodes_count_)) {
		pair<double, TreeNode*> best_node_to_split = queue.top();
		queue.pop();
		if ((best_node_to_split.first > 0)
				&& (best_node_to_split.second->min_split_count() >= rows->at(0)->size() * 4)) {
			cout << best_node_to_split.first << endl;
			nodes_count += 2;
			TreeNode *splitting_node = best_node_to_split.second;
			splitting_node->split_node();

			queue.push(
					make_pair(splitting_node->left_child()->sum_sqr_improvement(),
							splitting_node->left_child()));

			queue.push(
					make_pair(splitting_node->right_child()->sum_sqr_improvement(),
							splitting_node->right_child()));
		} else {
			break;
		}
	}

}

void RegressionTree::generate_hme_model(fstream *fmodel) {
	int x_count = rows_->at(0)->size() - 1;
	fmodel->write((char*) &x_count, sizeof(x_count));
	root_node_->generate_hme_model(fmodel);
}
