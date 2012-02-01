/*
 * TreeNode.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "tree_node.h"

#include "matrix_utils.h"

#include <algorithm>
#include <assert.h>
#include <iostream>

using std::cout;
using std::endl;
using std::min;
using std::reverse;

using matrix_utils::operator *;
using matrix_utils::inversion;
using matrix_utils::transpose;
using matrix_utils::remove_linear_dependence_rows;

TreeNode::TreeNode(vector<vector<double>*> *rows) {
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

	//find best dividing onto two parts
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

	left_child_ = new TreeNode(left_leaf_rows);
	right_child_ = new TreeNode(right_leaf_rows);
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
		Matrix x_matrix;
		Matrix response_vector;
		x_matrix.reserve(rows_->size());
		response_vector.reserve(rows_->size());
		//divide onto y and x values
		for (uint i = 0; i != rows_->size(); i++) {
			vector<double>::iterator it = ++rows_->at(i)->begin();
			x_matrix.push_back(vector<double>(it, rows_->at(i)->end()));
			response_vector.push_back(vector<double>(rows_->at(i)->begin(), it));
		}

		Matrix transposed_x_matrix = transpose(x_matrix);
		//remove const parameters
		vector<uint> removed_const_parameters;
		for (uint i = 0; i != transposed_x_matrix.size(); i++) {
			bool ok = false;
			for (uint j = 1; j != transposed_x_matrix[0].size(); j++) {
				if (transposed_x_matrix[i][j] != transposed_x_matrix[i][0]) {
					ok = true;
					break;
				}
			}
			if (!ok) {
				transposed_x_matrix.erase(transposed_x_matrix.begin() + i);
				removed_const_parameters.push_back(i + 1); //shift depends on insert const columb
				--i;
			}
		}

		//inserting const column
		transposed_x_matrix.insert(transposed_x_matrix.begin(),
				vector<double>(transposed_x_matrix[0].size(), 1.0));

		vector<uint> removed_linear_dependenced_parameters = remove_linear_dependence_rows(
				transposed_x_matrix);
		// cann't remove first const column
		if (removed_linear_dependenced_parameters.size()) {
			assert(removed_linear_dependenced_parameters[0] > 0);
		}

		x_matrix = transpose(transposed_x_matrix);

		vector<double> weight_vector =
				transpose(
						inversion(transposed_x_matrix * x_matrix)
								* (transposed_x_matrix * response_vector))[0];

		//place zeroes in place of removed parameters
		reverse(removed_linear_dependenced_parameters.begin(),
				removed_linear_dependenced_parameters.end());
		for (uint i = 0; i != removed_linear_dependenced_parameters.size(); i++) {
			weight_vector.insert(weight_vector.begin() + removed_linear_dependenced_parameters[i],
					0);
		}
		reverse(removed_const_parameters.begin(), removed_const_parameters.end());
		for (uint i = 0; i != removed_const_parameters.size(); i++) {
			weight_vector.insert(weight_vector.begin() + removed_const_parameters[i], 0);
		}

		for (uint i = 0; i != weight_vector.size(); i++) {
			save_stream->write((char *) &weight_vector[i], sizeof(weight_vector[i]));
		}

#define matrix_out
#ifdef matrix_out
		freopen("addit1.txt", "a", stdout);
		cout.setf(std::ios_base::fixed);
		cout.precision(6);
		Matrix weight_vector_test;
		weight_vector_test.push_back(weight_vector);
		weight_vector_test = transpose(weight_vector_test);
		double sum = 0;

		for (uint i = 0; i != rows_->size(); i++) {
			Matrix xVC;
			xVC.push_back(vector<double>(rows_->at(i)->begin() + 1, rows_->at(i)->end()));
			xVC[0].insert(xVC[0].begin(), 1);
			Matrix rez = xVC * weight_vector_test;
			double tmp = rez[0][0] - rows_->at(i)->at(0);
			sum += tmp * tmp;
		}
		cout << sum_sqr_difference_ << ' ' << sum << ' ' << rows_->size() << ' '
				<< removed_linear_dependenced_parameters.size() << ' '
				<< removed_const_parameters.size() << endl;
		fclose(stdout);
		freopen("addit.txt", "a", stdout);
		cout.setf(std::ios_base::fixed);
		cout.precision(6);
		cout << x_matrix.size() << '*' << x_matrix[0].size() << endl;
		for (uint i = 0; i != x_matrix.size(); i++) {
			cout << response_vector[i][0];
			for (uint j = 0; j != x_matrix[i].size(); j++) {
				cout << ' ' << x_matrix[i][j];
			}
			cout << endl;
		}
		cout << endl;

		for (uint i = 0; i != weight_vector.size(); i++) {
			cout << weight_vector[i] << ' ';
		}
		cout << endl << endl;
		fclose(stdout);
		freopen("weight.txt", "a", stdout);
		for (uint i = 0; i != weight_vector.size(); i++) {
			cout << weight_vector[i] << ' ';
		}
		cout << endl;
		fclose(stdout);

#endif //#ifdef matrix_out
	} else {
		//init gate with two oppositely directed vectors
		double zero = 0;
		double tmp = -1;
		for (int i = 1; i != split_index_; i++) {
			save_stream->write((char *) &zero, sizeof(zero));
		}
		save_stream->write((char *) &tmp, sizeof(tmp));
		for (uint i = split_index_ + 1; i != rows_->at(0)->size(); i++) {
			save_stream->write((char *) &zero, sizeof(zero));
		}
		save_stream->write((char *) &split_value_, sizeof(split_value_));

		for (int i = 1; i != split_index_; i++) {
			save_stream->write((char *) &zero, sizeof(zero));
		}
		tmp = 1;
		save_stream->write((char *) &tmp, sizeof(tmp));
		for (uint i = split_index_ + 1; i != rows_->at(0)->size(); i++) {
			save_stream->write((char *) &zero, sizeof(zero));
		}
		tmp = -split_value_;
		save_stream->write((char *) &tmp, sizeof(tmp));

		left_child_->generate_hme_model(save_stream);
		right_child_->generate_hme_model(save_stream);
	}
}
