/*
 * RegressionTree.h
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#ifndef REGRESSIONTREE_H_
#define REGRESSIONTREE_H_
#include <vector>
#include "tree_node.h"

class RegressionTree {
public:
	RegressionTree(std::vector<std::vector<double>*> *rows);
	~RegressionTree();

private:
	void init(std::vector<std::vector<double>*> *rows);
	TreeNode *root_node_;
};

#endif /* REGRESSIONTREE_H_ */
