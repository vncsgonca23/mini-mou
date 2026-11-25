#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex05"

if [ ! -d "$EX_DIR" ] || [ ! -f "$EX_DIR/Makefile" ]; then
    fail_test "ex05/Makefile not found"
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT
cp -R "$EX_DIR"/. "$TMP_DIR/src/"
BUILD="$TMP_DIR/src"

if (cd "$BUILD" && make clean >/dev/null 2>&1; make >/dev/null 2>&1) && [ -x "$BUILD/do-op" ]; then
    pass_test "'make clean && make' built do-op"
else
    fail_test "'make clean && make' failed to build do-op"
    finish_shell_tests
fi

if command -v nm &> /dev/null; then
    bad_syms="$(nm -u "$BUILD/do-op" 2>/dev/null | awk '{print $NF}' | sed 's/@.*//' \
        | grep -vE '^(_|write$)' || true)"
    if [ -n "$bad_syms" ]; then
        fail_test "Forbidden function(s) used in do-op: $(printf '%s' "$bad_syms" | tr '\n' ' ') (allowed: write)"
    else
        pass_test "Only authorized function (write) used"
    fi
fi

BIN="$BUILD/do-op"

run_op() {
    local desc="$1"
    local expected="$2"
    shift 2
    printf '%s' "$expected" > "$TMP_DIR/exp"
    "$BIN" "$@" > "$TMP_DIR/act" 2>/dev/null
    assert_files_equal "$desc" "$TMP_DIR/exp" "$TMP_DIR/act"
}

# Subject examples & edge cases
run_op "No arguments -> empty output" ""
run_op "Too few arguments (2 args) -> empty output" "" "1" "+"
run_op "Too many arguments (4 args) -> empty output" "" "1" "+" "1" "extra"
run_op "./do-op 1 + 1 -> 2" $'2\n' "1" "+" "1"
run_op "./do-op 42 + 21 -> 63" $'63\n' "42" "+" "21"
run_op "./do-op 42amis - --+-20toto12 -> 62" $'62\n' "42amis" "-" "--+-20toto12"
run_op "./do-op 1 p 1 (invalid op) -> 0" $'0\n' "1" "p" "1"
run_op "./do-op 1 + toto3 -> 1" $'1\n' "1" "+" "toto3"
run_op "./do-op toto3 + 4 -> 4" $'4\n' "toto3" "+" "4"
run_op "./do-op foo plus bar -> 0" $'0\n' "foo" "plus" "bar"
run_op "./do-op 25 / 0 -> Stop : division by zero" $'Stop : division by zero\n' "25" "/" "0"
run_op "./do-op 25 % 0 -> Stop : modulo by zero" $'Stop : modulo by zero\n' "25" "%" "0"
run_op "./do-op 6 '*' 7 -> 42" $'42\n' "6" "*" "7"
run_op "./do-op -42 / 5 -> -8" $'-8\n' "-42" "/" "5"
run_op "./do-op -42 % 5 -> -2" $'-2\n' "-42" "%" "5"
run_op "./do-op 10 ++ 5 (multi-char invalid op) -> 0" $'0\n' "10" "++" "5"
run_op "./do-op 0 - 2147483648 (INT_MIN) -> -2147483648" $'-2147483648\n' "-2147483648" "+" "0"

finish_shell_tests
