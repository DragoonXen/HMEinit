/*
 * RegressionTree.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "RegressionTree.h"

RegressionTree::RegressionTree(vector<vector<double>* > &rows) {
	rootNode = new TreeNode(rows);
}

RegressionTree::~RegressionTree() {

}
