#include <cmath>
#include "TToken.h"

/** Floating point precision. */
#define EPS 1e-12

/**
 * Converts the current value into an improper fraction (i.e. numerator and
 * denominator only), returning a value indicating whetheth this token can be
 * represented as such.
 * @param pArg Token to process. This may be a non-decimal value token, or a
 * fraction token.
 * @param pnum (Output) Reference to numerator, filled on exit.
 * @param pdenom (Output) Reference to numerator, filled on exit.
 */
bool asFractionParts(TToken* pArg, int* pnum, int *pdenom) {
    if (pArg->type() == TOKEN_FRACTION) {
        return ((TTokenFraction*)pArg)->toFractionParts(pnum, pdenom);
    }
    
    double value =((TTokenResultBase*)pArg)->value();
    int ivalue = (int) std::round(value);
    if (std::abs(value - ivalue) < EPS) {
        *pnum = ivalue;
        *pdenom = 1;
        return true;
    }

    return false;
}

TToken* TTokenBinaryOp::evaluate(TToken* pArg1, TToken* pArg2) {
    eTokenType type1 = pArg1->type();
    eTokenType type2 = pArg2->type();

    /** Numerical arguments for unary or binary operators. */
    double dArg1 = ((TTokenResultBase*)pArg1)->value();
    double dArg2 = ((TTokenResultBase*)pArg2)->value();

    int iNum1 = 0, iNum2 = 0, iNum = 0;
    int iDenom1 = 1, iDenom2 = 1, iDenom = 0;
    bool asFraction = 
        (type1 == TOKEN_FRACTION || type2 == TOKEN_FRACTION)
        && asFractionParts(pArg1, &iNum1, &iDenom1)
        && asFractionParts(pArg2, &iNum2, &iDenom2);

    /** Evaluated value for numerical tokens. */
    double dVal;

    switch (m_action) {
        case ACTION_ADD:
            if (asFraction) {
                iNum = iNum1 * iDenom2 + iNum2 * iDenom1;
                iDenom = iDenom1 * iDenom2;
            } else {
                dVal = dArg1 + dArg2;
            }
            break;

        case ACTION_SUB:
            if (asFraction) {
                iNum = iNum1 * iDenom2 - iNum2 * iDenom1;
                iDenom = iDenom1 * iDenom2;
            } else {
                dVal = dArg1 - dArg2;
            }
            break;

        case ACTION_MULT:
        case ACTION_MULTNEG:
            if (asFraction) {
                iNum = iNum1 * iNum2;
                iDenom = iDenom1 * iDenom2;
            } else {
                dVal = dArg1 * dArg2;
            }
            break;

        case ACTION_DIV:
            if (asFraction) {
                iNum = iNum1 * iDenom2;
                iDenom = iDenom1 * iNum2;
            } else {
                dVal = dArg1 / dArg2;
            }
            break;
    }

    if (asFraction) {
        return new TTokenFraction(0, iNum, iDenom);
    }
    return new TTokenValue(dVal);
}
