# BigInt Division

A C23 program that divides two arbitrarily large integers, entered as strings of decimal digits, and prints the quotient and the remainder. Written for the Advanced C Programming course.

## Features

- Operands of any length, limited only by available memory
- Signed numbers (`+` and `-`), with C-style truncating division
- No external libraries, standard C only
- Invalid input and division by zero are detected and reported
- Automated test script and a clean run under AddressSanitizer / Valgrind

## Project structure

| File | Purpose |
|---|---|
| `main.c` | Reads two numbers, calls the division, prints the result, frees memory |
| `bigint.c` | Implementation: input, output, division, and the `static` helpers `append_digit`, `subtract`, `compare` |
| `bigint.h` | `BigInt` and `DivResult` types and the public functions |
| `Makefile` | Build script |
| `test_script.sh` | Automated tests |

## Building

Requires a compiler with C23 support. GCC 13 accepts it as `-std=c2x`; GCC 14 and later also accept `-std=c23`.

```
make main
```

or manually:

```
gcc -std=c2x -Wall -Wextra -Wpedantic main.c bigint.c -o main
```

> Do **not** build with `-DNDEBUG`. The program uses `assert` to stop on invalid input, and disabling asserts would let bad input through silently.

## Usage

Run `./main` and type the dividend and the divisor, one number per line.

```
$ ./main
Enter the number: 1425012795210215799705217901257905
Enter the number: 597120521799725907190

============ RESULT ============
----------- Quotient -----------
Number: 2386474326682
Length: 13
Negative: false
----------- Remainder -----------
Number: 210412491811028614325
Length: 21
Negative: false
```

### Input rules

- One number per line, no spaces
- An optional leading `+` or `-`
- Digits `0`-`9` only
- Leading zeros are rejected (`066` is an error), but `0`, `+0` and `-0` are valid and all mean zero
- Invalid input (empty line, a sign with no digits, a non-digit character, leading zeros) stops the program with an `assert` failure
- Division by zero prints `Error: division by zero.` to `stderr` and exits with `EXIT_FAILURE`

### Sign convention

Division truncates toward zero, as in C:

- the quotient is negative when the operands have different signs
- the remainder has the sign of the dividend
- `a = q * b + r` and `|r| < |b|` always hold
- zero is never negative

| Dividend | Divisor | Quotient | Remainder |
|---|---|---|---|
| `-7` | `2` | `-3` | `-1` |
| `7` | `-2` | `-3` | `1` |
| `-5` | `7` | `0` | `-5` |
| `-6` | `3` | `-2` | `0` |

## How it works

A number is stored as a `BigInt`:

```c
typedef struct BigInt {
	uint8_t* digits;   // one decimal digit per element, most significant first
	size_t   length;
	bool     negative;
} BigInt;
```

The sign is kept separately from the magnitude, and the division runs on the absolute values.

The algorithm is schoolbook long division:

1. Take the dividend's digits from left to right.
2. Append each digit to a running remainder `R`, which is also a digit array.
3. Subtract the divisor from `R` while `R >= divisor`, counting the subtractions. This takes at most 9 steps and gives the next quotient digit.
4. After the last digit, `R` is the remainder.

The helpers are `compare`, `subtract` (digit by digit with borrow) and `append_digit`. Leading zeros are removed so that `compare` can decide by length first.

Complexity is O(n * m) for a dividend of `n` digits and a divisor of `m` digits.

## Design decisions

- **Memory over speed.** A digit takes one byte, there are no wider intermediate values, and arrays are shrunk to their exact size. The cost is slower division than a larger base (for example 10^9) with Knuth's Algorithm D would give. Base 10 was kept for simplicity and easy verification.
- **`assert` for invalid input.** This is a deliberate choice for a course project and is not user-friendly error handling.
- **Division by zero** is reported by returning a result whose `digits` pointer is `nullptr`, which `main` checks.

## Limitations

- The number length is limited by available memory and `size_t`.
- Capacity doubling during input is guarded against `size_t` overflow only if the corresponding `assert` is present.
- Each number must be on a single line.

## Testing

Run the test script from the project folder:

```
chmod +x test_script.sh
./test_script.sh
```

It covers:

- ordinary divisions and divisions with a zero quotient or remainder
- all sign combinations
- zero, `+0` and `-0`
- a large 34-digit by 21-digit division
- rejected input: leading zeros, a lone sign, an empty line, a non-digit character, and division by zero

The large case was checked against Python's `divmod`, and `q * b + r = a` was verified.

To run tests with Valgrind:

```
gcc -std=c2x -g main.c bigint.c -o main
printf '1234\n56\n' | valgrind --leak-check=full ./main
```

## Standards

The code targets C23 and was reviewed by hand against the basic MISRA C rules given for the course. Deviations (dynamic memory, standard I/O, multiple `return` statements, `assert` for input errors) are documented in the header comment of `main.c`. The project follows the course coding standard.