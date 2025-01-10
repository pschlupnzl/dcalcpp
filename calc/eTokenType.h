#ifndef ETOKENTYPE_H
#define ETOKENTYPE_H

/** Types of token. */
enum eTokenType {
  // TOKEN_UNDEFINED,
  // /** Input number only. */
  // SCAN_NUMBER,
  // /** Input binary operator. */
  // SCAN_BINARYOP,
  // /** Input decimal point. */
  // TOKEN_DECIMAL,
  // /** Negate current operand. */
  // TOKEN_NEGATE,
  /** Numerical value evaluated from TOKEN_NUMBER, e.g. "-23.45e-5". */
  TOKEN_VALUE,
  // /** Constant, e.g. Pi. */
  // TOKEN_CONSTANT,
  /** Binary infix operator, e.g. "+". */
  TOKEN_BINARYOP,
  // /**
  //  * Binary infix power operator, e.g. "^". Differs from {@link BINARYOP} only
  //  * by the allowed buttons, e.g. excludes time and angles.
  //  */
  // TOKEN_BINARYPOW,
  /** Unary operator or function, e.g. "sin". */
  TOKEN_UNARYOP,
  /** Unary operator after the argument, e.g. "!". */
  TOKEN_POSTUNARYOP,
  // /** Open bracket, "(". */
  // TOKEN_OPEN,
  // /** Close bracket, ")". */
  // TOKEN_CLOSE,
  /** Fraction, e.g. "-1_2_3" =-1⅔ */
  TOKEN_FRACTION,
  // /** Time, e.g. "113:45:33.55". */
  // TOKEN_TIME,
  // /** Angle in degrees, minutes, and seconds. */
  // TOKEN_ANGLE,
  // /** Percentage added or removed, e.g. «2 + 5 %». */
  // TOKEN_PERCENT,
  // /** Tax percentage added or removed, e.g. «2 + Tax». */
  // TOKEN_TAX,
  // /** Value to solve for. */
  // TOKEN_SOLVE,
  // /** Equality operator when solving for a value. */
  // TOKEN_EQUALS,
};

#endif /* ETOKENTYPE_H */