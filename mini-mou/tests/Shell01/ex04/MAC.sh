#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex04"
TARGET="$EX_DIR/MAC.sh"

if ! check_delivery_file "$EX_DIR" "MAC.sh"; then
    finish_shell_tests
fi

check_executable "$TARGET"

TARGET_ABS="$(cd "$EX_DIR" && pwd)/MAC.sh"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Determine expected system MAC addresses (Linux ifconfig / ip link / sysfs)
RUN_PATH="$PATH"
if command -v ifconfig &> /dev/null; then
    ifconfig -a 2>/dev/null | awk '/ether[[:space:]]/ {print tolower($2)}' | LC_ALL=C sort > "$TMP_DIR/exp"
elif command -v ip &> /dev/null; then
    ip link show 2>/dev/null | awk '/link\/ether[[:space:]]/ {print tolower($2)}' | LC_ALL=C sort > "$TMP_DIR/exp"
    # Provide an ifconfig shim at the END of PATH when net-tools is not installed
    mkdir -p "$TMP_DIR/shim"
    cat <<'EOF' > "$TMP_DIR/shim/ifconfig"
#!/bin/sh
ip link show | awk '
/^[0-9]+:/ {
    iface = $2
    sub(/:$/, "", iface)
    printf "%s: flags=4163<UP>\n", iface
}
/link\/ether[[:space:]]/ {
    printf "        ether %s  txqueuelen 1000  (Ethernet)\n", $2
}
'
EOF
    chmod +x "$TMP_DIR/shim/ifconfig"
    RUN_PATH="$PATH:$TMP_DIR/shim"
else
    for f in /sys/class/net/*/address; do
        [ -r "$f" ] || continue
        mac="$(tr '[:upper:]' '[:lower:]' < "$f")"
        [ "$mac" = "00:00:00:00:00:00" ] && continue
        printf '%s\n' "$mac"
    done | LC_ALL=C sort > "$TMP_DIR/exp"
fi

PATH="$RUN_PATH" /bin/sh "$TARGET_ABS" > "$TMP_DIR/raw_sh" 2>"$TMP_DIR/err_sh"
rc=$?
if [ "$rc" -eq 0 ]; then
    pass_test "MAC.sh exited with status 0 via /bin/sh"
else
    fail_test "MAC.sh exited with non-zero status $rc"
fi

# Verify every output line strictly matches a MAC address (xx:xx:xx:xx:xx:xx)
bad_lines="$(grep -vE '^([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}$' "$TMP_DIR/raw_sh" || true)"
if [ -z "$bad_lines" ]; then
    pass_test "Every output line strictly matches MAC format (xx:xx:xx:xx:xx:xx)"
else
    fail_test "Invalid MAC line(s) detected: $(printf '%s' "$bad_lines" | head -n 2 | tr '\n' ' ')"
fi

tr '[:upper:]' '[:lower:]' < "$TMP_DIR/raw_sh" | LC_ALL=C sort > "$TMP_DIR/act_sh"
assert_files_equal "MAC.sh outputs all machine Ethernet MAC addresses" "$TMP_DIR/exp" "$TMP_DIR/act_sh"

if [ -x "$TARGET_ABS" ]; then
    PATH="$RUN_PATH" "$TARGET_ABS" > "$TMP_DIR/raw_exec" 2>/dev/null
    tr '[:upper:]' '[:lower:]' < "$TMP_DIR/raw_exec" | LC_ALL=C sort > "$TMP_DIR/act_exec"
    assert_files_equal "MAC.sh via ./MAC.sh matches expected MAC addresses" "$TMP_DIR/exp" "$TMP_DIR/act_exec"
fi

finish_shell_tests
