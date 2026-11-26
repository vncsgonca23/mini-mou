#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex04"
TARGET="$EX_DIR/midLS"

if ! check_delivery_file "$EX_DIR" "midLS"; then
    finish_shell_tests
fi

TARGET_ABS="$(cd "$EX_DIR" && pwd)/midLS"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Test 1: Mixed regular files, executable, directory, and hidden files/dirs with distinct mtimes
WORK1="$TMP_DIR/case1"
mkdir -p "$WORK1/sub_dir" "$WORK1/.hidden_dir"
touch "$WORK1/old_file" "$WORK1/exec_file" "$WORK1/new_file" "$WORK1/.hidden_file"
chmod +x "$WORK1/exec_file"
touch -d "2024-01-01 00:00:01" "$WORK1/old_file"
touch -d "2024-01-01 00:00:02" "$WORK1/exec_file"
touch -d "2024-01-01 00:00:03" "$WORK1/sub_dir"
touch -d "2024-01-01 00:00:04" "$WORK1/new_file"
touch -d "2024-01-01 00:00:05" "$WORK1/.hidden_file"
touch -d "2024-01-01 00:00:06" "$WORK1/.hidden_dir"

(cd "$WORK1" && ls -mpt) > "$TMP_DIR/exp1"
(cd "$WORK1" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/act1" 2>/dev/null
assert_files_equal "midLS orders by mtime, appends '/' only to dirs, excludes hidden entries" "$TMP_DIR/exp1" "$TMP_DIR/act1"

# Test 2: Directory with only hidden files -> empty output
WORK2="$TMP_DIR/case2"
mkdir -p "$WORK2/.secret_dir"
touch "$WORK2/.secret_file"
(cd "$WORK2" && ls -mpt) > "$TMP_DIR/exp2"
(cd "$WORK2" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/act2" 2>/dev/null
assert_files_equal "midLS excludes hidden files when no visible files exist" "$TMP_DIR/exp2" "$TMP_DIR/act2"

# Test 3: Multiple directories and files with reversed chronological order
WORK3="$TMP_DIR/case3"
mkdir -p "$WORK3/dir_a" "$WORK3/dir_z"
touch "$WORK3/file_m"
touch -d "2024-06-01 10:00:00" "$WORK3/dir_z"
touch -d "2024-06-01 10:01:00" "$WORK3/file_m"
touch -d "2024-06-01 10:02:00" "$WORK3/dir_a"
(cd "$WORK3" && ls -mpt) > "$TMP_DIR/exp3"
(cd "$WORK3" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/act3" 2>/dev/null
assert_files_equal "midLS handles multiple directories sorted by modification time" "$TMP_DIR/exp3" "$TMP_DIR/act3"

finish_shell_tests
