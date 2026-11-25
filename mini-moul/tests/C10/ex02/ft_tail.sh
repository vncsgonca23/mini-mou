#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex02"

if [ ! -d "$EX_DIR" ] || [ ! -f "$EX_DIR/Makefile" ]; then
    fail_test "ex02/Makefile not found"
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT
cp -R "$EX_DIR"/. "$TMP_DIR/src/"
BUILD="$TMP_DIR/src"

if (cd "$BUILD" && make fclean >/dev/null 2>&1; make all >/dev/null 2>&1) && [ -x "$BUILD/ft_tail" ]; then
    pass_test "'make all' built ft_tail"
else
    fail_test "'make all' failed to build ft_tail"
    finish_shell_tests
fi

if command -v nm &> /dev/null; then
    bad_syms="$(nm -u "$BUILD/ft_tail" 2>/dev/null | awk '{print $NF}' | sed 's/@.*//' \
        | grep -vE '^(_|close$|open$|read$|write$|malloc$|free$|strerror$|basename$|errno$)' || true)"
    if [ -n "$bad_syms" ]; then
        fail_test "Forbidden function(s) used: $(printf '%s' "$bad_syms" | tr '\n' ' ')"
    else
        pass_test "Only authorized functions used"
    fi
fi

BIN="$BUILD/ft_tail"

printf '0123456789abcdefghij\n' > "$TMP_DIR/f1"
printf 'Hello 42 World!\n' > "$TMP_DIR/f2"
python3 -c 'import sys; sys.stdout.buffer.write(bytes([i % 256 for i in range(20000)]))' > "$TMP_DIR/f_big"

# 1. -c 5 file (separate arg)
tail -c 5 "$TMP_DIR/f1" > "$TMP_DIR/exp1"
"$BIN" -c 5 "$TMP_DIR/f1" > "$TMP_DIR/act1" 2>/dev/null
assert_files_equal "ft_tail -c 5 file matches system tail" "$TMP_DIR/exp1" "$TMP_DIR/act1"

# 2. -c5 file (attached arg)
tail -c5 "$TMP_DIR/f1" > "$TMP_DIR/exp2"
"$BIN" -c5 "$TMP_DIR/f1" > "$TMP_DIR/act2" 2>/dev/null
assert_files_equal "ft_tail -c5 file (attached option) matches system tail" "$TMP_DIR/exp2" "$TMP_DIR/act2"

# 3. -c 0 file (empty output)
tail -c 0 "$TMP_DIR/f1" > "$TMP_DIR/exp3"
"$BIN" -c 0 "$TMP_DIR/f1" > "$TMP_DIR/act3" 2>/dev/null
assert_files_equal "ft_tail -c 0 outputs 0 bytes" "$TMP_DIR/exp3" "$TMP_DIR/act3"

# 4. -c larger than file size
tail -c 100 "$TMP_DIR/f1" > "$TMP_DIR/exp4"
"$BIN" -c 100 "$TMP_DIR/f1" > "$TMP_DIR/act4" 2>/dev/null
assert_files_equal "ft_tail -c N > file size outputs entire file" "$TMP_DIR/exp4" "$TMP_DIR/act4"

# 5. Large binary file with -c 5000
tail -c 5000 "$TMP_DIR/f_big" > "$TMP_DIR/exp5"
"$BIN" -c 5000 "$TMP_DIR/f_big" > "$TMP_DIR/act5" 2>/dev/null
assert_files_equal "ft_tail -c 5000 on 20KB binary file matches system tail" "$TMP_DIR/exp5" "$TMP_DIR/act5"

# 6. Multiple files (with headers ==> name <==)
tail -c 6 "$TMP_DIR/f1" "$TMP_DIR/f2" > "$TMP_DIR/exp6"
"$BIN" -c 6 "$TMP_DIR/f1" "$TMP_DIR/f2" > "$TMP_DIR/act6" 2>/dev/null
assert_files_equal "ft_tail -c 6 on multiple files matches headers & content" "$TMP_DIR/exp6" "$TMP_DIR/act6"

# 7. Stdin with -c 7
printf 'abcdefghijklmnopqrstuvwxyz' > "$TMP_DIR/in_stdin"
tail -c 7 < "$TMP_DIR/in_stdin" > "$TMP_DIR/exp7"
"$BIN" -c 7 < "$TMP_DIR/in_stdin" > "$TMP_DIR/act7" 2>/dev/null
assert_files_equal "ft_tail -c 7 from stdin matches system tail" "$TMP_DIR/exp7" "$TMP_DIR/act7"

# 8. Missing file writes stderr error and continues to valid file
tail -c 5 "$TMP_DIR/nonexistent_file" "$TMP_DIR/f2" > "$TMP_DIR/exp8" 2>/dev/null || true
"$BIN" -c 5 "$TMP_DIR/nonexistent_file" "$TMP_DIR/f2" > "$TMP_DIR/act8" 2> "$TMP_DIR/err8"
if [ -s "$TMP_DIR/err8" ] && cmp -s "$TMP_DIR/exp8" "$TMP_DIR/act8"; then
    pass_test "Missing file reports error on stderr and processes remaining files"
else
    fail_test "Missing file handling failed"
fi

if (cd "$BUILD" && make clean >/dev/null 2>&1 && make fclean >/dev/null 2>&1 && [ ! -e "$BUILD/ft_tail" ]); then
    pass_test "Makefile 'clean' and 'fclean' rules work"
else
    fail_test "Makefile 'clean'/'fclean' failed"
fi

finish_shell_tests
