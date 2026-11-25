#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex07"
TARGET="$EX_DIR/b"

if ! check_delivery_file "$EX_DIR" "b"; then
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

if [ -s "$TARGET" ]; then
    pass_test "File ex07/b is non-empty"
else
    fail_test "File ex07/b is empty"
    finish_shell_tests
fi

if LC_ALL=C grep -q $'\r' "$TARGET"; then
    fail_test "File ex07/b contains Windows CRLF (\\r) line endings"
else
    pass_test "File ex07/b uses Unix LF line endings"
fi

# Original 'a' file from the subject PDF
cat <<'EOF' > "$TMP_DIR/a"
STARWARS
Episode IV, A NEW HOPE It is a period of civil war.

Rebel spaceships, striking from a hidden base, have won their first victory against the evil Galactic Empire.
During the battle, Rebel spies managed to steal secret plans to the Empire's ultimate weapon, the DEATH STAR,
an armored space station with enough power to destroy an entire planet.

Pursued by the Empire's sinister agents, Princess Leia races home aboard her starship, custodian of the stolen plans that can save her people and restore freedom to the galaxy...

EOF

if cmp -s "$TMP_DIR/a" "$TARGET"; then
    fail_test "File ex07/b is identical to original file 'a' (patch was not applied)"
else
    pass_test "File ex07/b differs from original file 'a'"
fi

# If the user places their intranet sw.diff in tests/Shell00/ex07/resources/sw.diff,
# perform exact diff validation: diff a b == sw.diff
if [ -f "$SCRIPT_DIR/resources/sw.diff" ]; then
    diff "$TMP_DIR/a" "$TARGET" > "$TMP_DIR/actual.diff" || true
    assert_files_equal "diff a b matches resources/sw.diff byte-for-byte" "$SCRIPT_DIR/resources/sw.diff" "$TMP_DIR/actual.diff"
fi

finish_shell_tests
