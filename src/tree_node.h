/*
 * TreeNode.h
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include <vector>
#include <list>
#include <functional>

class TreeNode {
public:
	TreeNode(std::vector<std::vector<double>*> *rows);
	virtual ~TreeNode();

	TreeNode* left_child();
	TreeNode* right_child();
	double sum_sqr_difference();
	double sum_sqr_improvement();
	double subtree_leafs_error();
	int subtree_leafs();
	void is_leaf(bool leaf);

	void split_node();
	std::vector<std::pair<double, TreeNode*> > evaluate_cut_tree();
	std::vector<TreeNode* > get_leafs();
	double evaluate_row(std::vector<double>* row);
	void cut_subtrees();
	void leafs_re_mark();

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
	double subtree_leafs_error_;
	int subtree_leafs_;
	bool is_leaf_;

	int split_index_;
	double split_value_;
};

#endif /* TREENODE_H_ */
