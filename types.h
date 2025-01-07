#ifndef TYPES_H
#define TYPES_H

// Arduino style short integer.
#ifndef int16_t
typedef short int int16_t;
// #define int16_t short int
#endif /* int16_t */

// Arduino style unsigned short integer.
#ifndef uint16_t
typedef unsigned short int uint16_t;
// #define uint16_t unsigned short int
#endif /* uint16_t */

/** Floating point precision. */
#define EPS 1e-12


#ifndef ABS
// Absolute value as ternary rather than math operator.
#define ABS(x) ((x) < 0 ? (-(x)) : (x))
#endif /* ABS */

/** Options for evaluating and rendering results. */
typedef struct ICalcOptions {
  /** Value indicating whether to use Radians for trig functions (instead of rad). */
  bool trigRad;
  /** Decimal separator, or 0x00 for default '.'. */
  char deciSep;
  /** Thousands separator, or 0x00 for none. */
  char thouSep;
} ICalcOptions;

#endif /* TYPES_H */