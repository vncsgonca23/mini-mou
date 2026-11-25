#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex07"
TARGET="$EX_DIR/r_dwssap.sh"

if ! check_delivery_file "$EX_DIR" "r_dwssap.sh"; then
    finish_shell_tests
fi

check_executable "$TARGET"

TARGET_ABS="$(cd "$EX_DIR" && pwd)/r_dwssap.sh"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Compute exact expected output from /etc/passwd following the 7 steps in the subject PDF
build_expected() {
    local l1="$1"
    local l2="$2"
    local out_file="$3"

    LC_ALL=C awk -F: '!/^#/ { c++; if (c % 2 == 0) print $1 }' /etc/passwd \
        | rev \
        | LC_ALL=C sort -r \
        | sed -n "${l1},${l2}p" \
        | awk '{ if (NR > 1) printf ", "; printf "%s", $0 } END { if (NR > 0) printf "." }' \
        > "$out_file"
}

run_range_test() {
    local l1="$1"
    local l2="$2"
    local label="$3"

    build_expected "$l1" "$l2" "$TMP_DIR/exp"

    LC_ALL=C FT_LINE1="$l1" FT_LINE2="$l2" /bin/sh "$TARGET_ABS" > "$TMP_DIR/act_sh" 2>/dev/null
    assert_files_equal "r_dwssap.sh ($label: FT_LINE1=$l1, FT_LINE2=$l2) via /bin/sh" "$TMP_DIR/exp" "$TMP_DIR/act_sh"

    if [ -x "$TARGET_ABS" ]; then
        LC_ALL=C FT_LINE1="$l1" FT_LINE2="$l2" "$TARGET_ABS" > "$TMP_DIR/act_exec" 2>/dev/null
        assert_files_equal "r_dwssap.sh ($label: FT_LINE1=$l1, FT_LINE2=$l2) via ./r_dwssap.sh" "$TMP_DIR/exp" "$TMP_DIR/act_exec"
    fi
}

total_even="$(awk '!/^#/ { c++ } END { print int(c / 2) }' /etc/passwd)"

run_range_test 1 5 "range 1..5"
if [ "$total_even" -ge 15 ]; then
    run_range_test 7 15 "subject example 7..15"
elif [ "$total_even" -ge 6 ]; then
    run_range_test 2 "$total_even" "range 2..$total_even"
fi
run_range_test 3 3 "single login (no trailing comma)"

finish_shell_tests
