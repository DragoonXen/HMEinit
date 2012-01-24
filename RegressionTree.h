/*
 * RegressionTree.h
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#ifndef REGRESSIONTREE_H_
#define REGRESSIONTREE_H_
#include <vector>
#include "TreeNode.h"

using namespace std;

class RegressionTree {
public:
	RegressionTree(vector<vector<double>* > &rows);
	~RegressionTree();
private:
	TreeNode *rootNode;
};

#endif /* REGRESSIONTREE_H_ */
