#include "eScanType.h"
#include "CCalculate.h"
#include "eAction.h"

void CCalculate::scan(const char ch) {
    if (scanSilent(ch)) {
        m_char.push_back(ch);
    };
}

bool CCalculate::scanSilent(const char ch) {
    TScan* last = m_scan.size() <= 0 
        ? nullptr 
        : m_scan.back();
    eScanType type = last ? last->type() : SCAN_UNDEFINED;

    switch (ch) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case eAction::ACTION_DECIMAL:
        case eAction::ACTION_FRACTION:
        case eAction::ACTION_NEGATE:
            if (!last || type != SCAN_NUMBER) {
                last = new TScanNumber();
                m_scan.push_back(last);
            }
            ((TScanNumber*) last)->append(ch);

            break;

        case '+':
        case '*':
        case '-':
        case '/':
            if (type == SCAN_NUMBER || type == SCAN_CLOSE) {
                m_scan.push_back(new TScanBinaryOp(
                    ch == '+' ? BINARY_OP_ACTION_ADD :
                    ch == '*' ? BINARY_OP_ACTION_MULT :
                    ch == '-' ? BINARY_OP_ACTION_SUB :
                    ch == '/' ? BINARY_OP_ACTION_DIV :
                    BINARY_OP_ACTION_ADD
                ));
            }
            break;

        case '(':
            m_scan.push_back(new TScanOpen());
            break;

        case ')':
            m_scan.push_back(new TScanClose());
            break;
        default:
            // All other characters not scanned.
            return false;
    }
    return true;
}

void CCalculate::backspace() {
    if (m_char.size() > 0) {
        m_char.pop_back();
        reset_scan();
        for (char ch : m_char) {
            scanSilent(ch);
        }
    }
}