/*
 * TreeNode.cpp
 *
 *  Created on: Jan 24, 2012
 *      Author: dragoon
 */

#include "TreeNode.h"

TreeNode::TreeNode(vector<vector<double>*> &rows) {
	greaterSqrSumDifference = 0;
	bestValue = 0;
	bestIndex = -1;

	double allSum = 0;
	double allSqrSum = 0;
	for (uint i = 0; i != rows.size(); i++) {
		allSum += rows[i]->at(0);
		allSqrSum += rows[i]->at(0) * rows[i]->at(0);
	}

	double avgValue = allSum / rows.size();
	double sumSqrDifference = avgValue * avgValue + allSqrSum - 2 * avgValue * allSum;

	int columnsCount = rows[0]->size();
	//find best dividing onto two parts (by greater sum squares of differences)
	for (int i = 1; i != columnsCount; i++) {
		sort(rows.begin(), rows.end(), RowsCompare(i));

		double firstSum = 0;
		double firstSqrSum = 0;
		for (uint j = 0; j + 1 != rows.size(); j++) {
			firstSum += rows[j]->at(0);
			firstSqrSum += rows[j]->at(0) * rows[j]->at(0);

			double stepAvgValueFirst = firstSum / (j + 1);
			double stepSumSqrDifferenceFirst = stepAvgValueFirst * stepAvgValueFirst + firstSqrSum - 2 * stepAvgValueFirst * firstSum;

			double stepAvgValueLast = (allSum - firstSum) / (rows.size() - j - 1);
			double stepSumSqrDifferenceLast = stepAvgValueLast * stepAvgValueLast + allSqrSum - firstSqrSum - 2 * stepAvgValueLast * (allSum - firstSum);

			double diff = sumSqrDifference - stepSumSqrDifferenceFirst - stepSumSqrDifferenceLast;
			if (diff > greaterSqrSumDifference) {
				greaterSqrSumDifference = diff;
				bestValue = (rows[j]->at(i) + rows[j + 1]->at(i)) / 2;
				bestIndex = i;
			}
		}

	}
}

TreeNode::~TreeNode() {
}
