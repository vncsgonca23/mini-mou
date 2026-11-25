#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex01"

if [ ! -d "$EX_DIR" ] || [ ! -f "$EX_DIR/Makefile" ]; then
    fail_test "ex01/Makefile not found"
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT
cp -R "$EX_DIR"/. "$TMP_DIR/src/"
BUILD="$TMP_DIR/src"

if (cd "$BUILD" && make fclean >/dev/null 2>&1; make all >/dev/null 2>&1) && [ -x "$BUILD/ft_cat" ]; then
    pass_test "'make all' built ft_cat"
else
    fail_test "'make all' failed to build ft_cat"
    finish_shell_tests
fi

if command -v nm &> /dev/null; then
    bad_syms="$(nm -u "$BUILD/ft_cat" 2>/dev/null | awk '{print $NF}' | sed 's/@.*//' \
        | grep -vE '^(_|close$|open$|read$|write$|strerror$|basename$|errno$)' || true)"
    if [ -n "$bad_syms" ]; then
        fail_test "Forbidden function(s) used: $(printf '%s' "$bad_syms" | tr '\n' ' ') (allowed: close open read write strerror basename)"
    else
        pass_test "Only authorized functions used (fixed buffer, no malloc)"
    fi
fi

BIN="$BUILD/ft_cat"

printf 'First file\nLine 2\n' > "$TMP_DIR/f1"
printf 'Second file without newline' > "$TMP_DIR/f2"
python3 -c 'import sys; sys.stdout.buffer.write(b"0123456789ABCDEF\x00\n" * 4000)' > "$TMP_DIR/f_big"

# 1. Single file
cat "$TMP_DIR/f1" > "$TMP_DIR/exp1"
"$BIN" "$TMP_DIR/f1" > "$TMP_DIR/act1" 2>/dev/null
assert_files_equal "Single file output matches cat" "$TMP_DIR/exp1" "$TMP_DIR/act1"

# 2. Multiple files
cat "$TMP_DIR/f1" "$TMP_DIR/f2" > "$TMP_DIR/exp2"
"$BIN" "$TMP_DIR/f1" "$TMP_DIR/f2" > "$TMP_DIR/act2" 2>/dev/null
assert_files_equal "Multiple files concatenated in order" "$TMP_DIR/exp2" "$TMP_DIR/act2"

# 3. Large binary file (> 64KB, tests < 30KB buffer chunking)
cat "$TMP_DIR/f_big" > "$TMP_DIR/exp3"
"$BIN" "$TMP_DIR/f_big" > "$TMP_DIR/act3" 2>/dev/null
assert_files_equal "Large binary file (72KB) matches cat" "$TMP_DIR/exp3" "$TMP_DIR/act3"

# 4. Stdin with no arguments
printf 'hello from stdin\nline 2\n' > "$TMP_DIR/in_stdin"
"$BIN" < "$TMP_DIR/in_stdin" > "$TMP_DIR/act4" 2>/dev/null
assert_files_equal "No arguments reads from stdin" "$TMP_DIR/in_stdin" "$TMP_DIR/act4"

# 5. '-' argument reads stdin between files
cat "$TMP_DIR/f1" "$TMP_DIR/in_stdin" "$TMP_DIR/f2" > "$TMP_DIR/exp5"
"$BIN" "$TMP_DIR/f1" - "$TMP_DIR/f2" < "$TMP_DIR/in_stdin" > "$TMP_DIR/act5" 2>/dev/null
assert_files_equal "'-' argument interleaves stdin between files" "$TMP_DIR/exp5" "$TMP_DIR/act5"

# 6. Missing file writes error to stderr and continues with next valid file
"$BIN" "$TMP_DIR/missing_xyz" "$TMP_DIR/f1" > "$TMP_DIR/act6" 2> "$TMP_DIR/err6"
if cmp -s "$TMP_DIR/f1" "$TMP_DIR/act6" && [ -s "$TMP_DIR/err6" ]; then
    pass_test "Missing file prints error on stderr and continues to next file"
else
    fail_test "Missing file handling failed (stdout or stderr mismatch)"
fi

if (cd "$BUILD" && make clean >/dev/null 2>&1 && make fclean >/dev/null 2>&1 && [ ! -e "$BUILD/ft_cat" ]); then
    pass_test "Makefile 'clean' and 'fclean' rules work"
else
    fail_test "Makefile 'clean'/'fclean' failed"
fi

finish_shell_tests
