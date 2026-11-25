#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex01"
TARGET="$EX_DIR/print_groups.sh"

if ! check_delivery_file "$EX_DIR" "print_groups.sh"; then
    finish_shell_tests
fi

check_executable "$TARGET"

TARGET_ABS="$(cd "$EX_DIR" && pwd)/print_groups.sh"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

test_user() {
    local user="$1"
    id -Gn "$user" | tr ' ' ',' | tr -d '\n' > "$TMP_DIR/exp"

    FT_USER="$user" /bin/sh "$TARGET_ABS" > "$TMP_DIR/act_sh" 2>/dev/null
    assert_files_equal "FT_USER=$user via /bin/sh matches comma-separated groups (no trailing newline)" "$TMP_DIR/exp" "$TMP_DIR/act_sh"

    if [ -x "$TARGET_ABS" ]; then
        FT_USER="$user" "$TARGET_ABS" > "$TMP_DIR/act_exec" 2>/dev/null
        assert_files_equal "FT_USER=$user via ./print_groups.sh matches expected output" "$TMP_DIR/exp" "$TMP_DIR/act_exec"
    fi
}

cur_user="$(id -un)"
test_user "$cur_user"
if id root >/dev/null 2>&1 && [ "$cur_user" != "root" ]; then
    test_user "root"
fi
if id daemon >/dev/null 2>&1 && [ "$cur_user" != "daemon" ]; then
    test_user "daemon"
fi

finish_shell_tests
