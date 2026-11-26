#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex00"

if [ ! -d "$EX_DIR" ] || [ ! -f "$EX_DIR/Makefile" ]; then
    fail_test "ex00/Makefile not found"
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT
cp -R "$EX_DIR"/. "$TMP_DIR/src/"
BUILD="$TMP_DIR/src"

# Build & Makefile rules check
if (cd "$BUILD" && make fclean >/dev/null 2>&1; make all >/dev/null 2>&1) && [ -x "$BUILD/ft_display_file" ]; then
    pass_test "'make all' built ft_display_file"
else
    fail_test "'make all' failed to build ft_display_file"
    finish_shell_tests
fi

# Check forbidden functions (allowed: close, open, read, write; malloc is explicitly forbidden!)
if command -v nm &> /dev/null; then
    bad_syms="$(nm -u "$BUILD/ft_display_file" 2>/dev/null | awk '{print $NF}' | sed 's/@.*//' \
        | grep -vE '^(_|close$|open$|read$|write$)' || true)"
    if [ -n "$bad_syms" ]; then
        fail_test "Forbidden function(s) used: $(printf '%s' "$bad_syms" | tr '\n' ' ') (allowed: close open read write)"
    else
        pass_test "Only authorized functions used (no malloc)"
    fi
fi

BIN="$BUILD/ft_display_file"

# Error cases on stderr (standard Moulinette messages)
: > "$TMP_DIR/empty"
printf 'File name missing.\n' > "$TMP_DIR/exp_noarg"
"$BIN" > "$TMP_DIR/out1" 2> "$TMP_DIR/err1"
if cmp -s "$TMP_DIR/empty" "$TMP_DIR/out1" && cmp -s "$TMP_DIR/exp_noarg" "$TMP_DIR/err1"; then
    pass_test "No arguments -> 'File name missing.' on stderr (empty stdout)"
else
    fail_test "No arguments mismatch (stdout='$(cat -e "$TMP_DIR/out1")', stderr='$(cat -e "$TMP_DIR/err1")')"
fi

printf 'Too many arguments.\n' > "$TMP_DIR/exp_many"
"$BIN" a b > "$TMP_DIR/out2" 2> "$TMP_DIR/err2"
if cmp -s "$TMP_DIR/empty" "$TMP_DIR/out2" && cmp -s "$TMP_DIR/exp_many" "$TMP_DIR/err2"; then
    pass_test "Too many arguments -> 'Too many arguments.' on stderr"
else
    fail_test "Too many arguments mismatch (stdout='$(cat -e "$TMP_DIR/out2")', stderr='$(cat -e "$TMP_DIR/err2")')"
fi

printf 'Cannot read file.\n' > "$TMP_DIR/exp_cant"
"$BIN" "$TMP_DIR/does_not_exist" > "$TMP_DIR/out3" 2> "$TMP_DIR/err3"
if cmp -s "$TMP_DIR/empty" "$TMP_DIR/out3" && cmp -s "$TMP_DIR/exp_cant" "$TMP_DIR/err3"; then
    pass_test "Non-existent file -> 'Cannot read file.' on stderr"
else
    fail_test "Non-existent file mismatch (stdout='$(cat -e "$TMP_DIR/out3")', stderr='$(cat -e "$TMP_DIR/err3")')"
fi

# Valid files: empty, text, binary with NULs, large (> 64KB)
: > "$TMP_DIR/f_empty"
"$BIN" "$TMP_DIR/f_empty" > "$TMP_DIR/out_empty" 2>/dev/null
assert_files_equal "Empty file outputs 0 bytes" "$TMP_DIR/f_empty" "$TMP_DIR/out_empty"

printf 'Hello 42!\nLine 2\twith tabs\nNo trailing newline' > "$TMP_DIR/f_text"
"$BIN" "$TMP_DIR/f_text" > "$TMP_DIR/out_text" 2>/dev/null
assert_files_equal "Standard text file matches byte-for-byte" "$TMP_DIR/f_text" "$TMP_DIR/out_text"

python3 -c 'import sys; sys.stdout.buffer.write(b"ABC\x00DEF\xff\n" * 8000)' > "$TMP_DIR/f_large"
"$BIN" "$TMP_DIR/f_large" > "$TMP_DIR/out_large" 2>/dev/null
assert_files_equal "Large binary file (72KB with NUL bytes) matches byte-for-byte" "$TMP_DIR/f_large" "$TMP_DIR/out_large"

# Makefile clean & fclean
if (cd "$BUILD" && make clean >/dev/null 2>&1 && [ -x "$BUILD/ft_display_file" ] \
    && make fclean >/dev/null 2>&1 && [ ! -e "$BUILD/ft_display_file" ]); then
    pass_test "Makefile 'clean' and 'fclean' rules work"
else
    fail_test "Makefile 'clean'/'fclean' failed"
fi

finish_shell_tests
