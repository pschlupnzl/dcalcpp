#ifndef EERRORS_H
#define EERRORS_H

enum eErrors {
    /** No error. */
    ERROR_NONE,
    /** Looking for a number but didn't get a value token. */
    PARSE_NUMBER_EXPECTED,
    /** Looking for a binary operator but didn't get one. */
    PARSE_BINARYOP_EXPECTED,

    /** A binary operator does not have two operands to work on. */
    EVAL_BINARY_STACK_UNDERFLOW,
    /**
     * A binary operator could not be executed because the arguments didn't
     * match (this shouldn't happen). 
    */
    EVAL_BINARY_ARGUMENT_MISSING,
    /** The stack is not empty after finishing the evaluation. */
    EVAL_STACK_NOT_EMPTY,
};
#endif /* EERRORS_H */