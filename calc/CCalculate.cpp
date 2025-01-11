#include <iostream>
#include <string>
#include "CCalculate.h"
#include "TScan.h"

void CCalculate::reset() {
    reset_actions();
    reset_scan();
    reset_pvoEquation();
    reset_result();
}

void CCalculate::reset_actions() {
    while (m_actions.size() > 0) {
        m_actions.pop_back();
    }
}

void CCalculate::reset_scan() {
    while (m_scan.size() > 0) {
        IScan* scan = m_scan.back();
        m_scan.pop_back();
        delete scan;
        // switch (scan->type()) {
        //     case eScanType::SCAN_NUMBER:
        //         delete (TScanNumber*) scan;
        //         break;
        //     case eScanType::SCAN_BINARYOP:
        //         delete (TScanBinaryOp*) scan;
        //         break;
        //     case eScanType::SCAN_UNARYOP:
        //         delete (TScanUnaryOp*) scan;
        //         break;
        //     case eScanType::SCAN_POSTUNARYOP:
        //         delete (TScanPostUnaryOp*) scan;
        //         break;
        //     case eScanType::SCAN_OPEN:
        //         delete (TScanOpen*) scan;
        //         break;
        //     case eScanType::SCAN_CLOSE:
        //         delete (TScanClose*) scan;
        //         break;
        // }
    }
}

void CCalculate::reset_pvoEquation() {
    while (m_pvoEquation.size() > 0) {
        IToken* token = m_pvoEquation.back();
        m_pvoEquation.pop_back();
        delete token;
        // switch (token->type()) {
        //     case eTokenType::TOKEN_VALUE:
        //         delete (TTokenValue*) token;
        //         break;
        //     case eTokenType::TOKEN_FRACTION:
        //         delete (TTokenFraction*) token;
        //         break;
        //     case eTokenType::TOKEN_BINARYOP:
        //         delete (TTokenBinaryOp*) token;
        //         break;
        //     case eTokenType::TOKEN_UNARYOP:
        //         delete (TTokenUnaryOp*) token;
        //         break;
        //     case eTokenType::TOKEN_POSTUNARYOP:
        //         delete (TTokenPostUnaryOp*) token;
        //         break;
        // }
    }
}

void CCalculate::reset_result() {
    // TODO:
    // Because we're *NOT* taking a copy of dsVals[0] at the end of
    // evalEquation, we do not need to free anything here.
    if (m_presult != nullptr) {
        // delete m_presult;
        m_presult = nullptr;
    }
}

std::string CCalculate::toString(const ICalcOptions &options)
{
    std::string str;
    str += "Scan:\n ";
    for (IScan* scan : m_scan) {
        str += "‹" + scan->toString(options) + "› ";
    }
    str += "\n";

    str += "Tokens:\n ";
    for (IToken* tok : m_pvoEquation) {
        str += "«" + tok->toString(options) + "» ";
    }
    str += "\n";

    if (m_presult != nullptr) {
        str += "= " + m_presult->toString(options) + "\n";
    }

    return str;
}

std::string CCalculate::toDisplayString(const ICalcOptions &options) {
    std::string str;
    for (IScan* scan : m_scan) {
        str += scan->toString(options) + " ";
    }
    if (m_presult != nullptr) {
        str += "=" + m_presult->toString(options);
    }
    return str;
}

void CCalculate::forEach(const std::function<void(IScan *)> fn)
{
    for (IScan* scan : m_scan) {
        fn(scan);
    }
}

