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

// Character keys defining keyboard layout.
#define CHAR_DECIMAL '.'
#define CHAR_FRACTION '_'
#define CHAR_PLUSMINUS '~'

#endif /* TYPES_H */