#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex08"
TARGET="$EX_DIR/add_chelou.sh"

if ! check_delivery_file "$EX_DIR" "add_chelou.sh"; then
    finish_shell_tests
fi

check_executable "$TARGET"

TARGET_ABS="$(cd "$EX_DIR" && pwd)/add_chelou.sh"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

run_case() {
    local desc="$1"
    local nbr1="$2"
    local nbr2="$3"
    local expected="$4"

    printf '%s\n' "$expected" > "$TMP_DIR/exp"
    FT_NBR1="$nbr1" FT_NBR2="$nbr2" /bin/sh "$TARGET_ABS" > "$TMP_DIR/act_sh" 2>/dev/null
    assert_files_equal "$desc (via /bin/sh)" "$TMP_DIR/exp" "$TMP_DIR/act_sh"
}

# Subject Example 1: FT_NBR1=\'?"\"'\  FT_NBR2=rcrdmddd -> Salut
run_case "Subject Example 1 -> Salut" $'\\\'?"\\"\'\\' "rcrdmddd" "Salut"

# Subject Example 2: FT_NBR1=\"\"!\"\"!\"\"!\"\"!\"\"!\"\"  FT_NBR2=dcrcmcmooododmrrrmorcmcrmomo -> Segmentation fault
run_case "Subject Example 2 -> Segmentation fault" $'\\"\\"!\\"\\"!\\"\\"!\\"\\"!\\"\\"!\\"\\"' "dcrcmcmooododmrrrmorcmcrmomo" "Segmentation fault"

# Edge cases
run_case "Zero + Zero -> g" "'" "m" "g"
run_case "1 + 1 -> a" "\\" "r" "a"
run_case "4 + 4 -> S" "!" "c" "S"
run_case "Leading zeros -> S" "'''!" "mmmc" "S"
run_case "Arbitrary precision (>64-bit) addition" "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" "ccccccccccccccccccccccccccccccccccc" "teuiuSuuagogo u lotuutt"

# Also verify direct execution (./add_chelou.sh) with Example 1
if [ -x "$TARGET_ABS" ]; then
    printf '%s\n' "Salut" > "$TMP_DIR/exp"
    FT_NBR1=$'\\\'?"\\"\'\\' FT_NBR2="rcrdmddd" "$TARGET_ABS" > "$TMP_DIR/act_exec" 2>/dev/null
    assert_files_equal "Subject Example 1 (via ./add_chelou.sh)" "$TMP_DIR/exp" "$TMP_DIR/act_exec"
fi

finish_shell_tests
