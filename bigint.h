#ifndef BIGINT_H
#define BIGINT_H

#include <stdio.h>
#include <inttypes.h>

enum { CMP_LESS = -1, CMP_EQUAL = 0, CMP_GREATER = 1 };

typedef struct BigInt {
    uint8_t *digits;
    size_t   length;
    bool     negative;
} BigInt;

typedef struct DivResult {
    BigInt quotient;
    BigInt remainder;
} DivResult;

BigInt get_big_number(void);
DivResult big_int_div(const BigInt* dividend, const BigInt* divisor);

void free_big_int(BigInt* big_int);
void print_big_int(const BigInt* big_int);

#endif