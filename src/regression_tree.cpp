/*
 * RegressionTree.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "regression_tree.h"
#include <algorithm>
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

	std::random_shuffle(learn_rows->begin(), learn_rows->end());

	vector<vector<double>*> *test_rows = new vector<vector<double>*>();
	int test_rows_count = learn_rows->size() / 10;
	//must be at least 10 samples
	assert(test_rows_count);
	//move test_rows_count rows into test_rows
	for (int i = 0; i != test_rows_count; i++) {
		test_rows->push_back(*learn_rows->rbegin());
		learn_rows->pop_back();
	}
	root_node_ = new TreeNode(learn_rows);

	std::priority_queue<pair<double, TreeNode*> > queue;
	queue.push(make_pair(root_node_->sum_sqr_improvement(), root_node_));

	int nodes_count = 1;
	while (nodes_count < max_nodes_count_) {
		pair<double, TreeNode*> best_node_to_split = queue.top();
		queue.pop();
		if (best_node_to_split.first > 0) {
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

	vector<pair<double, TreeNode*> > tree_evaluation;

	//find mean_sqr_error evaluation of the best tree && it's leafs
	double best_subtree_test_mean_sqr_error = evaluate_mean_sqr_error(test_rows);
	vector<TreeNode*> best_tree_leafs = root_node_->get_leafs();

	cout << "evaluate_trees" << endl;
	cout << best_subtree_test_mean_sqr_error << endl;
	cout << best_tree_leafs.size() << endl;
	cout << "--------------" << endl;
	while ((tree_evaluation = root_node_->evaluate_cut_tree()).size() > 0) {
		sort(tree_evaluation.begin(), tree_evaluation.end());
		double cut_value = tree_evaluation.begin()->first;
		uint i = 0;
		cout << cut_value << endl;
		while (i != tree_evaluation.size() && tree_evaluation[i].first == cut_value) {
			tree_evaluation[i].second->is_leaf(true);
			i++;
		}

		double subtree_mean_sqr_error = evaluate_mean_sqr_error(test_rows);
		cout << subtree_mean_sqr_error << endl;
		if (best_subtree_test_mean_sqr_error > subtree_mean_sqr_error) {
			best_subtree_test_mean_sqr_error = subtree_mean_sqr_error;
			best_tree_leafs = root_node_->get_leafs();
			cout << best_tree_leafs.size() << endl;
		}
		cout << "--------------" << endl;
	}

	for (uint i = 0; i != best_tree_leafs.size(); i++) {
		best_tree_leafs[i]->cut_subtrees();
	}
	root_node_->leafs_re_mark();

	delete (test_rows);
}

void RegressionTree::generate_hme_model(fstream *fmodel) {
	int x_count = rows_->at(0)->size() - 1;
	fmodel->write((char*) &x_count, sizeof(x_count));
	root_node_->generate_hme_model(fmodel);
}
