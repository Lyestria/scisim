// ImpactOperator.cpp
//
// Breannan Smith
// Last updated: 09/03/2015

#include "ImpactOperator.h"
#include <Eigen/Eigenvalues>

ImpactOperator::~ImpactOperator()
{}

// We assume an M-Matrix has only positive entries on the diagonal and non-positive entries elsewhere
// The deviance returns the greatest deviation from this on both the diagonal and off-diagonal entries
std::pair<double, double> ImpactOperator::MMatrixDeviance(const SparseMatrixsc &M) {
  assert(M.rows() == M.cols());
  double diagonal_deviance = 0.0;
  double off_diagonal_deviance = 0.0;
  for (int i = 0; i < M.outerSize(); ++i) {
    for (SparseMatrixsc::InnerIterator it(M, i); it; ++it) {
      if (it.row() == it.col() && it.value() <= 0)
        diagonal_deviance = std::max(diagonal_deviance, -it.value());
      else if (it.row() != it.col() && it.value() >= 0)
        off_diagonal_deviance = std::max(off_diagonal_deviance, it.value());
    }
  }
  return {diagonal_deviance, off_diagonal_deviance};
}

// Returns the maximum deviation from a diagonal dominant matrix on any of the rows.
double ImpactOperator::DiagonalDominanceDeviance(const SparseMatrixsc &M) {
  assert(M.rows() == M.cols());
  double max_deviance = 0.0;
  for (int i = 0; i < M.outerSize(); ++i) {
    double diag = 0.0, sum = 0.0;
    for (SparseMatrixsc::InnerIterator it(M, i); it; ++it) {
      if (it.row() == it.col())
        diag += std::abs(it.value());
      else
        sum += std::abs(it.value());
    }
    double dev = std::max(0.0, diag - sum);
    max_deviance = std::max(max_deviance, dev);
  }
  return max_deviance;
}

std::vector<double> ImpactOperator::getEigenvalues(const SparseMatrixsc &M) {
  assert(M.rows() == M.cols());
  auto dense = M.toDense();
  auto eigenvalues = dense.eigenvalues().real();
  std::vector<double> eigenvalues_vec(eigenvalues.data(), eigenvalues.data() + eigenvalues.size());
  return eigenvalues_vec;
}

double ImpactOperator::getConditionNumber(const SparseMatrixsc &M) {
  assert(M.rows() == M.cols());
  auto dense = M.toDense();
  auto eigenvalues = dense.eigenvalues().real();
  double max_eigenvalue = *std::max_element(eigenvalues.data(), eigenvalues.data() + eigenvalues.size());
  double min_eigenvalue = *std::min_element(eigenvalues.data(), eigenvalues.data() + eigenvalues.size());
  return max_eigenvalue / min_eigenvalue;

}