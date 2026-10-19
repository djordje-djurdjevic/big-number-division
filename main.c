/*
program should be compiled without -DNDEBUG.
assert is used for errors to end program execution.
*/
//try all cases/asserts
//for testing: 0, -0, +0, 5, -5, 123456789012345, 066, -, empty Enter, 12a3,
//12 − 5, 100 − 1, 123 − 45, 1000 − 999, 5 − 5, 10 − 10.
//Testovi: 1234/56 (22, 2), 5/7 (0, 5), 100/10 (10, 0), 0/5, 7/1, -7/2, 7/-2, 999999/999999, delilac 0 (assert).
#include <stdlib.h>

#include "bigint.h"


int main(void)
{
    BigInt first_op  = get_big_number();
    BigInt second_op = get_big_number();

    // print_big_int(&first_op);
    // print_big_int(&second_op);

    DivResult div_result = big_int_div(&first_op, &second_op);
    if (div_result.quotient.digits == nullptr) //digits == nullptr is an error signal
    {
        fprintf(stderr, "Error: division by zero.\n");
        free_big_int(&first_op);
        free_big_int(&second_op);
        return EXIT_FAILURE;
    }

    printf("\n");
    printf("============ RESULT ============\n");
    printf("----------- Quotient -----------\n");
    print_big_int(&div_result.quotient);
    printf("----------- Remainder -----------\n");
    print_big_int(&div_result.remainder);

    /*
    tested both helper functions
    subtract(&first_op, &second_op);
    printf("%d",compare(&first_op, &second_op));
    */

    free_big_int(&first_op);
    free_big_int(&second_op);
    free_big_int(&div_result.quotient);
    free_big_int(&div_result.remainder);

    return 0;
}
