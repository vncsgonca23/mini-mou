#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex03"
TARGET="$EX_DIR/count_files.sh"

if ! check_delivery_file "$EX_DIR" "count_files.sh"; then
    finish_shell_tests
fi

check_executable "$TARGET"

TARGET_ABS="$(cd "$EX_DIR" && pwd)/count_files.sh"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Case 1: Empty directory -> 1 (counts '.')
WORK1="$TMP_DIR/empty"
mkdir -p "$WORK1"
printf '1\n' > "$TMP_DIR/exp1"
(cd "$WORK1" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/act1" 2>/dev/null
assert_files_equal "Empty directory counts as 1 (includes '.')" "$TMP_DIR/exp1" "$TMP_DIR/act1"

# Case 2: Nested dirs, regular files, hidden files, and a symlink (symlinks are NOT regular files/dirs)
WORK2="$TMP_DIR/tree"
mkdir -p "$WORK2/a/b" "$WORK2/.hidden_dir"
touch "$WORK2/f1" "$WORK2/.hidden_file" "$WORK2/a/f2" "$WORK2/a/b/f3" "$WORK2/.hidden_dir/f4"
ln -s "$WORK2/f1" "$WORK2/sym_link"
# Count: . (1) + a (2) + a/b (3) + .hidden_dir (4) + f1 (5) + .hidden_file (6) + f2 (7) + f3 (8) + f4 (9) = 9
printf '9\n' > "$TMP_DIR/exp2"

(cd "$WORK2" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/act2_sh" 2>/dev/null
assert_files_equal "Nested tree counts regular files and directories (9, excluding symlink, no spaces)" "$TMP_DIR/exp2" "$TMP_DIR/act2_sh"

if [ -x "$TARGET_ABS" ]; then
    (cd "$WORK2" && "$TARGET_ABS") > "$TMP_DIR/act2_exec" 2>/dev/null
    assert_files_equal "count_files.sh via ./count_files.sh matches expected count" "$TMP_DIR/exp2" "$TMP_DIR/act2_exec"
fi

finish_shell_tests
