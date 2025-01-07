#include "TToken.h"
#include "types.h"

#include <iostream>

TToken* TTokenPostUnaryOp::evaluate(TToken *pArg)
{
    eTokenType type = pArg->type();
    double dVal = ((TTokenResultBase*)pArg)->value();

    int num, denom;
    bool neg;
    bool asFraction = type == eTokenType::TOKEN_FRACTION &&
        ((TTokenFraction*)pArg)->toSignedFraction(&num, &denom, &neg);
    /** Value indicating whether the **result** can be a fraction. */
    bool isFraction = false;

    switch (m_action) {
        case ePostUnaryOpAction::POST_UNARY_OP_POW2:
            if (asFraction) {
                num = num * num;
                denom = denom * denom;
                neg = false;
                isFraction = true;
            } else {
                dVal = dVal * dVal;
            }
            break;
    }

    if (isFraction) {
        return new TTokenFraction(num, denom);
    }
    return new TTokenValue(dVal);
}

