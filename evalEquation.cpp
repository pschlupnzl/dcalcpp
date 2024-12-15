#include <iostream>
#include "CCalculate.h"

/**
 * Evaluate the equation.
 */
void CCalculate::evalEquation(IEvalEquationOptions &options) {
    reset_result();

    /** Number of tokens on the equation stack. */
    size_t n = m_pvoEquation.size();
    if (n <= 0) {
        return;
    }

    /** RPN stack of values being calculated. */
    std::vector<TToken*> dsVals;
    /** Generic arguments for binary or unary operators. */
    TToken *pArg1, *pArg2;

    // Iterate each token.
    for (size_t iThisPt = 0; iThisPt < n; iThisPt++) {
        TToken* pvoThisValop = m_pvoEquation[iThisPt];
        switch (pvoThisValop->type()) {
            case TOKEN_VALUE:
                dsVals.push_back(pvoThisValop);
                break;

            case TOKEN_BINARYOP:
                if (dsVals.size() < 2) {
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

        } // end switch
    } // end for

    if (dsVals.size() > 1) {
        return errorEvaluating(EVAL_STACK_NOT_EMPTY);
    }

    // Final answer.
    m_presult = dsVals[0];
}

