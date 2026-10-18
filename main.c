/*
program should be compiled without -DNDEBUG.
assert is used for errors to end program execution.
*/
//try all cases/asserts
//for testing: 0, -0, +0, 5, -5, 123456789012345, 066, -, empty Enter, 12a3,

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

typedef struct BigInt {
    uint8_t *digits;
    size_t   length;
    bool     negative;
} BigInt;

typedef struct DivResult {
    BigInt quotient;
    BigInt remainder;
} DivResult;

DivResult big_int_div(const BigInt* first_op, const BigInt* second_op);
BigInt get_big_number(void);
void free_big_int(BigInt *big_int);

int main(void)
{
    BigInt first_op  = get_big_number();
    BigInt second_op = get_big_number();

    for(size_t i = 0; i < first_op.length; i++)
    {
        printf("%hhu",first_op.digits[i]);
    }
    printf("\nLength: %ld\n", first_op.length);
    printf("Negative: %d\n", first_op.negative);

    for(size_t i = 0; i < second_op.length; i++)
    {
        printf("%hhu",second_op.digits[i]);
    }
    printf("\nLength: %ld\n", second_op.length);
    printf("Negative: %d\n", second_op.negative);


    free_big_int(&first_op);
    free_big_int(&second_op);

    return 0;
}


DivResult big_int_div(const BigInt* first_op, const BigInt* second_op)
{
    DivResult ret_val;
    ret_val.quotient.negative  = first_op->negative ^ second_op->negative; // ^ xor operator
    ret_val.remainder.negative = false;

    

    return ret_val;
}

BigInt get_big_number(void)
{
    BigInt ret_val;

    size_t capacity = 8;
    ret_val.digits = malloc(capacity * sizeof(uint8_t));
    assert(ret_val.digits != nullptr);
    ret_val.length = 0;
    ret_val.negative = false; //by default it's a positive number

    printf("Enter the number: ");
    fflush(stdout);
    
    int c; //int because return value of func getchar() is int

    c = getchar();
    //assert(c != '0'); //number can't start with a zero, reject leading zeros
    if     (c == '-') {ret_val.negative = true;  c = getchar();}
    else if(c == '+') {ret_val.negative = false; c = getchar();}

    if (c == '0')
    {
        int next = getchar();
        assert(next == '\n' || next == EOF);  //number can't start with zero, reject leading zeros
        ungetc(next, stdin); //ungetc(EOF) doesn't return anything
    }

    while (c != '\n' && c != EOF)
    {
        int digit = c - '0'; //get int from ascii
        assert(digit >= 0 && digit <= 9);

        ret_val.digits[ret_val.length++] = (uint8_t)digit;
        if (ret_val.length >= capacity)
        {
            capacity *= 2;
            ret_val.digits = realloc(ret_val.digits, capacity * sizeof(uint8_t));
            assert(ret_val.digits != nullptr);
        }

        c = getchar();
    }

    assert(ret_val.length != 0);
    if(ret_val.length == 1 && ret_val.digits[0] == 0) {ret_val.negative = false;}


    uint8_t *tmp = realloc(ret_val.digits, ret_val.length * sizeof(uint8_t));
    if (tmp != nullptr) {ret_val.digits = tmp; }
    
    return ret_val;
}

void free_big_int(BigInt *big_int)
{
    free(big_int->digits);
    big_int->digits = nullptr;
    big_int->length = 0;
}