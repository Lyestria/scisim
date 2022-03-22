// ImpactOperator.cpp
//
// Breannan Smith
// Last updated: 09/03/2015

#include "ImpactOperator.h"

ImpactOperator::~ImpactOperator()
{}

bool ImpactOperator::isMMatrix(const SparseMatrixsc &M) {
  assert(M.rows() == M.cols());
  for (int i = 0; i < M.outerSize(); ++i) {
    for (SparseMatrixsc::InnerIterator it(M, i); it; ++it) {
      if (it.row() == it.col() && it.value() <= 0)
        return false;
      if (it.row() != it.col() && it.value() > 0)
        return false;
    }
  }
  return true;
}
