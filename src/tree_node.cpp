/*
 * TreeNode.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "tree_node.h"

#include <algorithm>

TreeNode::TreeNode(std::vector<std::vector<double>*> *rows) {
	tree_nodes_count++;
	left_child_ = NULL;
	right_child_ = NULL;
	rows_ = rows;
	init();
}

TreeNode::~TreeNode() {
	left_child_->~TreeNode();
	right_child_->~TreeNode();
	if (rows_ != NULL) {
		for (uint i = 0; i != rows_->size(); i++) {
			delete (rows_->at(i));
		}
		rows_->clear();
		delete (rows_);
	}
}

double TreeNode::sum_sqr_difference() {
	return sum_sqr_difference_;
}

TreeNode* TreeNode::left_child() {
	return left_child_;
}

TreeNode* TreeNode::right_child() {
	return right_child_;
}

void TreeNode::init() {
	split_value_ = 0;
	split_index_ = -1;

	double all_sum = 0;
	double all_sum_sqrs = 0;
	for (uint i = 0; i != rows_->size(); i++) {
		all_sum += rows_->at(i)->at(0);
		all_sum_sqrs += rows_->at(i)->at(0) * rows_->at(i)->at(0);
	}

	double avg_value = all_sum / rows_->size();
	sum_sqr_difference_ = avg_value * avg_value * rows_->size() + all_sum_sqrs
			- 2 * avg_value * all_sum;
	int columns_count = rows_->at(0)->size();
	double min_leafes_sum_sqr_difference = sum_sqr_difference_;
	// no reason to slit node if all learn samples have same value
	if (sum_sqr_difference_ == 0)
		return;

	//find best dividing onto two parts
	for (int i = 1; i != columns_count; i++) {
		sort(rows_->begin(), rows_->end(), RowsCompare(i));

		double first_sum = 0;
		double first_sum_sqrs = 0;
		for (uint j = 0; j + 1 != rows_->size(); j++) {
			first_sum += rows_->at(j)->at(0);
			first_sum_sqrs += rows_->at(j)->at(0) * rows_->at(j)->at(0);

			if (rows_->at(j)->at(i) != rows_->at(j + 1)->at(i)) { //if can split at this point
				double step_avg_value_first = first_sum / (j + 1);
				double step_sum_sqr_difference_first = step_avg_value_first * step_avg_value_first
						* (j + 1) + first_sum_sqrs - 2 * step_avg_value_first * first_sum;

				double step_avg_value_last = (all_sum - first_sum) / (rows_->size() - j - 1);
				double step_sum_sqr_difference_last = step_avg_value_last * step_avg_value_last
						* (rows_->size() - j - 1) + all_sum_sqrs - first_sum_sqrs
						- 2 * step_avg_value_last * (all_sum - first_sum);

				double diff = step_sum_sqr_difference_first + step_sum_sqr_difference_last;
				if (diff < min_leafes_sum_sqr_difference) {
					min_leafes_sum_sqr_difference = diff;
					split_value_ = (rows_->at(j)->at(i) + rows_->at(j + 1)->at(i)) / 2;
					split_index_ = i;
				}
			}
		}
	}
	if (sum_sqr_difference_ - min_leafes_sum_sqr_difference > 1) { //if find any point to split
		split_node();
	}
}

void TreeNode::split_node() {
	std::vector<std::vector<double>*> *left_leaf_rows = new std::vector<std::vector<double>*>();
	std::vector<std::vector<double>*> *right_leaf_rows = new std::vector<std::vector<double>*>();

	sort(rows_->begin(), rows_->end(), RowsCompare(split_index_));

	uint idx = 0;
	while (rows_->at(idx)->at(split_index_) <= split_value_) {
		left_leaf_rows->push_back(rows_->at(idx));
		idx++;
	}
	while (idx != rows_->size()) {
		right_leaf_rows->push_back(rows_->at(idx));
		idx++;
	}

	rows_->clear();
	delete (rows_);
	rows_ = NULL;

	left_child_ = new TreeNode(left_leaf_rows);
	right_child_ = new TreeNode(right_leaf_rows);
}
