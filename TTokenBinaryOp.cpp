#include "CCalculate.h"
#include "TToken.h"



/**
 * Updates the operator token with precedence and bracket offset.
 * @param {number} iBrktOff Current bracket offset.
 */
void TTokenBinaryOp::setPrecedence(int iBrktOff) {
    m_op = BINARY_OP_PRECEDENCE(m_action)
        + (m_asTax || m_asPercent ? OP_PERCENTOFFSET : 0);
}


// void withPrecedence (TTokenBinaryOp tok, int iBrktOff) {
// //   token: TTokenBinaryOp | TTokenBinaryPow | TTokenUnaryOp,
// //   iBrktOff: number
// // ): TValOpOp => {
//     tok.m_op
//   var binaryOp = BinaryOpPrecedence(token.action),
//     unaryOp = UnaryOpPrecedence.indexOf(token.action),
//     op = unaryOp >= 0 ? unaryOp + OP_UNARY : binaryOp;

//   // Percent and tax operations take higher precedence: 100 + 10% / 2 = 105.
//   if (token.typ === eTokenType.BINARYOP && (token.asPercent || token.asTax)) {
//     op += 0.5;
//   }

//   return { ...token, op: op + iBrktOff };
// };

// /**
//  * Updates the unary operator token with precedence and bracket offset.
//  */
// void withPrecedence (TTokenUnaryOp tok, int iBrktOff) {

// }
