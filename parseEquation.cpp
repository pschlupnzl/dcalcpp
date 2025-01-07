#include <iostream>
#include <string>
#include "eLookFor.h"
#include "CCalculate.h"

/**
 * Process the next operator, evaluating higher precedence operators
 * before adding the new one.
 * @param iThisOp Current operator to be added.
 */
void processOps (
  int iThisOp,
  std::vector<TToken*> &pvoEquation,
  std::vector<ITokenOp*> &isOps
) {
    /** Previous operator on stack. */
    ITokenOp* iPrevOp;

    for (; isOps.size() > 0; ) {
        iPrevOp = isOps.back();
        if (iPrevOp->op() >= iThisOp) {
            // Add to RPN stack.
            isOps.pop_back();
            pvoEquation.push_back(iPrevOp);
        } else {
            // Exit on lower precedence.
            break;
        }
    }
}

void processOps(
    ITokenOp* iThisOp,
    std::vector<TToken*> &pvoEquation,
    std::vector<ITokenOp*> &isOps
) {
    return processOps(iThisOp->op(), pvoEquation, isOps);
}

/**
 * Parse the equation tokens into the RPN stack. This takes into account
 * operator precedence and such.
 */
void CCalculate::parseEquation() {
    m_lastErrorLocation = 0;
    m_lastError = ERROR_NONE;
    reset_pvoEquation();
    int iBrktOff = 0;
    eLookFor uLookFor = eLookFor::LOOKFOR_NUMBER;
    std::vector<ITokenOp*> isOps;


    size_t n = m_scan.size();
    for (size_t iThisPt = 0; iThisPt < n; iThisPt++) {
        TScan* scan = m_scan[iThisPt];
        eScanType scanType = scan->type();
        switch (uLookFor) {
            case eLookFor::LOOKFOR_NUMBER:
                if (
                    scanType == eScanType::SCAN_NUMBER
                    // token.typ === eTokenType.FRACTION ||
                    // token.typ === eTokenType.TIME ||
                    // token.typ === eTokenType.ANGLE ||
                    // token.typ === eTokenType.CONSTANT
                ) {
                    // TODO: if dVal<0: push_back(-1*)
                    m_pvoEquation.push_back(((TScanNumber*)scan)->toToken());
                    //// std::cout << "parseEquation@67 m_pvoEquation push number " << scan->toString() << std::endl;
                    uLookFor = eLookFor::LOOKFOR_BINARYOP;
                } else if (scanType == eScanType::SCAN_OPEN) {
                    iBrktOff += 1;
                    //// uLookFor = eLookFor::LOOKFOR_NUMBER;
                } else if (scanType == eScanType::SCAN_UNARYOP) {
                    //// std::cout << "parseEquation@71 isOps push unary op" << std::endl;
                    isOps.push_back(
                        ((TScanUnaryOp*)scan)->toToken(iBrktOff)
                    );
                    uLookFor = eLookFor::LOOKFOR_BRACKET;
                } else {
                    return errorParsing(iThisPt, PARSE_NUMBER_EXPECTED);
                }
                break;

            case eLookFor::LOOKFOR_BINARYOP:
                if (scanType == eScanType::SCAN_BINARYOP) {
                    TTokenBinaryOp* binaryToken = 
                        ((TScanBinaryOp*)scan)->toToken(iBrktOff);
                    processOps(binaryToken, m_pvoEquation, isOps);
                    isOps.push_back(binaryToken);
                    uLookFor = eLookFor::LOOKFOR_NUMBER;
                } else if (scanType == eScanType::SCAN_POSTUNARYOP) {
                    TTokenPostUnaryOp* postUnaryToken =
                        ((TScanPostUnaryOp*)scan)->toToken(iBrktOff);
                    processOps(postUnaryToken, m_pvoEquation, isOps);
                    isOps.push_back(postUnaryToken);
                } else if (scanType == eScanType::SCAN_CLOSE) {
                    iBrktOff -= 1;
                    if (iBrktOff < 0) {
                        return errorParsing(iThisPt, PARSE_MISSING_OPEN);
                    }
                    uLookFor = eLookFor::LOOKFOR_BINARYOP;
                } else {
                    return errorParsing(iThisPt, PARSE_BINARYOP_EXPECTED);
                }
                break;

            case eLookFor::LOOKFOR_BRACKET:
                if (scanType == eScanType::SCAN_OPEN) {
                    iBrktOff += 1;
                    uLookFor = eLookFor::LOOKFOR_NUMBER;
                } else {
                    return errorParsing(iThisPt, PARSE_NUMBER_EXPECTED);
                }
                break;
                
        } // end switch
    } // end for

    if (iBrktOff > 0) {
        // NOP - Automatically close brackets.
    } else if (uLookFor == eLookFor::LOOKFOR_NUMBER) {
        return errorParsing(n, PARSE_NUMBER_EXPECTED);
    }

    //// std::cout << "parseEquation@113 Final processOps" << std::endl;
    processOps(-1, m_pvoEquation, isOps);

}

