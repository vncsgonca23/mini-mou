#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex02"
ARCHIVE="$EX_DIR/exo2.tar"

if ! check_delivery_file "$EX_DIR" "exo2.tar"; then
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
cleanup() {
    chmod -R u+rwx "$TMP_DIR" 2>/dev/null || true
    rm -rf "$TMP_DIR"
}
trap cleanup EXIT

ARCHIVE_ABS="$(cd "$EX_DIR" && pwd)/exo2.tar"

if ! (cd "$TMP_DIR" && umask 000 && tar -xf "$ARCHIVE_ABS" 2>/dev/null); then
    fail_test "Failed to extract exo2.tar"
    finish_shell_tests
fi

entries="$(find "$TMP_DIR" -mindepth 1 -maxdepth 1 -printf '%f\n' | LC_ALL=C sort | tr '\n' ' ' | sed 's/ $//')"
expected_entries="test0 test1 test2 test3 test4 test5 test6"
if [ "$entries" = "$expected_entries" ]; then
    pass_test "Archive contains exact entries: test0..test6"
else
    fail_test "Archive entries Expected \"$expected_entries\", got \"$entries\""
    finish_shell_tests
fi

check_entry() {
    local name="$1"
    local exp_perms="$2"
    local exp_links="$3"
    local exp_size="$4"
    local exp_time="$5"
    local path="$TMP_DIR/$name"

    local perms links size epoch fmt ok=1
    perms="$(stat -c '%A' "$path")"
    links="$(stat -c '%h' "$path")"
    size="$(stat -c '%s' "$path")"
    epoch="$(stat -c '%Y' "$path")"
    fmt="$(date -d "@$epoch" "+%m %d %H:%M")"

    if [ "$perms" != "$exp_perms" ]; then
        fail_test "$name permissions Expected \"$exp_perms\", got \"$perms\""
        ok=0
    fi
    if [ "$links" != "$exp_links" ]; then
        fail_test "$name links Expected $exp_links, got $links"
        ok=0
    fi
    if [ "$exp_size" != "ANY" ] && [ "$size" != "$exp_size" ]; then
        fail_test "$name size Expected $exp_size, got $size"
        ok=0
    fi
    if [ "$fmt" != "$exp_time" ]; then
        fail_test "$name timestamp Expected \"$exp_time\", got \"$fmt\""
        ok=0
    fi
    if [ "$ok" -eq 1 ]; then
        pass_test "$name attributes ($exp_perms, links=$exp_links, size=$exp_size, $exp_time)"
    fi
}

check_entry "test0" "drwx--xr-x" "2" "ANY" "06 01 20:47"
check_entry "test1" "-rwx--xr--" "1" "4"   "06 01 21:46"
check_entry "test2" "dr-x---r--" "2" "ANY" "06 01 22:45"
check_entry "test3" "-r-----r--" "2" "1"   "06 01 23:44"
check_entry "test4" "-rw-r----x" "1" "2"   "06 01 23:43"
check_entry "test5" "-r-----r--" "2" "1"   "06 01 23:44"
check_entry "test6" "lrwxrwxrwx" "1" "5"   "06 01 22:20"

ino3="$(stat -c '%i' "$TMP_DIR/test3")"
ino5="$(stat -c '%i' "$TMP_DIR/test5")"
if [ "$ino3" = "$ino5" ]; then
    pass_test "test3 and test5 are hard links (same inode)"
else
    fail_test "test3 and test5 are NOT hard links (inode $ino3 != $ino5)"
fi

link_target="$(readlink "$TMP_DIR/test6")"
if [ "$link_target" = "test0" ]; then
    pass_test "test6 symlink points to 'test0'"
else
    fail_test "test6 symlink Expected \"test0\", got \"$link_target\""
fi

finish_shell_tests
