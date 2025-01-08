#include <math.h>
#include "TToken.h"
#include "types.h"

/** Conversion from degrees to radians. */
#define M_PI_180 (M_PI / 180.00)

ITokenResultBase* TTokenUnaryOp::evaluate(ITokenResultBase *pArg)
{
    /** Multiplier for arguments to math.h trig functions in radians. */
    // double trigToRad = 1.00; // M_PI_180
    double trigToRad = M_PI_180;

    eTokenType type = pArg->type();
    double dVal = pArg->value();

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
                if (abs(dnum - round(dnum)) < EPS &&
                    abs(ddenom - round(ddenom)) < EPS) {
                    num = (int) dnum;
                    denom = (int) ddenom;
                    isFraction = true;
                }
            }
            break;

        case eUnaryOpAction::UNARY_OP_SIN:
            dVal = sin(trigToRad * dVal);
            break;
        case eUnaryOpAction::UNARY_OP_COS:
            dVal = cos(trigToRad * dVal);
            break;
        case eUnaryOpAction::UNARY_OP_TAN:
            dVal = tan(trigToRad * dVal);
            break;
        case eUnaryOpAction::UNARY_OP_ASIN:
            dVal = asin(dVal) / trigToRad;
            break;
        case eUnaryOpAction::UNARY_OP_ACOS:
            dVal = acos(dVal) / trigToRad;
            break;
        case eUnaryOpAction::UNARY_OP_ATAN:
            dVal = atan(dVal) / trigToRad;
            break;
    }

    if (isFraction) {
        return new TTokenFraction(num, denom);
    }
    return new TTokenValue(dVal);
}
