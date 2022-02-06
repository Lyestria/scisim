//
// Created by Kevin Wan on 2022-02-01.
//

#include "LCPOperatorPI.h"
#include "scisim/Utilities.h"

LCPOperatorPI::LCPOperatorPI(const scalar &tol)
: m_tol (tol)
{
    assert(m_tol >= 0);
}

LCPOperatorPI::LCPOperatorPI(std::istream &input_stream)
: m_tol (Utilities::deserialize<scalar>( input_stream ))
{
    assert(m_tol >= 0);
}

void LCPOperatorPI::flow(const std::vector<std::unique_ptr<Constraint>> &cons, const SparseMatrixsc &M,
                         const SparseMatrixsc &Minv, const VectorXs &q0, const VectorXs &v0, const VectorXs &v0F,
                         const SparseMatrixsc &N, const SparseMatrixsc &Q, const VectorXs &nrel, const VectorXs &CoR,
                         VectorXs &alpha) {

}

std::string LCPOperatorPI::name() const {
    return "lcp_policy_iteration";
}

std::unique_ptr<ImpactOperator> LCPOperatorPI::clone() const {
    return std::unique_ptr<ImpactOperator>(new LCPOperatorPI(m_tol));
}

void LCPOperatorPI::serialize(std::ostream &output_stream) const {
    Utilities::serialize(m_tol, output_stream);
}

