#!/bin/bash
# Test script for the BigInt division program.
# Usage: ./test.sh 


BIN=${BIN:-./main}

# ---------------------------------------------------------------------------
# Colors
# ---------------------------------------------------------------------------
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BOLD='\033[1m'
NC='\033[0m' # No Color
 

total=0
failed=0

# Runs the program and prints "<quotient> <remainder>" with signs.
run_division()
{
	printf '%s\n%s\n' "$1" "$2" | "$BIN" 2>/dev/null | awk '
		/Quotient/   { s = "q" }
		/Remainder/  { s = "r" }
		/^Number:/   { sub(/^-/, "", $2); n[s] = $2 }
		/^Negative:/ { g[s] = ($2 == "true") ? "-" : "" }
		END          { print g["q"] n["q"], g["r"] n["r"] }'
}


# check <dividend> <divisor> <expected quotient> <expected remainder>
check()
{
	local a=$1
	local b=$2
	local expected="$3 $4"
	local got

	got=$(run_division "$a" "$b")
	((total++))

	if [ "$got" == "$expected" ]
	then
		echo -e "${GREEN}PASS${NC}  $a / $b"
	else
		echo -e "${RED}FAIL${NC}  $a / $b"
		echo -e "      expected: ${YELLOW}${expected}${NC}"
		echo -e "      got:      ${YELLOW}${got}${NC}"
		((failed++))
	fi
}

# check_fails <dividend> <divisor>: the program must exit with a non-zero code
check_fails()
{
	local a=$1
	local b=$2
	local code

	{ printf '%s\n%s\n' "$a" "$b" | "$BIN" >/dev/null 2>&1; } 2>/dev/null
	code=$?
	((total++))

	if [ "$code" -ne 0 ]
	then
		echo -e "${GREEN}PASS${NC}  rejected: '$a' / '$b' (exit code $code)"
	else
		echo -e "${RED}FAIL${NC}  accepted invalid input: '$a' / '$b'"
		((failed++))
	fi
}


# --- valid divisions: dividend divisor quotient remainder ---
check 1234 56 22 2
check 5 7 0 5
check -5 7 0 -5
check 100 10 10 0
check 0 5 0 0
check 7 1 7 0
check -7 2 -3 -1
check 7 -2 -3 1
check -6 3 -2 0
check 999999 999999 1 0
check +8 +2 4 0
check -0 5 0 0
check 1425012795210215799705217901257905 597120521799725907190 2386474326682 210412491811028614325
check 75920572901790257902197215790129804219080972171597090723790367695326985326985639868953285692966954269886926859469255682395683268539269575692532968888888888888832569853692865983266666665632986235986247546932698568256893589362233 48912491804980218940894102 1552171441285273147560096313219960559849845453417224874732841767271241571108220399219244398419094854394813714087662829686800955260387320503389919216543231906349372931221829019967075351544449829550634716 16812931385111290948517201

# --- invalid input and division by zero ---
check_fails 066 5
check_fails - 5
check_fails "" 5
check_fails 12a3 5
check_fails 5 0
check_fails 5 -0
check_fails 0000 0000

# --- summary ---
if [ "$failed" -gt 0 ]
then
	echo -e "${RED}${BOLD}Tests passed: $((total - failed))/${total}${NC}"
	exit 1
fi

echo -e "${GREEN}${BOLD}ALL TESTS PASSED (${total}/${total})${NC}"
exit 0