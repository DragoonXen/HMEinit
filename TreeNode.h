/*
 * TreeNode.h
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#ifndef TREENODE_H_
#define TREENODE_H_
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

class TreeNode {
public:
	TreeNode(vector<vector<double>*> &rows);
	virtual ~TreeNode();


private:
	struct RowsCompare : public binary_function<vector<double>*,vector<double>*,bool>{
		int idx;
		RowsCompare(int idx) {
			this->idx = idx;
		}
		inline bool operator()(const vector<double>* a, const vector<double>* b) {
			return a->at(idx) < b->at(idx);
		}
	};
	int bestIndex;
	double bestValue;
};

#endif /* TREENODE_H_ */
