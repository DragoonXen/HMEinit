/*
 * matrixutils.h
 *
 *  Created on: Jan 28, 2012
 *      Author: dragoon
 */

#ifndef MATRIXUTILS_H_
#define MATRIXUTILS_H_

#include <vector>

typedef unsigned int uint;

using std::vector;

namespace matrix_utils {

inline bool is_matrix(const vector<vector<double>*> &matrix);
vector<vector<double>*> create_matrix(uint n, uint m);
vector<vector<double>*> transpose(const vector<vector<double>*> &matrix);
vector<vector<double>*> operator *(const vector<vector<double>*> &first,
		const vector<vector<double>*> &second);
vector<vector<double>*> inversion(const vector<vector<double>*> &matrix);
vector<vector<double>*> pseudo_inversion(const vector<vector<double>*> &matrix);

} //namespace matrix_utils

#endif /* MATRIXUTILS_H_ */
