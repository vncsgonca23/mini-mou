#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex05"
TARGET="$EX_DIR/git_commit.sh"

if ! check_delivery_file "$EX_DIR" "git_commit.sh"; then
    finish_shell_tests
fi

TARGET_ABS="$(cd "$EX_DIR" && pwd)/git_commit.sh"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Case 1: Repository with 7 commits (must output only the last 5)
REPO1="$TMP_DIR/repo7"
mkdir -p "$REPO1"
(
    cd "$REPO1" || exit 1
    git init -q
    for i in 1 2 3 4 5 6 7; do
        echo "commit $i" > "file_$i.txt"
        git add "file_$i.txt"
        git -c user.name="test" -c user.email="test@42.fr" commit -q -m "commit $i"
    done
)

(cd "$REPO1" && git log -n 5 --format=%H) > "$TMP_DIR/exp1"
(cd "$REPO1" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/act1_sh" 2>/dev/null
assert_files_equal "git_commit.sh via /bin/sh outputs exact last 5 commit IDs (7 commits repo)" "$TMP_DIR/exp1" "$TMP_DIR/act1_sh"

(cd "$REPO1" && bash "$TARGET_ABS") > "$TMP_DIR/act1_bash" 2>/dev/null
assert_files_equal "git_commit.sh via bash outputs exact last 5 commit IDs (7 commits repo)" "$TMP_DIR/exp1" "$TMP_DIR/act1_bash"

# Case 2: Repository with only 3 commits (must output all 3)
REPO2="$TMP_DIR/repo3"
mkdir -p "$REPO2"
(
    cd "$REPO2" || exit 1
    git init -q
    for i in 1 2 3; do
        echo "commit $i" > "f_$i.txt"
        git add "f_$i.txt"
        git -c user.name="test" -c user.email="test@42.fr" commit -q -m "commit $i"
    done
)

(cd "$REPO2" && git log -n 5 --format=%H) > "$TMP_DIR/exp2"
(cd "$REPO2" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/act2" 2>/dev/null
assert_files_equal "git_commit.sh outputs all 3 commits when repo has fewer than 5 commits" "$TMP_DIR/exp2" "$TMP_DIR/act2"

finish_shell_tests
