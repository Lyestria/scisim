//
// Created by Kevin Wan on 2022-02-01.
//

#ifndef SCISIM_LCP_OPERATOR_PI_H
#define SCISIM_LCP_OPERATOR_PI_H

#include "ImpactOperator.h"

class LCPOperatorPI final : public ImpactOperator {

public:

    explicit LCPOperatorPI(const scalar& tol);
    explicit LCPOperatorPI( std::istream& input_stream );

    virtual ~LCPOperatorPI() override = default;

    virtual void flow( const std::vector<std::unique_ptr<Constraint>>& cons, const SparseMatrixsc& M, const SparseMatrixsc& Minv, const VectorXs& q0, const VectorXs& v0, const VectorXs& v0F, const SparseMatrixsc& N, const SparseMatrixsc& Q, const VectorXs& nrel, const VectorXs& CoR, VectorXs& alpha ) override;

    virtual std::string name() const override;

    virtual std::unique_ptr<ImpactOperator> clone() const override;

    virtual void serialize( std::ostream& output_stream ) const override;

private:
    const scalar m_tol;
};


#endif //SCISIM_LCP_OPERATOR_PI_H
