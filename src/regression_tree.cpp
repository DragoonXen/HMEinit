/*
 * RegressionTree.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "regression_tree.h"
#include <algorithm>
#include <assert.h>

RegressionTree::RegressionTree(std::vector<std::vector<double>*> *rows) {
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

}

RegressionTree::~RegressionTree() {

}

void RegressionTree::init(std::vector<std::vector<double>*> *rows) {

}
