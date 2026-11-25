#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex03"
TARGET="$EX_DIR/id_rsa_pub"

if ! check_delivery_file "$EX_DIR" "id_rsa_pub"; then
    finish_shell_tests
fi

if [ ! -s "$TARGET" ]; then
    fail_test "id_rsa_pub is empty"
    finish_shell_tests
fi

if grep -qE 'BEGIN .*PRIVATE KEY' "$TARGET"; then
    fail_test "id_rsa_pub contains a PRIVATE key! Deliver only your PUBLIC key"
    finish_shell_tests
else
    pass_test "id_rsa_pub is not a private key"
fi

line_count="$(wc -l < "$TARGET" | tr -d ' ')"
non_empty_lines="$(grep -c '[^[:space:]]' "$TARGET" || true)"
if [ "$non_empty_lines" -ne 1 ]; then
    fail_test "id_rsa_pub should contain a single public key line, found $non_empty_lines"
else
    pass_test "id_rsa_pub contains a single key line"
fi

if head -n 1 "$TARGET" | grep -qE '^(ssh-rsa|ssh-ed25519|ecdsa-sha2-nistp256|ecdsa-sha2-nistp384|ecdsa-sha2-nistp521|sk-ssh-ed25519@openssh\.com|sk-ecdsa-sha2-nistp256@openssh\.com) [A-Za-z0-9+/=]+'; then
    pass_test "id_rsa_pub has valid SSH public key format"
else
    fail_test "id_rsa_pub does not match expected SSH public key format"
fi

if command -v ssh-keygen &> /dev/null; then
    if ssh-keygen -l -f "$TARGET" >/dev/null 2>&1; then
        pass_test "ssh-keygen validated id_rsa_pub fingerprint"
    else
        fail_test "ssh-keygen failed to parse id_rsa_pub as a valid SSH public key"
    fi
fi

finish_shell_tests
