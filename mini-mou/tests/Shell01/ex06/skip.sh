#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex06"
TARGET="$EX_DIR/skip.sh"

if ! check_delivery_file "$EX_DIR" "skip.sh"; then
    finish_shell_tests
fi

check_executable "$TARGET"

TARGET_ABS="$(cd "$EX_DIR" && pwd)/skip.sh"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Case 1: 5 files (6 ls -l lines -> lines 1, 3, 5)
WORK1="$TMP_DIR/case5"
mkdir -p "$WORK1"
touch "$WORK1/a" "$WORK1/b" "$WORK1/c" "$WORK1/d" "$WORK1/e"
(cd "$WORK1" && LC_ALL=C ls -l | awk 'NR % 2 == 1') > "$TMP_DIR/exp1"
(cd "$WORK1" && LC_ALL=C /bin/sh "$TARGET_ABS") > "$TMP_DIR/act1_sh" 2>/dev/null
assert_files_equal "skip.sh via /bin/sh keeps odd lines (1, 3, 5) for 5 files" "$TMP_DIR/exp1" "$TMP_DIR/act1_sh"

if [ -x "$TARGET_ABS" ]; then
    (cd "$WORK1" && LC_ALL=C "$TARGET_ABS") > "$TMP_DIR/act1_exec" 2>/dev/null
    assert_files_equal "skip.sh via ./skip.sh matches expected output" "$TMP_DIR/exp1" "$TMP_DIR/act1_exec"
fi

# Case 2: 4 files (5 ls -l lines -> lines 1, 3, 5)
WORK2="$TMP_DIR/case4"
mkdir -p "$WORK2"
touch "$WORK2/alpha" "$WORK2/beta" "$WORK2/gamma" "$WORK2/delta"
(cd "$WORK2" && LC_ALL=C ls -l | awk 'NR % 2 == 1') > "$TMP_DIR/exp2"
(cd "$WORK2" && LC_ALL=C /bin/sh "$TARGET_ABS") > "$TMP_DIR/act2" 2>/dev/null
assert_files_equal "skip.sh keeps odd lines (1, 3, 5) for 4 files" "$TMP_DIR/exp2" "$TMP_DIR/act2"

# Case 3: Empty directory (1 line: total 0)
WORK3="$TMP_DIR/empty"
mkdir -p "$WORK3"
(cd "$WORK3" && LC_ALL=C ls -l | awk 'NR % 2 == 1') > "$TMP_DIR/exp3"
(cd "$WORK3" && LC_ALL=C /bin/sh "$TARGET_ABS") > "$TMP_DIR/act3" 2>/dev/null
assert_files_equal "skip.sh keeps first line on empty directory" "$TMP_DIR/exp3" "$TMP_DIR/act3"

finish_shell_tests
