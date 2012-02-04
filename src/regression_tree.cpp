/*
 * RegressionTree.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "regression_tree.h"
#include <assert.h>
#include <queue>
#include <limits>
#include <iostream>

using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

RegressionTree::RegressionTree(vector<vector<double>*> *rows, uint max_nodes_count,
		uint min_rows_per_leaf, bool skip_on_min_rows_achieve, double max_params_correlation,
		bool debug_output) {
	min_rows_per_leaf_ = min_rows_per_leaf;
	skip_on_min_rows_achieve_ = skip_on_min_rows_achieve;
	max_params_correlation_ = max_params_correlation;
	assert(max_nodes_count > 0);
	if ((max_nodes_count & 1) == 0) { //binary regression tree cann't have even number of nodes
		--max_nodes_count;
	}
	max_nodes_count_ = max_nodes_count;
	init(rows, debug_output);
}

RegressionTree::RegressionTree(vector<vector<double>*> *rows, uint max_nodes_count,
		bool skip_on_min_rows_achieve) :
		skip_on_min_rows_achieve_(skip_on_min_rows_achieve), min_rows_per_leaf_(
				rows->at(0)->size() * 4) {
	max_params_correlation_ = 1.0;
	assert(max_nodes_count > 0);
	if ((max_nodes_count & 1) == 0) { //binary regression tree cann't have even number of nodes
		--max_nodes_count;
	}
	max_nodes_count_ = max_nodes_count;
	init(rows);
}

RegressionTree::RegressionTree(vector<vector<double>*> *rows, bool skip_on_min_rows_achieve) :
		max_nodes_count_(std::numeric_limits<uint>::max()), skip_on_min_rows_achieve_(
				skip_on_min_rows_achieve), min_rows_per_leaf_(rows->at(0)->size() * 4) {
	max_params_correlation_ = 1.0;
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

void RegressionTree::init(vector<vector<double>*> *rows, bool debug_output) {
	rows_ = new vector<vector<double>*>(rows->begin(), rows->end());
	vector<vector<double>*> *learn_rows = new vector<vector<double>*>(rows_->begin(), rows_->end());

	root_node_ = new TreeNode(learn_rows, max_params_correlation_, debug_output);

	std::priority_queue<pair<double, TreeNode*> > queue;
	queue.push(make_pair(root_node_->sum_sqr_improvement(), root_node_));

	uint nodes_count = 1;
	while ((queue.size() > 0) && (nodes_count < max_nodes_count_)) {
		pair<double, TreeNode*> best_node_to_split = queue.top();
		queue.pop();
		if (best_node_to_split.first > 0) {
			if (best_node_to_split.second->min_split_count() >= min_rows_per_leaf_) {
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
				if (!skip_on_min_rows_achieve_) {
					break;
				}
			}
		} else {
			break;
		}
	}

}

void RegressionTree::generate_hme_model(fstream *fmodel) {
	size_t x_count = rows_->at(0)->size();
	fmodel->write((char*) &x_count, sizeof(x_count));
	root_node_->generate_hme_model(fmodel);
}
