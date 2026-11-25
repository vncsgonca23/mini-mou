#!/bin/bash

source "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." &> /dev/null && pwd)/config.sh"

ERRORS=0
TEST_NUM=0

pass_test() {
    TEST_NUM=$((TEST_NUM + 1))
    printf "  ${GREEN}${CHECKMARK}${GREY} [%d] %s\n${DEFAULT}" "$TEST_NUM" "$1"
}

fail_test() {
    TEST_NUM=$((TEST_NUM + 1))
    printf "    ${RED}[%d] %s\n${DEFAULT}" "$TEST_NUM" "$1"
    ERRORS=$((ERRORS + 1))
}

# Verify delivery directory exists, contains exact expected file, and has no extra files
check_delivery_file() {
    local ex_dir="$1"
    local expected="$2"

    if [ ! -d "$ex_dir" ]; then
        fail_test "Delivery directory ${ex_dir#../}/ does not exist"
        return 1
    fi

    if [ ! -e "$ex_dir/$expected" ]; then
        fail_test "Expected file ${ex_dir#../}/${expected} not found"
        return 1
    fi

    if [ ! -f "$ex_dir/$expected" ] || [ -L "$ex_dir/$expected" ]; then
        fail_test "Expected ${ex_dir#../}/${expected} to be a regular file"
        return 1
    fi

    local extras=()
    local entry base
    while IFS= read -r -d '' entry; do
        base="$(basename "$entry")"
        if [ "$base" != "$expected" ]; then
            extras+=("$base")
        fi
    done < <(find "$ex_dir" -mindepth 1 -maxdepth 1 -print0)

    if [ "${#extras[@]}" -gt 0 ]; then
        fail_test "Extra file(s) in ${ex_dir#../}/: ${extras[*]}"
        return 1
    fi

    pass_test "Delivery file ${ex_dir#../}/${expected} present with no extra files"
    return 0
}

check_executable() {
    local file="$1"
    if [ -x "$file" ]; then
        pass_test "$(basename "$file") has executable permission (+x)"
        return 0
    else
        fail_test "$(basename "$file") is missing executable permission (chmod +x)"
        return 1
    fi
}

# Compare two files byte-for-byte
assert_files_equal() {
    local desc="$1"
    local expected_file="$2"
    local actual_file="$3"

    if cmp -s "$expected_file" "$actual_file"; then
        pass_test "$desc"
        return 0
    else
        local exp_preview act_preview
        exp_preview="$(cat -e "$expected_file" | head -n 5 | tr '\n' ' ')"
        act_preview="$(cat -e "$actual_file" | head -n 5 | tr '\n' ' ')"
        fail_test "$desc (Expected: \"${exp_preview}\", got: \"${act_preview}\")"
        return 1
    fi
}

finish_shell_tests() {
    if [ "$ERRORS" -ne 0 ]; then
        exit 1
    fi
    exit 0
}
