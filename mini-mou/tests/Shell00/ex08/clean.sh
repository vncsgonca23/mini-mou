#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex08"
TARGET="$EX_DIR/clean"

if ! check_delivery_file "$EX_DIR" "clean"; then
    finish_shell_tests
fi

TARGET_ABS="$(cd "$EX_DIR" && pwd)/clean"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Check single-command constraint (no &&, ||, |, unescaped ;, command substitution, or multiple commands)
cmd_lines="$(grep -v '^[[:space:]]*$' "$TARGET_ABS" | grep -v '^[[:space:]]*#' || true)"
line_count="$(printf '%s\n' "$cmd_lines" | grep -c '[^[:space:]]' || true)"
if [ "$line_count" -ne 1 ]; then
    fail_test "clean must contain a single command (found $line_count command lines)"
else
    single_cmd="$cmd_lines"
    # Strip escaped \; so -exec ... \; is not confused with command chaining
    stripped="$(printf '%s' "$single_cmd" | sed 's/\\;//g')"
    if printf '%s' "$stripped" | grep -qE '(&&|\|\||;|\||`|\$\()'; then
        fail_test "clean must be a single command without ';', '&&', '||', '|' or chaining"
    else
        pass_test "clean contains a single unchained command"
    fi
fi

# Build test directory tree
WORK="$TMP_DIR/work"
mkdir -p "$WORK/sub/deep" "$WORK/dir~" "$WORK/#dir#"
touch "$WORK/file~" \
      "$WORK/#autosave#" \
      "$WORK/.hidden~" \
      "$WORK/sub/nested~" \
      "$WORK/sub/deep/#nested#" \
      "$WORK/sub/##" \
      "$WORK/sub/with space~" \
      "$WORK/normal.txt" \
      "$WORK/~leading_tilde" \
      "$WORK/mid~tilde.txt" \
      "$WORK/#only_start" \
      "$WORK/only_end#" \
      "$WORK/a#b#c" \
      "$WORK/sub/keep.c"

(cd "$WORK" && /bin/sh "$TARGET_ABS") > "$TMP_DIR/stdout" 2>"$TMP_DIR/stderr"
rc=$?

if [ "$rc" -eq 0 ]; then
    pass_test "clean exited with status 0 via /bin/sh"
else
    fail_test "clean exited with non-zero status $rc"
fi

# Check that all target files were deleted
should_delete=(
    "file~"
    "#autosave#"
    ".hidden~"
    "sub/nested~"
    "sub/deep/#nested#"
    "sub/##"
    "sub/with space~"
)
missing_del=()
for f in "${should_delete[@]}"; do
    if [ -e "$WORK/$f" ]; then
        missing_del+=("$f")
    fi
done
if [ "${#missing_del[@]}" -eq 0 ]; then
    pass_test "All matching files (*~ and #*#) were deleted"
else
    fail_test "Failed to delete matching file(s): ${missing_del[*]}"
fi

# Check that non-matching files and directories were preserved
should_keep=(
    "normal.txt"
    "~leading_tilde"
    "mid~tilde.txt"
    "#only_start"
    "only_end#"
    "a#b#c"
    "sub/keep.c"
    "dir~"
    "#dir#"
)
wrong_del=()
for f in "${should_keep[@]}"; do
    if [ ! -e "$WORK/$f" ]; then
        wrong_del+=("$f")
    fi
done
if [ "${#wrong_del[@]}" -eq 0 ]; then
    pass_test "All non-matching files and directories were preserved"
else
    fail_test "Incorrectly deleted non-matching entry/entries: ${wrong_del[*]}"
fi

# Check that stdout printed the deleted files (normalized removing optional leading ./)
sed 's|^\./||' "$TMP_DIR/stdout" | LC_ALL=C sort > "$TMP_DIR/act_printed"
printf '%s\n' "${should_delete[@]}" | LC_ALL=C sort > "$TMP_DIR/exp_printed"
assert_files_equal "clean printed every deleted file and nothing else" "$TMP_DIR/exp_printed" "$TMP_DIR/act_printed"

finish_shell_tests
