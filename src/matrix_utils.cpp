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

inline bool matrix_utils::is_matrix(const Matrix &matrix) {
	if (matrix.size() == 0) {
		return false;
	}
	for (uint i = 1; i != matrix.size(); i++) {
		if (matrix[i].size() != matrix[0].size()) {
			return false;
		}
	}
	return true;
}

Matrix matrix_utils::create_matrix(uint rows, uint columns) {
	Matrix result_matrix;
	result_matrix.reserve(rows);
	for (uint i = 0; i != rows; i++) {
		vector<double> tmp(columns);
		result_matrix.push_back(tmp);
	}
	return result_matrix;
}

Matrix matrix_utils::transpose(const Matrix &matrix) {
	assert(is_matrix(matrix));
	Matrix result_matrix = create_matrix(matrix[0].size(), matrix.size());
	for (uint i = 0; i != matrix[0].size(); i++) {
		for (uint j = 0; j != matrix.size(); j++) {
			result_matrix[i].at(j) = matrix[j].at(i);
		}
	}
	return result_matrix;
}

Matrix matrix_utils::operator *(const Matrix &first, const Matrix &second) {
	assert(is_matrix(first));
	assert(is_matrix(second));
	assert(first[0].size() == second.size());
	uint n(first.size()), m(second[0].size()), l(second.size());
	Matrix result_matrix = create_matrix(n, m);
	for (uint i = 0; i != n; i++) {
		for (uint j = 0; j != m; j++) {
			double result = 0;
			for (uint k = 0; k != l; k++) {
				result += first[i][k] * second[k][j];
			}
			result_matrix[i][j] = result;
		}
	}
	return result_matrix;
}

Matrix matrix_utils::inversion(const Matrix &matrix) {
	assert(is_matrix(matrix));
	uint n(matrix.size());
	assert(n == matrix[0].size());
	Matrix temp_matrix = create_matrix(n, n);
	Matrix result_matrix = create_matrix(n, n);
	for (uint i = 0; i != n; i++) {
		result_matrix[i][i] = 1;
		for (uint j = 0; j != n; j++) {
			temp_matrix[i][j] = matrix[i][j];
		}
	}
	for (uint i = 0; i != n; i++) {
		uint index_max = i;
		for (uint j = i + 1; j != n; j++) {
			if (abs(temp_matrix[j][i]) > abs(temp_matrix[index_max][i])) {
				index_max = j;
			}
		}
		if (index_max != i) {
			swap(temp_matrix[index_max], temp_matrix[i]);
			swap(result_matrix[index_max], result_matrix[i]);
		}
		if (abs(temp_matrix[index_max][i]) < 1e-9) {
			cout << "det (matrix) == 0" << endl;
			assert(false);
		}
		double divizor = temp_matrix[i][i];
		for (uint k = 0; k != n; k++) {
			temp_matrix[i][k] /= divizor;
			result_matrix[i][k] /= divizor;
		}
		for (uint j = 0; j != n; j++) {
			if (j != i) {
				double multiplier = -temp_matrix[j][i];
				for (uint k = 0; k != n; k++) {
					temp_matrix[j][k] += temp_matrix[i][k] * multiplier;
					result_matrix[j][k] += result_matrix[i][k] * multiplier;
				}
			}
		}
	}
	return result_matrix;
}

vector<uint> matrix_utils::remove_linear_dependence_rows(Matrix &matrix) {
	vector<uint> removed_rows;
	//need to prevent swap whole columns, just swap indexes
	vector<uint> columns(matrix[0].size());
	for (uint i = 0; i != columns.size(); i++) {
		columns[i] = i;
	}

	Matrix temp_matrix;
	temp_matrix.reserve(matrix.size());
	for (uint i = 0; i != matrix.size(); i++) {
		temp_matrix.push_back(vector<double>(matrix[i].begin(), matrix[i].end()));
	}
	for (uint i = 0; i != temp_matrix.size(); i++) {
		uint index_max = i;
		for (uint j = i + 1; j != temp_matrix[0].size(); j++) {
			if (abs(temp_matrix[i][columns[index_max]]) < abs(temp_matrix[i][columns[j]])) {
				index_max = j;
			}
		}
		if (index_max != i) {
			uint tmp = columns[i];
			columns[i] = columns[index_max];
			columns[index_max] = tmp;
		}
		index_max = columns[i];
		if (abs(temp_matrix[i][index_max]) < 1e-7) {
			temp_matrix.erase(temp_matrix.begin() + i);
			matrix.erase(matrix.begin() + i);
			removed_rows.push_back(i);
			i--;
			continue;
		}

		double divisor = temp_matrix[i][index_max];
		for (uint k = 0; k != temp_matrix[0].size(); k++) {
			temp_matrix[i][k] /= divisor;
		}
		for (uint j = i + 1; j != temp_matrix.size(); j++) {
			double multiplier = -temp_matrix[j][index_max];
			for (uint k = 0; k != temp_matrix[0].size(); k++) {
				temp_matrix[j][k] += temp_matrix[i][k] * multiplier;
			}
		}
	}

	return removed_rows;
}
