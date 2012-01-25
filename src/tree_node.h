/*
 * TreeNode.h
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include <vector>
#include <functional>

class TreeNode {
public:
	TreeNode(std::vector<std::vector<double>*> &rows);
	virtual ~TreeNode();

private:
	struct RowsCompare: public std::binary_function<std::vector<double>*, std::vector<double>*, bool> {
		int idx;
		RowsCompare(int idx) {
			this->idx = idx;
		}
		inline bool operator()(const std::vector<double>* a, const std::vector<double>* b) {
			return a->at(idx) < b->at(idx);
		}
	};
	int best_index;
	double greater_sqr_sum_difference;
	double best_split_value;
};

#endif /* TREENODE_H_ */
