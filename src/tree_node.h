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
	TreeNode(std::vector<std::vector<double>*> *rows);
	virtual ~TreeNode();

	TreeNode* left_child();
	TreeNode* right_child();
	double sum_sqr_difference();
	double sum_sqr_improvement();

	void split_node();

private:
	void init();

	struct RowsCompare: public std::binary_function<std::vector<double>*, std::vector<double>*, bool> {
		int idx;
		RowsCompare(int idx) {
			this->idx = idx;
		}
		inline bool operator()(const std::vector<double>* a, const std::vector<double>* b) {
			return a->at(idx) < b->at(idx);
		}
	};

	std::vector<std::vector<double>*> *rows_;

	TreeNode *left_child_;
	TreeNode *right_child_;

	double avg_value_;
	double sum_sqr_difference_;
	double sum_sqr_improvement_;

	int split_index_;
	double split_value_;
};

#endif /* TREENODE_H_ */
