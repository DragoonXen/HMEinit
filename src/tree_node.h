/*
 * TreeNode.h
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include <vector>
#include <fstream>
#include <functional>

typedef unsigned int uint;

using std::vector;
using std::pair;
using std::fstream;

class TreeNode {
public:
	TreeNode(vector<vector<double>*> *rows, double max_params_korrelation_ = 1.0,
			bool debug_output_ = false);
	virtual ~TreeNode();

	TreeNode* left_child();
	TreeNode* right_child();
	double sum_sqr_difference();
	double sum_sqr_improvement();
	double subtree_leafs_error();
	int subtree_leafs();
	uint min_split_count();
	void is_leaf(bool leaf);

	void split_node();
	vector<pair<double, TreeNode*> > evaluate_cut_tree();
	vector<TreeNode*> get_leafs();
	double evaluate_row(vector<double>* row);
	void cut_subtrees();
	void leafs_re_mark();
	void generate_hme_model(fstream* save_stream);

private:
	void init();

	struct RowsCompare: public std::binary_function<vector<double>*, vector<double>*, bool> {
		int idx;
		RowsCompare(int idx) {
			this->idx = idx;
		}
		inline bool operator()(const vector<double>* a, const vector<double>* b) {
			return a->at(idx) < b->at(idx);
		}
	};

	vector<vector<double>*> *rows_;

	TreeNode *left_child_;
	TreeNode *right_child_;

	double avg_value_;
	double sum_sqr_difference_;
	double sum_sqr_improvement_;
	double subtree_leafs_error_;
	double max_params_correlation_;
	bool debug_output_;
	int subtree_leafs_;
	bool is_leaf_;
	uint min_split_count_;

	int split_index_;
	double split_value_;

};

#endif /* TREENODE_H_ */
