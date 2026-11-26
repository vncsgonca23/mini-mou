#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex01"
ARCHIVE="$EX_DIR/testShell00.tar"

if ! check_delivery_file "$EX_DIR" "testShell00.tar"; then
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
cleanup() {
    chmod -R u+rwx "$TMP_DIR" 2>/dev/null || true
    rm -rf "$TMP_DIR"
}
trap cleanup EXIT

ARCHIVE_ABS="$(cd "$EX_DIR" && pwd)/testShell00.tar"

if ! (cd "$TMP_DIR" && umask 000 && tar -xf "$ARCHIVE_ABS" 2>/dev/null); then
    fail_test "Failed to extract testShell00.tar"
    finish_shell_tests
fi

entries=()
while IFS= read -r -d '' e; do
    entries+=("$(basename "$e")")
done < <(find "$TMP_DIR" -mindepth 1 -maxdepth 1 -print0)

if [ "${#entries[@]}" -ne 1 ] || [ "${entries[0]}" != "testShell00" ]; then
    fail_test "Archive should contain only 'testShell00', got: ${entries[*]}"
    finish_shell_tests
else
    pass_test "Archive contains only 'testShell00'"
fi

F="$TMP_DIR/testShell00"
perms="$(stat -c '%A' "$F")"
if [ "$perms" = "-r--r-xr-x" ]; then
    pass_test "Permissions match -r--r-xr-x"
else
    fail_test "Permissions Expected \"-r--r-xr-x\", got \"$perms\""
fi

size="$(stat -c '%s' "$F")"
if [ "$size" = "40" ]; then
    pass_test "Size matches 40 bytes"
else
    fail_test "Size Expected 40 bytes, got $size bytes"
fi

links="$(stat -c '%h' "$F")"
if [ "$links" = "1" ]; then
    pass_test "Link count matches 1"
else
    fail_test "Link count Expected 1, got $links"
fi

mtime_epoch="$(stat -c '%Y' "$F")"
mtime_fmt="$(date -d "@$mtime_epoch" "+%m %d %H:%M")"
if [ "$mtime_fmt" = "06 01 23:42" ]; then
    pass_test "Timestamp matches Jun 1 23:42"
else
    fail_test "Timestamp Expected \"06 01 23:42\" (Jun 1 23:42), got \"$mtime_fmt\""
fi

finish_shell_tests
