/*
 * TreeNode.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "tree_node.h"

#include <algorithm>

using namespace std;

TreeNode::TreeNode(vector<vector<double>*> &rows) {
	best_split_value = 0;
	best_index = -1;

	double all_sum = 0;
	double all_sqr_sum = 0;
	for (uint i = 0; i != rows.size(); i++) {
		all_sum += rows[i]->at(0);
		all_sqr_sum += rows[i]->at(0) * rows[i]->at(0);
	}

	double avg_value = all_sum / rows.size();
	double sum_sqr_difference = avg_value * avg_value + all_sqr_sum - 2 * avg_value * all_sum;

	int columns_count = rows[0]->size();
	double min_leafes_sum_sqr_difference = sum_sqr_difference;
	//find best dividing onto two parts
	for (int i = 1; i != columns_count; i++) {
		sort(rows.begin(), rows.end(), RowsCompare(i));

		double first_sum = 0;
		double first_sqr_sum = 0;
		for (uint j = 0; j + 1 != rows.size(); j++) {
			first_sum += rows[j]->at(0);
			first_sqr_sum += rows[j]->at(0) * rows[j]->at(0);

			if (rows[j]->at(i) != rows[j + 1]->at(i)) { //if can split at this point
				double step_avg_value_first = first_sum / (j + 1);
				double step_sum_sqr_difference_first = step_avg_value_first * step_avg_value_first
						+ first_sqr_sum - 2 * step_avg_value_first * first_sum;

				double step_avg_value_last = (all_sum - first_sum) / (rows.size() - j - 1);
				double step_sum_sqr_difference_last = step_avg_value_last * step_avg_value_last
						+ all_sqr_sum - first_sqr_sum
						- 2 * step_avg_value_last * (all_sum - first_sum);

				double diff = step_sum_sqr_difference_first + step_sum_sqr_difference_last;
				if (diff < min_leafes_sum_sqr_difference) {
					min_leafes_sum_sqr_difference = diff;
					best_split_value = (rows[j]->at(i) + rows[j + 1]->at(i)) / 2;
					best_index = i;
				}
			}
		}
	}
	greater_sqr_sum_difference = sum_sqr_difference - min_leafes_sum_sqr_difference;
}

TreeNode::~TreeNode() {
}
