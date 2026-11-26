#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex02"
TARGET="$EX_DIR/find_sh.sh"

if ! check_delivery_file "$EX_DIR" "find_sh.sh"; then
    finish_shell_tests
fi

check_executable "$TARGET"

TARGET_ABS="$(cd "$EX_DIR" && pwd)/find_sh.sh"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

WORK="$TMP_DIR/work"
mkdir -p "$WORK/sub/deep" "$WORK/dir.sh"
touch "$WORK/file1.sh" \
      "$WORK/sub/file2.sh" \
      "$WORK/sub/deep/my.script.sh" \
      "$WORK/sub/sh.sh" \
      "$WORK/my_sh_file.sh" \
      "$WORK/.hidden.sh" \
      "$WORK/not_sh.txt" \
      "$WORK/file.sh.bak" \
      "$WORK/dir.sh/inside.sh"

printf '%s\n' \
    ".hidden" \
    "file1" \
    "file2" \
    "inside" \
    "my.script" \
    "my_sh_file" \
    "sh" | LC_ALL=C sort > "$TMP_DIR/exp"

(cd "$WORK" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/raw_sh" 2>/dev/null
LC_ALL=C sort "$TMP_DIR/raw_sh" > "$TMP_DIR/act_sh"
assert_files_equal "find_sh.sh via /bin/sh lists only .sh filenames without path or extension" "$TMP_DIR/exp" "$TMP_DIR/act_sh"

if [ -x "$TARGET_ABS" ]; then
    (cd "$WORK" && "$TARGET_ABS") > "$TMP_DIR/raw_exec" 2>/dev/null
    LC_ALL=C sort "$TMP_DIR/raw_exec" > "$TMP_DIR/act_exec"
    assert_files_equal "find_sh.sh via ./find_sh.sh matches expected output" "$TMP_DIR/exp" "$TMP_DIR/act_exec"
fi

# Empty directory with no .sh files -> empty output
WORK2="$TMP_DIR/empty"
mkdir -p "$WORK2/folder.sh"
touch "$WORK2/readme.txt" "$WORK2/test.sh.txt"
: > "$TMP_DIR/exp_empty"
(cd "$WORK2" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/act_empty" 2>/dev/null
assert_files_equal "find_sh.sh outputs nothing when no .sh regular files exist" "$TMP_DIR/exp_empty" "$TMP_DIR/act_empty"

finish_shell_tests
