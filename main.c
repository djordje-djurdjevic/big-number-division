//for testing: 0, -0, +0, 5, -5, 123456789012345, 066, -, empty Enter, 12a3,
//12 − 5, 100 − 1, 123 − 45, 1000 − 999, 5 − 5, 10 − 10.
//Testovi: 1234/56 (22, 2), 5/7 (0, 5), 100/10 (10, 0), 0/5, 7/1, -7/2, 7/-2, 999999/999999, delilac 0 (assert).
#include <stdlib.h>

#include "bigint.h"


int main(void)
{
    /*
	Djordje Djurdjevic ra127/2023

	ZADATAK
	Deljenje dva proizvoljno velika cela broja zadata nizom cifara (kolicnik i ostatak).

	RESENJE
	struct BigInt: niz cifara (uint8_t, jedna decimalna cifra po elementu, najznacajnija prva),
	duzina i znak (znak + apsolutna vrednost). Deljenje je pisano (schoolbook long division)
	nad apsolutnim vrednostima, a cifra kolicnika se odredjuje ponovljenim oduzimanjem
	delioca.
	Moduli: main.c, bigint.c, bigint.h

	PREVODJENJE
	make main   ili   gcc -std=c2x -Wall -Wextra -Wpedantic main.c bigint.c -o main
	Bez -DNDEBUG: assert se koristi za prekid programa pri neispravnom unosu.

	PRETPOSTAVKE I KONVENCIJE
	- Svaki broj je u jednom redu, bez razmaka, opciono sa + ili -.
	- Vodece nule su greska (066), 0, +0 i -0 su ispravni, a nula nikad nije negativna.
	- Deljenje nulom: big_int_div vraca quotient.digits == nullptr, main ispisuje poruku.

	OGRANICENJA
	Duzina broja je ogranicena samo memorijom (size_t). Prekoracenje (size_t length) pri udvostrucavanju
	kapaciteta se ne proverava (skoro nemoguce za desiti).

	C23
	Koriste se nullptr, bool i constexpr kao kljucna rec.

	MISRA C
	Odstupanja (neizbezna ili svesna):
	- 21.3: malloc/realloc/free (broj cifara nije unapred poznat).
	- 21.6: printf, getchar, ungetc, fflush (ulaz/izlaz).
	- 15.5: vise return naredbi (compare, rani izlaz u big_int_div).
	- assert za neispravan unos nije MISRA obrada gresaka (svesna odluka).

	TESTIRANJE
    Unos: 0, -0, +0, 066, samo znak, prazan red, 12a3, broj duzi od pocetnog kapaciteta. 
    Deljenje: 1234/56, 5/7, -5/7, 100/10,
    0/5, 7/1, -7/2, 7/-2, -6/3, 999999/999999, delilac 0. Veliki brojevi uporedjeni sa online kalkulatorima
    testirano i curenje memorije sa valgrind
    */

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
