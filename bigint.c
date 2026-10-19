#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "bigint.h"

constexpr int BASE = 10;
constexpr int CAPACITY_START = 8;

static void append_digit(BigInt *big_int, uint8_t digit);
static int_fast8_t compare(const BigInt* first_op, const BigInt* second_op);
static void subtract(BigInt* first_op, const BigInt* second_op);


DivResult big_int_div(const BigInt* dividend, const BigInt* divisor)
{
    assert(dividend->length > 0 && divisor->length > 0);

    if (divisor->digits[0] == 0 && divisor->length == 1)
    {
        return (DivResult) { .quotient  = {nullptr, 0, false}, 
                             .remainder = {nullptr, 0, false} }; //digits == nullptr is an error signal
    }
    
    DivResult ret_val;

    ret_val.quotient.negative  = dividend->negative ^ divisor->negative; // ^ xor operator
    ret_val.quotient.digits = malloc(dividend->length * sizeof(uint8_t));
    assert(ret_val.quotient.digits != nullptr);
    ret_val.quotient.length = 0;    

    BigInt R;
    R.length = 0; 
    R.digits = malloc((divisor->length + 1) * sizeof(uint8_t));
    assert(R.digits != nullptr);


    for(size_t i = 0; i < dividend->length; i++)
    {
        append_digit(&R, dividend->digits[i]);

        uint8_t q = 0;
        while(compare(&R, divisor) >= 0)
        {
            subtract(&R, divisor);
            q++;
        }
        if(q != 0 || ret_val.quotient.length > 0) //won't write zero at first index
        {
            ret_val.quotient.digits[ret_val.quotient.length++] = q;
        }
    }

    // quotient is zero (example 6/7)
    if (ret_val.quotient.length == 0)
    {
        ret_val.quotient.digits[0] = 0;
        ret_val.quotient.length = 1;
    }
    // negative is false if it's 0
    if (ret_val.quotient.length == 1 && ret_val.quotient.digits[0] == 0)
    {
        ret_val.quotient.negative = false;
    }

    uint8_t *tmp = realloc(ret_val.quotient.digits, ret_val.quotient.length * sizeof(uint8_t));
    if (tmp != nullptr) ret_val.quotient.digits = tmp;


    ret_val.remainder = R;
    bool rem_zero = (R.length == 1 && R.digits[0] == 0);
    ret_val.remainder.negative = rem_zero ? false : dividend->negative;

    tmp = realloc(ret_val.remainder.digits, ret_val.remainder.length * sizeof(uint8_t));
    if (tmp != nullptr) ret_val.remainder.digits = tmp;

    return ret_val;
}

BigInt get_big_number(void)
{
    BigInt ret_val;

    size_t capacity = CAPACITY_START;
    ret_val.digits = malloc(capacity * sizeof(uint8_t));
    assert(ret_val.digits != nullptr);
    ret_val.length = 0;
    ret_val.negative = false; //by default it's a positive number

    printf("Enter the number: ");
    fflush(stdout);
    
    int c; //int because return value of func getchar() is int
    c = getchar();

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
    if (tmp != nullptr) ret_val.digits = tmp; 
    
    return ret_val;
}

void free_big_int(BigInt* big_int)
{
    free(big_int->digits);
    big_int->digits = nullptr;
    big_int->length = 0;
}

void print_big_int(const BigInt* big_int)
{

    printf("Number: %s", big_int->negative ? "-" : "");
    for(size_t i = 0; i < big_int->length; i++)
    {
        printf("%hhu",big_int->digits[i]);
    }
    printf("\nLength: %ld\n", big_int->length);
    printf("Negative: %s\n", big_int->negative ? "true" : "false");

}

static void append_digit(BigInt *big_int, uint8_t digit)
{
    if (big_int->length == 1 && big_int->digits[0] == 0) 
    {
        big_int->digits[0] = digit; 
    }
    else 
    {
        big_int->digits[big_int->length++] = digit;
    }
}

static int_fast8_t compare(const BigInt* first_op, const BigInt* second_op)
{
    if      (first_op->length > second_op->length) return CMP_GREATER; 
    else if (first_op->length < second_op->length) return CMP_LESS;

    for(size_t i = 0; i < first_op->length; i++)
    {
        if      (first_op->digits[i] > second_op->digits[i]) return CMP_GREATER;
        else if (first_op->digits[i] < second_op->digits[i]) return CMP_LESS;
    }
    return CMP_EQUAL;
}

static void subtract(BigInt* first_op, const BigInt* second_op)
{
    assert(first_op->length > 0);
    assert(second_op->length > 0);

    bool borrow;
    size_t i;
    size_t j;
    
    for (size_t k = 0; k < second_op->length; k++)
    {
        i = second_op->length - 1 - k;
        j = first_op->length  - 1 - k;

        if ((int)first_op->digits[j] - (int)second_op->digits[i] < 0) 
        {
            borrow = true;
            first_op->digits[j] += BASE;
            first_op->digits[j] -= (int)second_op->digits[i];
        }
        else 
        {
            borrow = false;
            first_op->digits[j] -= (int)second_op->digits[i];
        }

        if(borrow) //borrow has to go recursively if the digit at borrowing index is zero
        {
            size_t borrowing_index = j -1;
            while(first_op->digits[borrowing_index] == 0 && borrowing_index != 0)
            {
                first_op->digits[borrowing_index] = BASE -1;
                borrowing_index--;
            }   

            assert(first_op->digits[borrowing_index] != 0); //nowhere to borrow from, this assert shouldn't trigger
            first_op->digits[borrowing_index]--;
        }
    }

    
    // (divisor = 22 (example)) remainder: 078, 02 
    size_t leading_zeros = 0;
    while(leading_zeros < first_op->length -1  &&  first_op->digits[leading_zeros] == 0)
    {
        leading_zeros++;
    }

    if (leading_zeros)
    {
        memmove(first_op->digits, first_op->digits + leading_zeros, first_op->length - leading_zeros);
        first_op->length -= leading_zeros;
    }
}