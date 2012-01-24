/*
 * TreeNode.h
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#ifndef TREENODE_H_
#define TREENODE_H_
#include <vector>

using namespace std;

class TreeNode {
public:
	TreeNode(vector<vector<double>* > &rows);
	virtual ~TreeNode();
};

#endif /* TREENODE_H_ */
