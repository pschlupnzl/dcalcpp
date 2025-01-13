#include <iostream>
#include "CCalculate.h"

/**
 * Evaluate the equation.
 */
void CCalculate::evalEquation() {
    reset_result();

    /** Number of tokens on the equation stack. */
    size_t n = m_pvoEquation.size();
    if (n <= 0) {
        return;
    }

    /** RPN stack of values being calculated. */
    std::vector<ITokenResultBase*> dsVals;
    /** Generic arguments for binary or unary operators. */
    ITokenResultBase *pArg1, *pArg2;

    // Iterate each token.
    for (size_t iThisPt = 0; iThisPt < n; iThisPt++) {
        int dsValsSize = dsVals.size();
        IToken* pvoThisValop = m_pvoEquation[iThisPt];
        switch (pvoThisValop->type()) {
            case eTokenType::TOKEN_VALUE:
            case eTokenType::TOKEN_FRACTION:
                dsVals.push_back((ITokenResultBase*)pvoThisValop);
                break;


            case eTokenType::TOKEN_BINARYOP:
                if (dsValsSize < 2) {
                    return errorEvaluating(EVAL_BINARY_STACK_UNDERFLOW);
                }
                pArg2 = dsVals.back();
                dsVals.pop_back();
                pArg1 = dsVals.back();
                dsVals.pop_back();

                dsVals.push_back(((TTokenBinaryOp*)pvoThisValop)->evaluate(
                    pArg1, pArg2
                ));
                break;

            case eTokenType::TOKEN_UNARYOP:
                if (dsValsSize < 1) {
                    return errorEvaluating(EVAL_UNARY_STACK_UNDERFLOW);
                }
                pArg1 = dsVals.back();
                dsVals.pop_back();
                dsVals.push_back(((TTokenUnaryOp*)pvoThisValop)->evaluate(
                    pArg1,
                    m_options
                ));
                break;

            case eTokenType::TOKEN_POSTUNARYOP:
                if (dsValsSize < 1) {
                    return errorEvaluating(EVAL_POST_UNARY_STACK_UNDERFLOW);
                }
                pArg1 = dsVals.back();
                dsVals.pop_back();
                dsVals.push_back(((TTokenPostUnaryOp*)pvoThisValop)->evaluate(
                    pArg1
                ));
                break;

        } // end switch
    } // end for

    if (dsVals.size() > 1) {
        return errorEvaluating(EVAL_STACK_NOT_EMPTY);
    }

    // Final answer.
    // TODO: MEMORY LEAK! Take copy of dsVals[0] instead.
    m_presult = dsVals[0];
    if (m_presult->type() == TOKEN_FRACTION) {
        ((TTokenFraction*)m_presult)->simplify();
    }
}
