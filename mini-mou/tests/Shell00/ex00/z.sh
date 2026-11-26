#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex00"
TARGET="$EX_DIR/z"

if ! check_delivery_file "$EX_DIR" "z"; then
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

printf 'Z\n' > "$TMP_DIR/expected"
cat "$TARGET" > "$TMP_DIR/actual"

assert_files_equal "cat z outputs exact 'Z' followed by newline" "$TMP_DIR/expected" "$TMP_DIR/actual"

finish_shell_tests
