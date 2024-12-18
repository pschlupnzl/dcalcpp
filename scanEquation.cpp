#include "eScanType.h"
#include "CCalculate.h"

void CCalculate::scan(const char ch) {
    TScan* last = m_scan.size() <= 0 
        ? nullptr 
        : m_scan.back();
    eScanType type = last ? last->type() : SCAN_UNDEFINED;

    switch (ch) {
        case ' ':
            // Ignore whitespace.
            break;

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
        case '.':
        case '_':
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
    }    
}

