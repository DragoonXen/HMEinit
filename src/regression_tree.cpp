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

RegressionTree::RegressionTree(std::vector<std::vector<double>*> *rows, int max_nodes_count) {
	assert(max_nodes_count > 0);
	if ((max_nodes_count & 1) == 0) { //regression tree cann't have even number of nodes
		--max_nodes_count;
	}
	max_nodes_count_ = max_nodes_count;
	init(rows);
}

RegressionTree::RegressionTree(std::vector<std::vector<double>*> *rows) {
	max_nodes_count_ = INT_MAX;
	init(rows);
}

RegressionTree::~RegressionTree() {
	delete (root_node_);
}

void RegressionTree::init(std::vector<std::vector<double>*> *rows) {
	std::vector<std::vector<double>*> *learn_rows = new std::vector<std::vector<double>*>();
	for (uint i = 0; i != rows->size(); i++) {
		learn_rows->push_back(new std::vector<double>(*rows->at(i)));
	}

	std::random_shuffle(learn_rows->begin(), learn_rows->end());

	std::vector<std::vector<double>*> *test_rows = new std::vector<std::vector<double>*>();
	int test_rows_count = learn_rows->size() / 10;
	//must be at least 10 samples
	assert(test_rows_count);
	//move test_rows_count rows into test_rows
	for (int i = 0; i != test_rows_count; i++) {
		test_rows->push_back(*learn_rows->rbegin());
		learn_rows->pop_back();
	}
	root_node_ = new TreeNode(learn_rows);

	std::priority_queue<std::pair<double, TreeNode*> > queue;
	queue.push(std::make_pair(root_node_->sum_sqr_improvement(), root_node_));

	int nodes_count = 1;
	while (nodes_count < max_nodes_count_) {
		std::pair<double, TreeNode*> best_node_to_split = queue.top();
		queue.pop();
		if (best_node_to_split.first > 1) {
			std::cout << best_node_to_split.first << std::endl;
			nodes_count += 2;
			TreeNode *splitting_node = best_node_to_split.second;
			splitting_node->split_node();

			queue.push(
					std::make_pair(splitting_node->left_child()->sum_sqr_improvement(),
							splitting_node->left_child()));

			queue.push(
					std::make_pair(splitting_node->right_child()->sum_sqr_improvement(),
							splitting_node->right_child()));
		}
	}
	// TODO: here must be implement best tree size choosing

	for (uint i = 0; i != test_rows->size(); i++) {
		delete (test_rows->at(0));
	}

	delete (test_rows);
	delete (learn_rows);
}
