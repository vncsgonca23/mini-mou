#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex03"

if [ ! -d "$EX_DIR" ] || [ ! -f "$EX_DIR/Makefile" ]; then
    fail_test "ex03/Makefile not found"
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT
cp -R "$EX_DIR"/. "$TMP_DIR/src/"
BUILD="$TMP_DIR/src"

if (cd "$BUILD" && make fclean >/dev/null 2>&1; make all >/dev/null 2>&1) && [ -x "$BUILD/ft_hexdump" ]; then
    pass_test "'make all' built ft_hexdump"
else
    fail_test "'make all' failed to build ft_hexdump"
    finish_shell_tests
fi

if command -v nm &> /dev/null; then
    bad_syms="$(nm -u "$BUILD/ft_hexdump" 2>/dev/null | awk '{print $NF}' | sed 's/@.*//' \
        | grep -vE '^(_|close$|open$|read$|write$|malloc$|free$|strerror$|basename$|errno$)' || true)"
    if [ -n "$bad_syms" ]; then
        fail_test "Forbidden function(s) used: $(printf '%s' "$bad_syms" | tr '\n' ' ')"
    else
        pass_test "Only authorized functions used"
    fi
fi

BIN="$BUILD/ft_hexdump"

# Prepare test files
: > "$TMP_DIR/empty"
printf 'Hello, 42!' > "$TMP_DIR/short"
printf '0123456789ABCDEF' > "$TMP_DIR/exact16"
python3 -c 'import sys; sys.stdout.buffer.write(b"A"*16 + b"A"*16 + b"A"*16 + b"B\x00\x7f\x80\xff\n")' > "$TMP_DIR/dup_and_bin"

# 1. Empty file
hexdump -C "$TMP_DIR/empty" > "$TMP_DIR/exp1"
"$BIN" -C "$TMP_DIR/empty" > "$TMP_DIR/act1" 2>/dev/null
assert_files_equal "ft_hexdump -C on empty file outputs nothing" "$TMP_DIR/exp1" "$TMP_DIR/act1"

# 2. Short file (< 16 bytes)
hexdump -C "$TMP_DIR/short" > "$TMP_DIR/exp2"
"$BIN" -C "$TMP_DIR/short" > "$TMP_DIR/act2" 2>/dev/null
assert_files_equal "ft_hexdump -C on short file (< 16 bytes) matches hexdump -C" "$TMP_DIR/exp2" "$TMP_DIR/act2"

# 3. Exact 16 bytes
hexdump -C "$TMP_DIR/exact16" > "$TMP_DIR/exp3"
"$BIN" -C "$TMP_DIR/exact16" > "$TMP_DIR/act3" 2>/dev/null
assert_files_equal "ft_hexdump -C on exact 16 bytes matches hexdump -C" "$TMP_DIR/exp3" "$TMP_DIR/act3"

# 4. Duplicate 16-byte lines ('*' collapsing) & binary/non-printable bytes
hexdump -C "$TMP_DIR/dup_and_bin" > "$TMP_DIR/exp4"
"$BIN" -C "$TMP_DIR/dup_and_bin" > "$TMP_DIR/act4" 2>/dev/null
assert_files_equal "ft_hexdump -C collapses duplicate lines ('*') & handles binary bytes" "$TMP_DIR/exp4" "$TMP_DIR/act4"

# 5. Multiple files as continuous stream
hexdump -C "$TMP_DIR/short" "$TMP_DIR/exact16" > "$TMP_DIR/exp5"
"$BIN" -C "$TMP_DIR/short" "$TMP_DIR/exact16" > "$TMP_DIR/act5" 2>/dev/null
assert_files_equal "ft_hexdump -C across multiple files matches hexdump -C" "$TMP_DIR/exp5" "$TMP_DIR/act5"

# 6. Stdin with -C
hexdump -C < "$TMP_DIR/dup_and_bin" > "$TMP_DIR/exp6"
"$BIN" -C < "$TMP_DIR/dup_and_bin" > "$TMP_DIR/act6" 2>/dev/null
assert_files_equal "ft_hexdump -C from stdin matches hexdump -C" "$TMP_DIR/exp6" "$TMP_DIR/act6"

if (cd "$BUILD" && make clean >/dev/null 2>&1 && make fclean >/dev/null 2>&1 && [ ! -e "$BUILD/ft_hexdump" ]); then
    pass_test "Makefile 'clean' and 'fclean' rules work"
else
    fail_test "Makefile 'clean'/'fclean' failed"
fi

finish_shell_tests
