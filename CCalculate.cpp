#include <iostream>
#include <string>
#include "CCalculate.h"
#include "TScan.h"

// /**
//  * Reset the given array, deleting all items referenced by pointers in the
//  * array. This is useful with arrays where items are added with
//  * 
//  *     `arr.push_back(new Token(...));`
//  */
// template<class T>
// void reset_array(std::vector<T*>& arr) {
//     // TODO:
//     // I'm not sure whether I am responsible for deleting the (T*) item.
//     // Doing so triggers a segfault on Arduino / ESP32, but without it I'm not
//     // sure the memory gets freed properly.
//     // I tried making m_scan and m_pvoEquation of type std::vector<TToken&>
//     // but that didn't work either.
//     // I'm just gonna run with this for now.
//     while (arr.size() > 0) {
//         T* item = arr.back();
//         arr.pop_back();
//         // delete item;
//     }
//     // arr.clear();
// }

void CCalculate::reset() {
    reset_char();
    reset_scan();
    reset_pvoEquation();
    reset_result();
}

void CCalculate::reset_char() {
    while (m_char.size() > 0) {
        m_char.pop_back();
    }
}

void CCalculate::reset_scan() {
    while (m_scan.size() > 0) {
        TScan* scan = m_scan.back();
        m_scan.pop_back();
        delete scan;
    }
}

void CCalculate::reset_pvoEquation() {
    while (m_pvoEquation.size() > 0) {
        TToken* token = m_pvoEquation.back();
        m_pvoEquation.pop_back();
        switch (token->type()) {
            case TOKEN_BINARYOP:
                delete (TTokenBinaryOp*) token;
                break;
            case TOKEN_FRACTION:
                delete (TTokenFraction*) token;
                break;
            case TOKEN_VALUE:
                delete (TTokenValue*) token;
                break;
            default:
                // SHOULD NOT HAPPEN
                break;
        }
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

std::string CCalculate::toString()
{
    std::string str;
    str += "Scan:\n ";
    for (TScan* scan : m_scan) {
        str += "‹" + scan->toString() + "› ";
    }
    str += "\n";

    str += "Tokens:\n ";
    for (TToken* tok : m_pvoEquation) {
        str += "«" + tok->toString() + "» ";
    }
    str += "\n";

    if (m_presult != nullptr) {
        str += "= " + m_presult->toString() + "\n";
    }

    return str;
}

void CCalculate::forEach(const std::function<void(TScan *)> fn)
{
    for (TScan* scan : m_scan) {
        fn(scan);
    }
}

