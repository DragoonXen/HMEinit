/*
 * RegressionTree.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "regression_tree.h"

RegressionTree::RegressionTree(vector<vector<double>* > &rows) {
	root_node = new TreeNode(rows);
}

RegressionTree::~RegressionTree() {

}
