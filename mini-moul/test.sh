#!/bin/bash

source config.sh

#utils
index=0
index2=0
assignment_data=NULL
test_data=NULL
test_error=NULL
test_name=NULL

#variables
checks=0
passed=0
marks=0
questions=0
dirname_found=0
break_score=0
score_false=0
available_assignments=""
result=""
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

normalize_assignment()
{
    local raw="$1"
    local lower
    lower="$(printf '%s' "$raw" | tr '[:upper:]' '[:lower:]')"
    case "$lower" in
        c0[0-9]|c1[0-3])
            printf 'C%s' "${lower#c}"
            ;;
        shell0[0-1])
            printf 'Shell%s' "${lower#shell}"
            ;;
        rush00)
            printf 'Rush00'
            ;;
        *)
            printf '%s' "$raw"
            ;;
    esac
}

get_allowed_functions()
{
    local proj="$1"
    local ex="$2"
    case "$proj:$ex" in
        C00:ex0[0-8])
            printf 'write'
            ;;
        C01:ex05)
            printf 'write'
            ;;
        C01:ex0[0-4]|C01:ex0[6-8])
            printf ''
            ;;
        C02:ex11|C02:ex12)
            printf 'write'
            ;;
        C02:ex0[0-9]|C02:ex10)
            printf ''
            ;;
        C03:ex0[0-5])
            printf ''
            ;;
        C04:ex01|C04:ex02|C04:ex04)
            printf 'write'
            ;;
        C04:ex00|C04:ex03|C04:ex05)
            printf ''
            ;;
        C05:ex08)
            printf 'write'
            ;;
        C05:ex0[0-7])
            printf ''
            ;;
        C06:ex0[0-3])
            printf 'write'
            ;;
        C07:ex04)
            printf 'malloc free'
            ;;
        C07:ex0[0-3]|C07:ex05)
            printf 'malloc'
            ;;
        C08:ex0[0-3])
            printf '__HEADER_ONLY__'
            ;;
        C08:ex04)
            printf 'malloc free'
            ;;
        C08:ex05)
            printf 'write'
            ;;
        C09:ex02)
            printf 'malloc'
            ;;
        C11:ex01)
            printf 'malloc'
            ;;
        C11:ex00|C11:ex02|C11:ex03|C11:ex04|C11:ex06|C11:ex07)
            printf ''
            ;;
        C12:ex00)
            printf 'malloc'
            ;;
        C12:ex01|C12:ex04|C12:ex05|C12:ex16)
            printf 'ft_create_elem'
            ;;
        C12:ex06|C12:ex12)
            printf 'free'
            ;;
        C12:ex02|C12:ex03|C12:ex07|C12:ex08|C12:ex09|C12:ex10|C12:ex11|C12:ex13|C12:ex14|C12:ex15|C12:ex17)
            printf ''
            ;;
        C13:ex00)
            printf 'malloc'
            ;;
        C13:ex04)
            printf 'btree_create_node'
            ;;
        C13:ex07)
            printf 'malloc free'
            ;;
        C13:ex01|C13:ex02|C13:ex03|C13:ex05|C13:ex06)
            printf ''
            ;;
        *)
            printf '__UNCHECKED__'
            ;;
    esac
}

get_expected_files()
{
    local proj="$1"
    local ex="$2"
    local default_file="$3"
    case "$proj:$ex" in
        C07:ex04)
            printf 'ft_convert_base.c ft_convert_base2.c'
            ;;
        C08:ex00)
            printf 'ft.h'
            ;;
        C08:ex01)
            printf 'ft_boolean.h'
            ;;
        C08:ex02)
            printf 'ft_abs.h'
            ;;
        C08:ex03)
            printf 'ft_point.h'
            ;;
        C12:ex08)
            printf '%s' "$default_file"
            ;;
        C12:*)
            printf '%s ft_list.h' "$default_file"
            ;;
        C13:*)
            printf '%s ft_btree.h' "$default_file"
            ;;
        *)
            printf '%s' "$default_file"
            ;;
    esac
}

is_program_exercise()
{
    local proj="$1"
    local ex="$2"
    case "$proj:$ex" in
        C06:ex0[0-3])
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

get_extra_include_flag()
{
    local proj="$1"
    local ex="$2"
    case "$proj:$ex" in
        C08:ex04)
            printf '%s' "-I./tests/C08/ex04"
            ;;
        C08:ex05)
            printf '%s' "-I./tests/C08/ex05/resources"
            ;;
        C12:ex08)
            printf '%s' "-std=gnu11 -I./tests/C12/ex08"
            ;;
        C12:*)
            printf '%s' "-std=gnu11 -I../$ex"
            ;;
        C13:*)
            printf '%s' "-I../$ex"
            ;;
        *)
            printf ''
            ;;
    esac
}

check_c_deliverables()
{
    local proj="$1"
    local ex="$2"
    local default_file="$3"
    local allowed
    allowed="$(get_allowed_functions "$proj" "$ex")"
    if [ "$allowed" = "__UNCHECKED__" ]; then
        return 0
    fi

    local expected_files
    expected_files="$(get_expected_files "$proj" "$ex" "$default_file")"
    local ex_dir="../$ex"
    local check_failed=0

    if [ ! -d "$ex_dir" ]; then
        printf "    ${RED}[0] Delivery directory %s/ does not exist.${DEFAULT}\n" "$ex"
        return 1
    fi

    local f
    for f in $expected_files; do
        if [ ! -f "$ex_dir/$f" ]; then
            printf "    ${RED}[0] Expected file %s/%s not found.${DEFAULT}\n" "$ex" "$f"
            check_failed=1
        fi
    done

    # Check for extra files in delivery directory
    local extras=()
    while IFS= read -r -d '' entry; do
        local base
        base="$(basename "$entry")"
        local is_expected=0
        for f in $expected_files; do
            if [ "$base" = "$f" ]; then
                is_expected=1
                break
            fi
        done
        if [ "$is_expected" -eq 0 ]; then
            extras+=("$base")
        fi
    done < <(find "$ex_dir" -mindepth 1 -maxdepth 1 -print0)

    if [ "${#extras[@]}" -gt 0 ]; then
        printf "    ${RED}[0] Extra files found in %s/: %s${DEFAULT}\n" "$ex" "${extras[*]}"
        check_failed=1
    fi

    if [ "$check_failed" -ne 0 ] || [ "$allowed" = "__HEADER_ONLY__" ]; then
        return "$check_failed"
    fi

    # Compile student .c file(s) standalone to verify headers, flags, main(), and allowed functions
    local inc_flag
    inc_flag="$(get_extra_include_flag "$proj" "$ex")"
    local tmp_dir
    tmp_dir="$(mktemp -d /tmp/mini_moul_obj.XXXXXX)"
    local objs=()
    local c_files=()
    for f in $expected_files; do
        case "$f" in
            *.c)
                c_files+=("$f")
                local obj="$tmp_dir/${f%.c}.o"
                if ! cc -Wall -Wextra -Werror -fno-stack-protector $inc_flag -c "$ex_dir/$f" -o "$obj" 2>/dev/null; then
                    printf "    ${RED}[0] %s/%s failed standalone compilation (-Wall -Wextra -Werror). Check headers/syntax.${DEFAULT}\n" "$ex" "$f"
                    rm -rf "$tmp_dir"
                    return 1
                fi
                objs+=("$obj")
                ;;
        esac
    done

    if command -v nm &> /dev/null && [ "${#objs[@]}" -gt 0 ]; then
        # Collect all symbols defined across the student's own delivered .c files
        local defined_syms
        defined_syms="$(nm -g --defined-only "${objs[@]}" 2>/dev/null | awk '{print $NF}')"

        if is_program_exercise "$proj" "$ex"; then
            if ! printf '%s\n' "$defined_syms" | grep -qx 'main'; then
                printf "    ${RED}[0] %s/ must contain a main() function.${DEFAULT}\n" "$ex"
                check_failed=1
            fi
        else
            if printf '%s\n' "$defined_syms" | grep -qx 'main'; then
                printf "    ${RED}[0] %s/ contains a main() function (forbidden for library exercises).${DEFAULT}\n" "$ex"
                check_failed=1
            fi
        fi

        # Check undefined external symbols against subject's authorized functions + own defined symbols
        local forbidden=()
        local sym
        while IFS= read -r sym; do
            [ -z "$sym" ] && continue
            case "$sym" in
                _* ) continue ;;
            esac
            if printf '%s\n' "$defined_syms" | grep -qx "$sym"; then
                continue
            fi
            local is_allowed=0
            local af
            for af in $allowed; do
                if [ "$sym" = "$af" ]; then
                    is_allowed=1
                    break
                fi
            done
            if [ "$is_allowed" -eq 0 ]; then
                forbidden+=("$sym")
            fi
        done < <(nm -u "${objs[@]}" 2>/dev/null | awk '{print $NF}' | sort -u)

        if [ "${#forbidden[@]}" -gt 0 ]; then
            local allowed_label="${allowed:-None}"
            printf "    ${RED}[0] Forbidden function(s) used in %s/: %s (allowed: %s)${DEFAULT}\n" "$ex" "${forbidden[*]}" "$allowed_label"
            check_failed=1
        fi
    fi

    rm -rf "$tmp_dir"
    return "$check_failed"
}

run_shell_assignment()
{
    local dir="$1"
    for assignment in "$dir"/*; do
        [ -d "$assignment" ] || continue
        questions=$((questions+1))
        score_false=0
        assignment_name="$(basename "$assignment")"
        test_file="$(ls "$assignment"/*.sh 2>/dev/null | head -n 1)"
        test_name="$(basename "$test_file")"

        if [ -f "$test_file" ]; then
            checks=$((checks+1))
            if bash "$test_file"; then
                passed=$((passed+1))
            else
                break_score=1
                score_false=1
            fi
            print_test_result
            space
        else
            break_score=1
            score_false=1
            printf "${RED}    %s has no test script.${DEFAULT}\n" "$assignment_name"
            print_test_result
            space
        fi
        ((index++))
    done
}

get_extra_link_sources()
{
    local proj="$1"
    local ex="$2"
    local test_file="$3"
    case "$proj:$ex" in
        C07:ex04)
            printf '%s' "../ex04/ft_convert_base.c ../ex04/ft_convert_base2.c"
            ;;
        C08:ex04)
            printf '%s' "../ex04/ft_strs_to_tab.c"
            ;;
        C08:ex05)
            printf '%s' "../ex05/ft_show_tab.c"
            ;;
        C12:*|C13:*)
            printf '%s' "../$ex/$test_file"
            ;;
        *)
            printf ''
            ;;
    esac
}

run_c_assignment()
{
    local dir="$1"
    local proj="$2"
    for assignment in "$dir"/*; do
        [ -d "$assignment" ] || continue
        questions=$((questions+1))
        score_false=0
        assignment_name="$(basename "$assignment")"

        local sh_test
        sh_test="$(ls "$assignment"/*.sh 2>/dev/null | head -n 1)"
        if [ -n "$sh_test" ] && [ -f "$sh_test" ]; then
            test_name="$(basename "$sh_test")"
            checks=$((checks+1))
            local sh_rc=0
            if command -v timeout &> /dev/null; then
                timeout 20s bash "$sh_test"
                sh_rc=$?
            else
                bash "$sh_test"
                sh_rc=$?
            fi
            if [ "$sh_rc" -eq 0 ]; then
                passed=$((passed+1))
            else
                if [ "$sh_rc" -eq 124 ]; then
                    printf "    ${RED}[TIMEOUT] %s exceeded 20s limit${DEFAULT}\n" "$test_name"
                fi
                break_score=1
                score_false=1
            fi
            print_test_result
            space
            ((index++))
            continue
        fi

        test_name="$(ls "$assignment"/*.c | head -n 1)"
        test_name="$(basename "$test_name")"

        if ! check_c_deliverables "$proj" "$assignment_name" "$test_name"; then
            break_score=1
            score_false=1
            checks=$((checks+1))
            print_test_result
            space
            ((index++))
            continue
        fi

        local inc_flag extra_srcs asan_flags=""
        inc_flag="$(get_extra_include_flag "$proj" "$assignment_name")"
        extra_srcs="$(get_extra_link_sources "$proj" "$assignment_name" "$test_name")"
        if [ "${MINI_ASAN:-0}" = "1" ]; then
            asan_flags="-fsanitize=address,undefined -fno-omit-frame-pointer -g"
        fi

        if cc -Wall -Werror -Wextra $inc_flag $asan_flags -o test1 "$(ls "$assignment"/*.c | head -n 1)" $extra_srcs; then
            rm -f test1
            checks=$((checks+1))
            passed=$((passed+1))

            if [ -d "$assignment" ]; then
                index2=0

                for test in "$assignment"/*.c; do
                    ((index2++))
                    checks=$((checks+1))

                    if cc -Wall -Werror -Wextra $inc_flag $asan_flags -o "${test%.c}" "$test" $extra_srcs 2> /dev/null; then
                        local run_rc=0
                        if [ "${MINI_ASAN:-0}" = "1" ]; then
                            if command -v timeout &> /dev/null; then
                                ASAN_OPTIONS="detect_leaks=1" UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1" timeout 15s "./${test%.c}"
                                run_rc=$?
                            else
                                ASAN_OPTIONS="detect_leaks=1" UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1" "./${test%.c}"
                                run_rc=$?
                            fi
                        else
                            if command -v timeout &> /dev/null; then
                                timeout 10s "./${test%.c}"
                                run_rc=$?
                            else
                                "./${test%.c}"
                                run_rc=$?
                            fi
                        fi
                        if [ "$run_rc" -eq 0 ]; then
                            passed=$((passed+1))
                        else
                            if [ "$run_rc" -eq 124 ]; then
                                printf "    ${RED}[TIMEOUT] %s exceeded time limit (possible infinite loop or slow algorithm)${DEFAULT}\n" "$test_name"
                            elif [ "${MINI_ASAN:-0}" = "1" ]; then
                                printf "    ${RED}[SANITIZER] %s exited with code %d (check ASan/UBSan/LSan report above)${DEFAULT}\n" "$test_name" "$run_rc"
                            fi
                            break_score=1
                            score_false=1
                        fi
                        rm -f "${test%.c}"
                    else
                        break_score=1
                        score_false=1
                        printf "    ${GREY}[$(($index2+1))] $test_error ${RED}FAILED${DEFAULT}\n"
                    fi
                done
                print_test_result
                space
            else
                printf "${RED}    $assignment_name does not exist.${DEFAULT}\n"
            fi
        else
            break_score=1
            checks=$((checks+1))
            printf "${RED}    $test_name cannot compile.${DEFAULT}\n"
            printf "${BG_RED}${BOLD} FAIL ${DEFAULT}${PURPLE} $assignment_name/${DEFAULT}$test_name\n"
            space

            if [ $index -gt 0 ]; then
                result+=", "
            fi
            result+="${RED}$assignment_name: KO${DEFAULT}"
        fi
        ((index++))
    done
}

main()
{
    start_time=$(date +%s)
    local target="$1"
    #print_collected_files
    for dir in ./tests/* ; do
        dirname="$(basename "$dir")"
        available_assignments+="$dirname "

        if [ -d "$dir" ] && [ "$dirname" == "$target" ]; then
            dirname_found=1
            print_header
            printf "${GREEN} Generating test for ${target}...\n${DEFAULT}"
            if [ "${MINI_ASAN:-0}" = "1" ]; then
                printf "${BLUE} Sanitizer mode enabled (MINI_ASAN=1: ASan + UBSan + LSan)\n${DEFAULT}"
            fi
            space
            index=0

            if [[ "$target" =~ ^(Shell|Rush) ]]; then
                run_shell_assignment "$dir"
            else
                run_c_assignment "$dir" "$target"
            fi
            break
        fi
    done

    if [ $dirname_found = 0 ]; then
        printf "${RED}Sorry. Tests for $target isn't available yet. Consider contributing at Github.${DEFAULT}\n"
        printf "Available assignment tests: ${PURPLE}$available_assignments${DEFAULT}\n"
        exit 1
    fi
    print_footer
}

print_header()
{
    printf "${PINK}"
    space
    printf " ███▄ ▄███▓ ██▓ ███▄    █  ██▓\n"
    printf "▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒\n"
    printf "▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒\n"
    printf "▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░\n"
    printf "▒██▒   ░██▒░██░▒██░   ▓██░░██░\n"
    printf "░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  \n"
    printf "░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░\n"
    printf "░      ░    ▒ ░   ░   ░ ░  ▒ ░\n"
    printf "       ░    ░           ░  ░  \n"
    printf "${DEFAULT}"
    printf "${BLUE}mini-mou ${DEFAULT}version 0.1.21 27 Feb 2023.\n"
    space
}

print_collected_files()
{
    printf "Collected files:\n"
    ls ../* | grep -v "../41test:*" | grep -v "../41test" | column
}

space()
{
    printf "\n"
}

print_test_result()
{
    if [ $index -gt 0 ]; then
        result+=", "
    fi
    if [ $score_false = 0 ]; then
        result+="${GREEN}$assignment_name: OK${DEFAULT}"
        printf "${BG_GREEN}${BLACK}${BOLD} PASS ${DEFAULT}${PURPLE} $assignment_name/${DEFAULT}$test_name\n"
    else
        result+="${RED}$assignment_name: KO${DEFAULT}"
        printf "${BG_RED}${BOLD} FAIL ${DEFAULT}${PURPLE} $assignment_name/${DEFAULT}$test_name\n"
    fi
    if [ $break_score = 0 ]; then
        marks=$((marks+1))
    fi
}

print_footer()
{
    printf "${PURPLE}-----------------------------------${DEFAULT}\n"
    space
    PERCENT=$((100 * marks / questions))
    #printf "Total checks:  ""${GREEN}${passed} passed  ${DEFAULT} ""${checks} total"
    printf "Result:        ${result}\n"
    if [ $PERCENT -ge 50 ]; then
        printf "Final score:   ${GREEN}${PERCENT}/100${DEFAULT}\n"
        printf "Status:        ${GREEN}passed${DEFAULT}\n"
    else
        printf "Final score:   ${RED}${PERCENT}/100${DEFAULT}\n"
        printf "Status:        ${RED}FAILED${DEFAULT}\n"
    fi
    end_time=$(date +%s)
    elapsed_time=$((end_time - start_time))
    printf "${GREY}Test completed. ${PINK}Total elapsed time: ${elapsed_time}s${DEFAULT}.\n"
    printf "${BLUE}mini-mou is updated daily. Please remember to git pull today!\n${DEFAULT}"
    space
    if [ $PERCENT -ge 50 ]; then
        return 0
    fi
    return 1
}

check_dependency()
{
    if ! command -v jq &> /dev/null; then
        printf "jq is not installed. To install:\n"
        printf "  Ubuntu/Debian:\n"
        printf "    sudo apt-get update\n"
        printf "    sudo apt-get install jq\n"
        printf "  macOS/Homebrew:\n"
        printf "    brew install jq\n"
    fi
}

#check_dependency
if [ "${1}" = "" ]; then
    printf "Please select an assignment. e.g. './test.sh C01', './test.sh Shell00', or './test.sh Rush00'\n"
    exit 1
fi
normalized_arg="$(normalize_assignment "${1}")"
if [[ "${normalized_arg}" =~ ^(C(0[0-9]|1[0-3])|Shell0[0-1]|Rush00)$ ]]; then
    main "${normalized_arg}"
    status=$?
    printf "$DEFAULT"
    exit "$status"
else
    printf "${RED}Invalid argument. Please select between C00 to C13, Shell00 to Shell01, or Rush00${DEFAULT}\n"
    exit 1
fi
