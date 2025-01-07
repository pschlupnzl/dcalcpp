#include <math.h>
#include <cmath>
#include "TToken.h"
#include "types.h"

TToken* TTokenUnaryOp::evaluate(TToken *pArg)
{
    eTokenType type = pArg->type();
    double dVal = ((TTokenResultBase*)pArg)->value();

    int num, denom;
    bool neg;
    bool asFraction = type == eTokenType::TOKEN_FRACTION &&
        ((TTokenFraction*)pArg)->toSignedFraction(&num, &denom, &neg);

    /** Value indicating whether the **result** can be a fraction. */
    bool isFraction = false;

    double dnum, ddenom;
    switch (m_action) {
        case eUnaryOpAction::UNARY_OP_SQRT:
            if (dVal < 0) {
                dVal = NAN;
                break;
            }
            dVal = sqrt(dVal);

            if (asFraction) {
                dnum = sqrt((double)num);
                ddenom = sqrt((double)denom);
                if (std::abs(dnum - std::round(dnum)) < EPS &&
                    std::abs(ddenom - std::round(ddenom)) < EPS) {
                    num = (int) dnum;
                    denom = (int) ddenom;
                    isFraction = true;
                }
            }
            break;

    }

    if (isFraction) {
        return new TTokenFraction(num, denom);
    }
    return new TTokenValue(dVal);
}
