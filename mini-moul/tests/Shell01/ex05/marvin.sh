#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex05"
EXPECTED_NAME=$'"\\?$*\'MaRViN\'*$?\\"'
TARGET="$EX_DIR/$EXPECTED_NAME"

if ! check_delivery_file "$EX_DIR" "$EXPECTED_NAME"; then
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

size="$(stat -c '%s' "$TARGET")"
if [ "$size" = "2" ]; then
    pass_test "File size is exactly 2 bytes (no trailing newline)"
else
    fail_test "File size Expected 2 bytes, got $size bytes"
fi

printf '42' > "$TMP_DIR/expected"
assert_files_equal "File content is exactly \"42\" and nothing more" "$TMP_DIR/expected" "$TARGET"

finish_shell_tests
