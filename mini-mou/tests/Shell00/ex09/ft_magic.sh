#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex09"
TARGET="$EX_DIR/ft_magic"

if ! check_delivery_file "$EX_DIR" "ft_magic"; then
    finish_shell_tests
fi

TARGET_ABS="$(cd "$EX_DIR" && pwd)/ft_magic"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Validate syntax by compiling magic file inside temp dir (never polluting ex09/)
cp "$TARGET_ABS" "$TMP_DIR/ft_magic"
if (cd "$TMP_DIR" && file -C -m ft_magic >/dev/null 2>&1); then
    pass_test "ft_magic compiles cleanly with 'file -C -m'"
else
    fail_test "ft_magic failed 'file -C -m' validation"
    finish_shell_tests
fi

# Case 1: Exactly "42" at the 42nd byte (0-based offset 41) with ASCII prefix
printf '%041d42rest\n' 0 > "$TMP_DIR/valid_ascii"
out1="$(file -b -m "$TARGET_ABS" "$TMP_DIR/valid_ascii" 2>/dev/null)"
if [ "$out1" = "42 file" ]; then
    pass_test "Detects '42 file' with \"42\" at 42nd byte (offset 41, ASCII prefix)"
else
    fail_test "Offset 41 (ASCII) Expected \"42 file\", got \"$out1\""
fi

# Case 2: Exactly "42" at the 42nd byte (offset 41) with NUL binary prefix
python3 -c 'import sys; sys.stdout.buffer.write(b"\x00"*41 + b"42")' > "$TMP_DIR/valid_bin"
out2="$(file -b -m "$TARGET_ABS" "$TMP_DIR/valid_bin" 2>/dev/null)"
if [ "$out2" = "42 file" ]; then
    pass_test "Detects '42 file' with \"42\" at 42nd byte (offset 41, binary NUL prefix)"
else
    fail_test "Offset 41 (binary) Expected \"42 file\", got \"$out2\""
fi

# Case 3: "42" at offset 42 (43rd byte) -> must NOT match
printf '%042d42rest\n' 0 > "$TMP_DIR/off42"
out3="$(file -b -m "$TARGET_ABS" "$TMP_DIR/off42" 2>/dev/null)"
if [ "$out3" != "42 file" ]; then
    pass_test "Does NOT match \"42\" at 43rd byte (offset 42)"
else
    fail_test "Incorrectly matched offset 42 (43rd byte) as '42 file' (42nd byte is offset 41)"
fi

# Case 4: "42" at offset 40 (41st byte) -> must NOT match
printf '%040d42rest\n' 0 > "$TMP_DIR/off40"
out4="$(file -b -m "$TARGET_ABS" "$TMP_DIR/off40" 2>/dev/null)"
if [ "$out4" != "42 file" ]; then
    pass_test "Does NOT match \"42\" at 41st byte (offset 40)"
else
    fail_test "Incorrectly matched offset 40 (41st byte) as '42 file'"
fi

# Case 5: "42" at offset 0 -> must NOT match
printf '42hello world\n' > "$TMP_DIR/off0"
out5="$(file -b -m "$TARGET_ABS" "$TMP_DIR/off0" 2>/dev/null)"
if [ "$out5" != "42 file" ]; then
    pass_test "Does NOT match \"42\" at start of file (offset 0)"
else
    fail_test "Incorrectly matched offset 0 as '42 file'"
fi

finish_shell_tests
