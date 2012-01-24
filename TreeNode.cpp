/*
 * TreeNode.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "TreeNode.h"

TreeNode::TreeNode(vector<vector<double>*> &rows) {
	bestValue = -1;
	int columnsCount = rows[0]->size();
	for (int i = 1; i != columnsCount; i++) {
		sort(rows.begin(), rows.end(), RowsCompare(i));
	}
}

TreeNode::~TreeNode() {
}
