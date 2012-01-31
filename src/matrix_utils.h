/*
 * matrixutils.h
 *
 *  Created on: Jan 28, 2012
 *      Author: dragoon
 */

#ifndef MATRIXUTILS_H_
#define MATRIXUTILS_H_

#include <vector>

using std::vector;

typedef unsigned int uint;
typedef vector<vector<double> > Matrix;

namespace matrix_utils {

inline bool is_matrix(const Matrix &matrix);
Matrix create_matrix(uint n, uint m);
Matrix transpose(const Matrix &matrix);
Matrix operator *(const Matrix &first,
		const Matrix &second);
Matrix inversion(const Matrix &matrix);
vector<uint> remove_linear_dependence_rows(Matrix &matrix);

} //namespace matrix_utils

#endif /* MATRIXUTILS_H_ */
