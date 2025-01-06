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

    switch (m_action) {
        case ePostUnaryOpAction::POST_UNARY_OP_POW2:
            if (asFraction) {
                num = num * num;
                denom = denom * denom;
                neg = false;
            } else {
                dVal = dVal * dVal;
            }
            break;
    }

    if (asFraction) {
        return new TTokenFraction(num, denom);
    }

    return new TTokenValue(dVal);
}

