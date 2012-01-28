/*
 * matrixutils.cpp
 *
 *  Created on: Jan 28, 2012
 *      Author: dragoon
 */

#include "matrix_utils.h"

#include <assert.h>
#include <iostream>
#include <cmath>

using std::abs;
using std::cout;
using std::endl;

inline bool matrix_utils::is_matrix(const vector<vector<double>*> &matrix) {
	if (matrix.size() == 0) {
		return false;

	}
	for (uint i = 1; i != matrix.size(); i++) {
		if (matrix[i]->size() != matrix[0]->size()) {
			return false;
		}
	}
	return true;
}

vector<vector<double>*> matrix_utils::create_matrix(uint rows, uint columns) {
	vector<vector<double>*> result_matrix(rows);
	for (uint i = 0; i != rows; i++) {
		result_matrix[i] = new vector<double>(columns);
	}
	return result_matrix;
}

vector<vector<double>*> matrix_utils::transpose(const vector<vector<double>*> &matrix) {
	assert(is_matrix(matrix));
	vector<vector<double>*> result_matrix = create_matrix(matrix[0]->size(), matrix.size());
	for (uint i = 0; i != matrix[0]->size(); i++) {
		for (uint j = 0; j != matrix.size(); j++) {
			result_matrix[i]->at(j) = matrix[j]->at(i);
		}
	}
	return result_matrix;
}

vector<vector<double>*> matrix_utils::operator *(const vector<vector<double>*> &first,
		const vector<vector<double>*> &second) {
	assert(is_matrix(first));
	assert(is_matrix(second));
	assert(first[0]->size() == second.size());
	uint n(first.size()), m(second[0]->size()), l(second.size());
	vector<vector<double>*> result_matrix = create_matrix(n, m);
	for (uint i = 0; i != n; i++) {
		for (uint j = 0; j != m; j++) {
			double result = 0;
			for (uint k = 0; k != l; k++) {
				result += first[i]->at(k) * second[k]->at(j);
			}
			result_matrix[i]->at(j) = result;
		}
	}
	return result_matrix;
}

vector<vector<double>*> matrix_utils::inversion(const vector<vector<double>*> &matrix) {
	assert(is_matrix(matrix));
	uint n(matrix.size());
	assert(n == matrix[0]->size());
	vector<vector<double>*> temp_matrix = create_matrix(n, n);
	vector<vector<double>*> result_matrix = create_matrix(n, n);
	for (uint i = 0; i != n; i++) {
		result_matrix[i]->at(i) = 1;
		for (uint j = 0; j != n; j++) {
			temp_matrix[i]->at(j) = matrix[i]->at(j);
		}
	}

	for (uint i = 0; i != n; i++) {
		uint index_max = 0;
		for (uint j = 1; j != n; j++) {
			if (abs(temp_matrix[j]->at(i)) > abs(temp_matrix[index_max]->at(i))) {
				index_max = j;
			}
		}
		if (index_max != i) {
			vector<double> *temp = temp_matrix[index_max];
			temp_matrix[index_max] = temp_matrix[i];
			temp_matrix[i] = temp;

			temp = result_matrix[index_max];
			result_matrix[index_max] = result_matrix[i];
			result_matrix[i] = temp;
		}
		if (abs(temp_matrix[index_max]->at(i)) < 1e-7) {
			cout << "det(A) == 0" << endl;
			assert(false);
		}
		double divizor = temp_matrix[i]->at(i);
		for (uint k = 0; k != n; k++) {
			temp_matrix[i]->at(k) /= divizor;
			result_matrix[i]->at(k) /= divizor;
		}
		for (uint j = 0; j != n; j++) {
			if (j != i) {
				double multiplier = -temp_matrix[j]->at(i);
				for (uint k = 0; k != n; k++) {
					temp_matrix[j]->at(k) += temp_matrix[i]->at(k) * multiplier;
					result_matrix[j]->at(k) += result_matrix[i]->at(k) * multiplier;
				}
			}
		}
	}
	return result_matrix;
}

vector<vector<double>*> matrix_utils::pseudo_inversion(const vector<vector<double>*> &matrix) {
	vector<vector<double>*> transposed_matrix = transpose(matrix);
	if (matrix.size() > transposed_matrix.size()) {
		return inversion(transposed_matrix * matrix) * transposed_matrix;
	} else {
		return transposed_matrix * inversion(matrix * transposed_matrix);
	}
}

