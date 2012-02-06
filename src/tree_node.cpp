/*
 * TreeNode.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "tree_node.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "perceptron/backpropagation_perceptron.h"

using std::cout;
using std::endl;
using std::min;
using std::reverse;
using std::copy;
using perceptron::BackpropagationPerceptron;

TreeNode::TreeNode(vector<vector<double>*> *rows, double max_params_correlation, bool debug_output) :
		max_params_correlation_(max_params_correlation), debug_output_(debug_output) {
	cout << "rows: " << rows->size() << endl;
	left_child_ = NULL;
	right_child_ = NULL;
	rows_ = rows;
	subtree_leafs_error_ = 0;
	subtree_leafs_ = 0;
	split_value_ = 0;
	split_index_ = -1;
	is_leaf_ = true;
	sum_sqr_improvement_ = 0;
	min_split_count_ = 0;
	init();
}

TreeNode::~TreeNode() {
	if (left_child_ != NULL) {
		delete (left_child_);
	}
	if (right_child_ != NULL) {
		delete (right_child_);
	}
	if (rows_ != NULL) {
		delete (rows_);
	}
}

TreeNode* TreeNode::left_child() {
	return left_child_;
}

TreeNode* TreeNode::right_child() {
	return right_child_;
}

double TreeNode::sum_sqr_difference() {
	return sum_sqr_difference_;
}

double TreeNode::sum_sqr_improvement() {
	return sum_sqr_improvement_;
}
double TreeNode::subtree_leafs_error() {
	return subtree_leafs_error_;
}
uint TreeNode::min_split_count() {
	return min_split_count_;
}
void TreeNode::is_leaf(bool leaf) {
	is_leaf_ = leaf;
}

void TreeNode::init() {
	double all_sum = 0;
	double all_sum_sqrs = 0;
	for (uint i = 0; i != rows_->size(); i++) {
		all_sum += rows_->at(i)->at(0);
		all_sum_sqrs += rows_->at(i)->at(0) * rows_->at(i)->at(0);
	}

	avg_value_ = all_sum / rows_->size();
	sum_sqr_difference_ = avg_value_ * avg_value_ * rows_->size() + all_sum_sqrs
			- 2 * avg_value_ * all_sum;
	// no reason to search slit node if all learn samples have same value
	if (sum_sqr_difference_ == 0) {
		return;
	}

	uint columns_count = rows_->at(0)->size();
	double min_leaves_sum_sqr_difference = sum_sqr_difference_;

	//find best dividing onto two parts, skip y
	for (uint i = 1; i != columns_count; i++) {
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
				if (diff < min_leaves_sum_sqr_difference) {
					min_leaves_sum_sqr_difference = diff;
					split_value_ = (rows_->at(j)->at(i) + rows_->at(j + 1)->at(i)) / 2;
					split_index_ = i;
					min_split_count_ = min((uint) (j + 1), (uint) (rows_->size() - j - 1));
				}
			}
		}
	}
	sum_sqr_improvement_ = sum_sqr_difference_ - min_leaves_sum_sqr_difference;
}

void TreeNode::split_node() {
	vector<vector<double>*> *left_leaf_rows = new vector<vector<double>*>();
	vector<vector<double>*> *right_leaf_rows = new vector<vector<double>*>();

	for (uint i = 0; i != rows_->size(); i++) {
		if (rows_->at(i)->at(split_index_) <= split_value_) {
			left_leaf_rows->push_back(rows_->at(i));
		} else {
			right_leaf_rows->push_back(rows_->at(i));
		}
	}

	is_leaf_ = false;

	left_child_ = new TreeNode(left_leaf_rows, max_params_correlation_, debug_output_);
	right_child_ = new TreeNode(right_leaf_rows, max_params_correlation_, debug_output_);
}

vector<pair<double, TreeNode*> > TreeNode::evaluate_cut_tree() {
	vector<pair<double, TreeNode*> > nodes_vector;
	if (is_leaf_) { //skip y value
		subtree_leafs_ = 1;
		subtree_leafs_error_ = sum_sqr_difference_;
		return nodes_vector;
	}

	nodes_vector = left_child_->evaluate_cut_tree();

	vector<pair<double, TreeNode*> > right_child_vector;
	right_child_vector = right_child_->evaluate_cut_tree();

	nodes_vector.insert(nodes_vector.end(), right_child_vector.begin(), right_child_vector.end());

	subtree_leafs_ = left_child_->subtree_leafs_ + right_child_->subtree_leafs_;
	subtree_leafs_error_ = left_child_->subtree_leafs_error_ + right_child_->subtree_leafs_error_;

	nodes_vector.push_back(
			std::make_pair((sum_sqr_difference_ - subtree_leafs_error_) / (subtree_leafs_ - 1),
					this));
	return nodes_vector;
}

vector<TreeNode*> TreeNode::get_leafs() {
	vector<TreeNode*> leafs_list;
	if (is_leaf_) {
		leafs_list.push_back(this);
		return leafs_list;
	}
	leafs_list = left_child_->get_leafs();

	vector<TreeNode*> right_child_leafs_list = right_child_->get_leafs();

	leafs_list.insert(leafs_list.end(), right_child_leafs_list.begin(),
			right_child_leafs_list.end());

	return leafs_list;
}

double TreeNode::evaluate_row(vector<double>* row) {
	if (is_leaf_) {
		return avg_value_;
	}
	if (row->at(split_index_) <= split_value_) {
		return left_child_->evaluate_row(row);
	}
	return right_child_->evaluate_row(row);
}

void TreeNode::cut_subtrees() {
	if (left_child_ != NULL) {
		delete (left_child_);
		left_child_ = NULL;
	}
	if (right_child_ != NULL) {
		delete (right_child_);
		right_child_ = NULL;
	}
}

void TreeNode::leafs_re_mark() {
	if (left_child_ == NULL && right_child_ == NULL) {
		is_leaf_ = true;
	} else {
		is_leaf_ = false;
		if (left_child_ != NULL)
			left_child_->leafs_re_mark();
		if (right_child_ != NULL)
			right_child_->leafs_re_mark();
	}
}

void TreeNode::generate_hme_model(fstream* save_stream) {
	save_stream->write((char *) &is_leaf_, sizeof(is_leaf_));
	if (is_leaf_) {
		cout << "learn expert" << endl;
		double** params_matrix = new double*[rows_->size()];
		double** vector_d = new double*[rows_->size()];
		for (size_t i = 0; i < rows_->size(); i++) {
			vector_d[i] = new double[1];
			vector_d[i][0] = rows_->at(i)->at(0);
			params_matrix[i] = new double[rows_->at(0)->size()];
			copy(++rows_->at(i)->begin(), rows_->at(i)->end(), params_matrix[i]);
		}

		vector<size_t> layers;
		layers.push_back(235);
		layers.push_back(10);
		layers.push_back(1);
		BackpropagationPerceptron perceptron(layers, 0.5, 0);
		perceptron.perceptron_learning(params_matrix, vector_d, rows_->size(), 10, 50, 0.97);

		cout << perceptron.evaluate_perceptron(params_matrix, vector_d, rows_->size()) << endl;
		perceptron.save(*save_stream);

		for (size_t i = 0; i < rows_->size(); i++) {
			delete[] params_matrix[i];
			delete[] vector_d[i];
		}
		delete[] params_matrix;
		delete[] vector_d;

	} else {
		cout << "learn split node" << endl;
		double** params_matrix = new double*[rows_->size()];
		double** vector_d = new double*[rows_->size()];
		for (size_t i = 0; i < rows_->size(); i++) {
			vector_d[i] = new double[1];
			if (split_value_ > rows_->at(i)->at(split_index_)) {
				vector_d[i][0] = 0.5
						+ (split_value_ - rows_->at(i)->at(split_index_)) / (split_value_ * 4);
			} else {
				vector_d[i][0] = 0.5
						- (rows_->at(i)->at(split_index_) - split_value_)
								/ ((1.0 - split_value_) * 4);
			}
			params_matrix[i] = new double[rows_->at(0)->size()];
			copy(++rows_->at(i)->begin(), rows_->at(i)->end(), params_matrix[i]);
		}

		vector<size_t> layers;
		layers.push_back(235);
		layers.push_back(10);
		layers.push_back(1);
		BackpropagationPerceptron perceptron(layers, 0.5, 0);
		perceptron.perceptron_learning(params_matrix, vector_d, rows_->size(), 10, 10, 1.0);

		cout << perceptron.evaluate_perceptron(params_matrix, vector_d, rows_->size()) << endl;
		perceptron.save(*save_stream);

		for (size_t i = 0; i < rows_->size(); i++) {
			delete[] params_matrix[i];
			delete[] vector_d[i];
		}
		delete[] params_matrix;
		delete[] vector_d;

		left_child_->generate_hme_model(save_stream);
		right_child_->generate_hme_model(save_stream);
	}
}
