//
// Created by Kevin Wan on 2022-02-01.
//

#include <iostream>
#include "LCPOperatorPI.h"
#include "scisim/Utilities.h"
#include <chrono>
#include <Eigen/LU>

LCPOperatorPI::LCPOperatorPI(const scalar &tol, const unsigned &max_iters)
: m_tol (tol), max_iters (max_iters)
{
  assert(m_tol >= 0);
}

LCPOperatorPI::LCPOperatorPI(std::istream &input_stream)
: m_tol (Utilities::deserialize<scalar>( input_stream ))
, max_iters (Utilities::deserialize<unsigned>(input_stream))
{
  assert(m_tol >= 0);
}

// Returns the error of our solution
scalar getPolicy(const SparseMatrixsc &Q, const VectorXs &x, const VectorXs &b, SparseMatrixsc &policy)
{
  scalar err2 = 0;
  VectorXs y = Q * x + b;
  for (int i = 0; i < x.size(); ++i) {
    scalar choice;
    if (y(i) < x(i))
      choice = 1;
    else
      choice = 0;
    err2 += fmin(x(i), y(i)) * fmin(x(i), y(i));
    policy.coeffRef(i, i) = choice;
  }
  return sqrt(err2);
}

void updateValue(const SparseMatrixsc &policy, const SparseMatrixsc &Q, const VectorXs &b, VectorXs &x)
{
  Eigen::ConjugateGradient<SparseMatrixsc, Eigen::Lower|Eigen::Upper> cg;
  SparseMatrixsc I {b.size(), b.size()};
  I.setIdentity();
  cg.compute(policy * Q * policy + I - policy);
  x = cg.solve(-policy * b);
}

void reportTime(const std::chrono::time_point<std::chrono::system_clock> &start)
{
  std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;
  std::cout << "LCPOperatorPI: Time elapsed: " << elapsed_seconds.count() << "s\n";
}

void LCPOperatorPI::flow(const std::vector<std::unique_ptr<Constraint>> &cons, const SparseMatrixsc &M,
                         const SparseMatrixsc &Minv, const VectorXs &q0, const VectorXs &v0, const VectorXs &v0F,
                         const SparseMatrixsc &N, const SparseMatrixsc &Q, const VectorXs &nrel, const VectorXs &CoR,
                         VectorXs &alpha) {
  std::cout << "LCPOperatorPI: Solving LCP of size " << N.cols() << std::endl;
  // Get initial time
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  VectorXs b { N.transpose() * (1 + CoR(0)) * v0 };
  // Solve complementarity with x and Qx + b
  VectorXs x { b.size() }; // Initial guess of 0 (maybe change later)
  SparseMatrixsc policy { x.size(), x.size() };
  scalar error = 0;
  for (unsigned n_iter = 0; n_iter <= max_iters; ++n_iter)
  {
    error = getPolicy(Q, x, b, policy);
    if (error <= m_tol) {
      alpha = x;
      std::cout << "LCPOperatorPI: Converged in " << n_iter << " iterations." << std::endl;
      reportTime(start);
      return;
    }
    if (n_iter == max_iters)
      break;
    updateValue(policy, Q, b, x);
  }
  std::cout << "LCPOperatorPI: Failed to converge in " << max_iters << " iterations." << std::endl;
  reportTime(start);
  std::cerr << "LCPOperatorPI: Result did not converge" << std::endl;
  std::cerr << "LCPOperatorPI: Error is: " << error << std::endl;
  std::cerr << "LCPOperatorPI: Failed with size: " << N.cols() << std::endl;
  alpha = x;
}

std::string LCPOperatorPI::name() const {
  return "lcp_policy_iteration";
}

std::unique_ptr<ImpactOperator> LCPOperatorPI::clone() const {
  return std::unique_ptr<ImpactOperator>(new LCPOperatorPI(m_tol, max_iters));
}

void LCPOperatorPI::serialize(std::ostream &output_stream) const {
  Utilities::serialize(m_tol, output_stream);
}

bool isInvertible(const SparseMatrixsc &M) {
  Eigen::FullPivLU<SparseMatrixsc> lu(M);
  return lu.isInvertible();
}

bool isMMatrix(const SparseMatrixsc &M)  {
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


