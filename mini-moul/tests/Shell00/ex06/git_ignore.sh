#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex06"
TARGET="$EX_DIR/git_ignore.sh"

if ! check_delivery_file "$EX_DIR" "git_ignore.sh"; then
    finish_shell_tests
fi

TARGET_ABS="$(cd "$EX_DIR" && pwd)/git_ignore.sh"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Case 1: Repo with ignored existing files, ignored non-existing patterns, tracked & untracked files
REPO1="$TMP_DIR/repo1"
mkdir -p "$REPO1"
(
    cd "$REPO1" || exit 1
    git init -q
    cat <<'EOF' > .gitignore
.DS_Store
*~
*.log
nonexistent.tmp
EOF
    echo "tracked" > tracked.c
    git add .gitignore tracked.c
    git -c user.name="test" -c user.email="test@42.fr" commit -q -m "init"

    touch .DS_Store mywork.c~ debug.log untracked.c
)

(cd "$REPO1" && git ls-files -o -i --exclude-standard | LC_ALL=C sort) > "$TMP_DIR/exp1"
(cd "$REPO1" && /bin/sh "$TARGET_ABS" | LC_ALL=C sort) > "$TMP_DIR/act1_sh" 2>/dev/null
assert_files_equal "git_ignore.sh via /bin/sh lists only existing ignored files" "$TMP_DIR/exp1" "$TMP_DIR/act1_sh"

(cd "$REPO1" && bash "$TARGET_ABS" | LC_ALL=C sort) > "$TMP_DIR/act1_bash" 2>/dev/null
assert_files_equal "git_ignore.sh via bash lists only existing ignored files" "$TMP_DIR/exp1" "$TMP_DIR/act1_bash"

# Case 2: Repo with no ignored files -> empty output
REPO2="$TMP_DIR/repo2"
mkdir -p "$REPO2"
(
    cd "$REPO2" || exit 1
    git init -q
    echo "*.tmp" > .gitignore
    echo "hello" > main.c
    git add .gitignore main.c
    git -c user.name="test" -c user.email="test@42.fr" commit -q -m "init"
    touch other.c
)

: > "$TMP_DIR/exp2"
(cd "$REPO2" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/act2" 2>/dev/null
assert_files_equal "git_ignore.sh outputs nothing when no ignored files exist" "$TMP_DIR/exp2" "$TMP_DIR/act2"

finish_shell_tests
