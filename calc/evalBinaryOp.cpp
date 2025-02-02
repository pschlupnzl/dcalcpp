#include <math.h>
#include "TToken.h"
#include "types.h"

/**
 * Converts the current value into a **signed improper** fraction (i.e. whole
 * number is zero, numerator is signed, denominator is positive, and the
 * negative flag is set), returning a value indicating whether this token can be
 * represented as such.
 * @param pArg Token to process. This may be a non-decimal value token, or a
 * fraction token.
 * @param pnum (Output) Reference to numerator, filled on exit.
 * @param pdenom (Output) Reference to denominator, filled on exit.
 * @param negative (Output) Reference to negative flag, filled on exit.
 */
bool asSignedFraction(ITokenResultBase* pArg, int* pnum, int *pdenom, bool *pneg) {
    if (pArg->type() == TOKEN_FRACTION) {
        return ((TTokenFraction*)pArg)->toSignedFraction(pnum, pdenom, pneg);
    }
    
    double value = pArg->value();
    int ivalue = (int) round(value);
    if (abs(value - ivalue) < EPS) {
        // Return whole number as fraction.
        *pnum = ivalue;
        *pdenom = 1;
        *pneg = ivalue < 0;
        return true;
    }

    // Decimal value not processed as fraction.
    return false;
}

ITokenResultBase* TTokenBinaryOp::evaluate(
    ITokenResultBase* pArg1,
    ITokenResultBase* pArg2
) {
    eTokenType type1 = pArg1->type();
    eTokenType type2 = pArg2->type();

    /** Numerical arguments for unary or binary operators. */
    double dArg1 = pArg1->value();
    double dArg2 = pArg2->value();

    int iNum1 = 0, iNum2 = 0, iNum = 0;
    int iDenom1 = 1, iDenom2 = 1, iDenom = 0;
    bool neg1 = false, neg2 = false;
    bool asFraction = 
        (type1 == eTokenType::TOKEN_FRACTION || 
            type2 == eTokenType::TOKEN_FRACTION)
        && asSignedFraction(pArg1, &iNum1, &iDenom1, &neg1)
        && asSignedFraction(pArg2, &iNum2, &iDenom2, &neg2);
    /** Value indicating whether result is a fraction. */
    bool isFraction = false;

    /** Evaluated value for numerical tokens. */
    double dVal;

    switch (m_action) {
        case eBinaryOpAction::BINARY_OP_ADD:
            if (asFraction) {
                iNum = iNum1 * iDenom2 + iNum2 * iDenom1;
                iDenom = iDenom1 * iDenom2;
                isFraction = true;
            } else {
                dVal = dArg1 + dArg2;
            }
            break;

        case eBinaryOpAction::BINARY_OP_SUB:
            if (asFraction) {
                iNum = iNum1 * iDenom2 - iNum2 * iDenom1;
                iDenom = iDenom1 * iDenom2;
                isFraction = true;
            } else {
                dVal = dArg1 - dArg2;
            }
            break;

        case eBinaryOpAction::BINARY_OP_MULT:
        case eBinaryOpAction::BINARY_OP_MULTNEG:
            if (asFraction) {
                iNum = iNum1 * iNum2;
                iDenom = iDenom1 * iDenom2;
                isFraction = true;
            } else {
                dVal = dArg1 * dArg2;
            }
            break;

        case eBinaryOpAction::BINARY_OP_DIV:
            if (asFraction) {
                iNum = iNum1 * iDenom2;
                iDenom = iDenom1 * iNum2;
                isFraction = true;
            } else {
                dVal = dArg1 / dArg2;
            }
            break;

        case eBinaryOpAction::BINARY_OP_POW:
            dVal = pow(dArg1, dArg2);
            break;

        case eBinaryOpAction::BINARY_OP_ROOT:
            dVal = pow(dArg2, 1.00 / dArg1);
            break;
        
        default: /* nop */ break;
    }

    if (isFraction) {
        return new TTokenFraction(iNum, iDenom);
    }
    return new TTokenValue(dVal);
}
