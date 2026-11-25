#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex00"

if [ ! -d "$EX_DIR" ]; then
    fail_test "Delivery directory ex00/ does not exist"
    finish_shell_tests
fi

# Collect delivered files and detect rush00.c..rush04.c variants
rush_files=()
extras=()
has_main=0
has_putchar=0

while IFS= read -r -d '' entry; do
    base="$(basename "$entry")"
    case "$base" in
        main.c)
            [ -f "$entry" ] && [ ! -L "$entry" ] && has_main=1
            ;;
        ft_putchar.c)
            [ -f "$entry" ] && [ ! -L "$entry" ] && has_putchar=1
            ;;
        rush0[0-4].c)
            if [ -f "$entry" ] && [ ! -L "$entry" ]; then
                rush_files+=("$base")
            else
                extras+=("$base")
            fi
            ;;
        *)
            extras+=("$base")
            ;;
    esac
done < <(find "$EX_DIR" -mindepth 1 -maxdepth 1 -print0)

# Sort rush_files deterministically
if [ "${#rush_files[@]}" -gt 0 ]; then
    IFS=$'\n' rush_files=($(printf '%s\n' "${rush_files[@]}" | sort))
    unset IFS
fi

delivery_ok=1
if [ "$has_main" -ne 1 ]; then
    fail_test "Expected file ex00/main.c not found"
    delivery_ok=0
fi
if [ "$has_putchar" -ne 1 ]; then
    fail_test "Expected file ex00/ft_putchar.c not found"
    delivery_ok=0
fi
if [ "${#rush_files[@]}" -eq 0 ]; then
    fail_test "No rush0X.c file found in ex00/ (expected one of rush00.c..rush04.c)"
    delivery_ok=0
fi
if [ "${#extras[@]}" -gt 0 ]; then
    fail_test "Extra file(s) found in ex00/: ${extras[*]}"
    delivery_ok=0
fi

if [ "$delivery_ok" -ne 1 ]; then
    finish_shell_tests
fi

pass_test "Delivery files valid: main.c, ft_putchar.c, ${rush_files[*]} (no extra files)"

TMP_DIR="$(mktemp -d /tmp/mini_moul_rush00.XXXXXX)"
trap 'rm -rf "$TMP_DIR"' EXIT

# Helper: check undefined external symbols against allowed list
check_allowed_syms() {
    local label="$1"
    shift
    local allowed_syms="$1"
    shift
    local objs=("$@")

    if ! command -v nm &> /dev/null; then
        return 0
    fi

    local defined_syms
    defined_syms="$(nm -g --defined-only "${objs[@]}" 2>/dev/null | awk '{print $NF}')"

    local forbidden=()
    local sym
    while IFS= read -r sym; do
        [ -z "$sym" ] && continue
        case "$sym" in
            _*) continue ;;
        esac
        if printf '%s\n' "$defined_syms" | grep -qx "$sym"; then
            continue
        fi
        local ok=0
        local af
        for af in $allowed_syms; do
            if [ "$sym" = "$af" ]; then
                ok=1
                break
            fi
        done
        if [ "$ok" -eq 0 ]; then
            forbidden+=("$sym")
        fi
    done < <(nm -u "${objs[@]}" 2>/dev/null | awk '{print $NF}' | sort -u)

    if [ "${#forbidden[@]}" -gt 0 ]; then
        fail_test "$label uses forbidden function(s): ${forbidden[*]} (allowed: write)"
        return 1
    fi
    return 0
}

# Compile ft_putchar.c standalone and verify symbol ft_putchar + allowed functions
if ! cc -Wall -Wextra -Werror -fno-stack-protector -c "$EX_DIR/ft_putchar.c" -o "$TMP_DIR/ft_putchar.o" 2>"$TMP_DIR/err.txt"; then
    fail_test "ex00/ft_putchar.c failed to compile (-Wall -Wextra -Werror)"
    finish_shell_tests
fi

if command -v nm &> /dev/null; then
    if ! nm -g --defined-only "$TMP_DIR/ft_putchar.o" 2>/dev/null | awk '{print $NF}' | grep -qx 'ft_putchar'; then
        fail_test "ex00/ft_putchar.c must define function ft_putchar"
        finish_shell_tests
    fi
fi

if check_allowed_syms "ex00/ft_putchar.c" "write" "$TMP_DIR/ft_putchar.o"; then
    pass_test "ex00/ft_putchar.c compiles cleanly and defines ft_putchar (allowed: write)"
fi

# Compile student's main.c standalone and verify allowed functions
if ! cc -Wall -Wextra -Werror -fno-stack-protector -c "$EX_DIR/main.c" -o "$TMP_DIR/main.o" 2>"$TMP_DIR/err.txt"; then
    fail_test "ex00/main.c failed to compile (-Wall -Wextra -Werror)"
    finish_shell_tests
fi

if check_allowed_syms "ex00/main.c" "write rush ft_putchar" "$TMP_DIR/main.o"; then
    pass_test "ex00/main.c compiles cleanly with only authorized functions"
fi

ASAN_FLAGS=""
if [ "${MINI_ASAN:-0}" = "1" ]; then
    ASAN_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer -g"
fi

# Write C test harness that takes variant number (0..4) and tests all PDF + edge cases
cat << 'EOF' > "$TMP_DIR/harness.c"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void	rush(int x, int y);

typedef struct s_style
{
	char	tl;
	char	tr;
	char	bl;
	char	br;
	char	h;
	char	v;
}	t_style;

static const t_style	g_styles[5] = {
	{'o', 'o', 'o', 'o', '-', '|'},
	{'/', '\\', '\\', '/', '*', '*'},
	{'A', 'A', 'C', 'C', 'B', 'B'},
	{'A', 'C', 'A', 'C', 'B', 'B'},
	{'A', 'C', 'C', 'A', 'B', 'B'}
};

static char	*build_expected(int variant, int x, int y, size_t *out_len)
{
	if (x <= 0 || y <= 0)
	{
		char *empty = calloc(1, 1);
		*out_len = 0;
		return (empty);
	}
	t_style	s = g_styles[variant];
	size_t	len = (size_t)(x + 1) * (size_t)y;
	char	*buf = malloc(len + 1);
	size_t	pos = 0;

	if (!buf)
		return (NULL);
	for (int row = 0; row < y; row++)
	{
		for (int col = 0; col < x; col++)
		{
			char	c;
			if (row == 0)
			{
				if (col == 0)
					c = s.tl;
				else if (col == x - 1)
					c = s.tr;
				else
					c = s.h;
			}
			else if (row == y - 1)
			{
				if (col == 0)
					c = s.bl;
				else if (col == x - 1)
					c = s.br;
				else
					c = s.h;
			}
			else
			{
				if (col == 0 || col == x - 1)
					c = s.v;
				else
					c = ' ';
			}
			buf[pos++] = c;
		}
		buf[pos++] = '\n';
	}
	buf[pos] = '\0';
	*out_len = pos;
	return (buf);
}

static int	run_case(int variant, int x, int y, const char *tmp_path)
{
	size_t	exp_len = 0;
	char	*expected = build_expected(variant, x, y, &exp_len);
	if (!expected)
		return (1);

	fflush(stdout);
	int	saved_stdout = dup(STDOUT_FILENO);
	int	fd = open(tmp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(expected);
		close(saved_stdout);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);

	rush(x, y);

	fflush(stdout);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);

	FILE	*fp = fopen(tmp_path, "rb");
	if (!fp)
	{
		free(expected);
		return (1);
	}
	fseek(fp, 0, SEEK_END);
	long	fsize = ftell(fp);
	rewind(fp);
	size_t	act_len = (fsize > 0) ? (size_t)fsize : 0;
	char	*actual = malloc(act_len + 1);
	if (!actual)
	{
		fclose(fp);
		free(expected);
		return (1);
	}
	if (act_len > 0)
		act_len = fread(actual, 1, act_len, fp);
	actual[act_len] = '\0';
	fclose(fp);

	int	ok = (act_len == exp_len && (exp_len == 0 || memcmp(actual, expected, exp_len) == 0));
	if (!ok)
	{
		fprintf(stderr, "MISMATCH rush(%d, %d): expected %zu bytes, got %zu bytes\n",
			x, y, exp_len, act_len);
		if (exp_len <= 128 && act_len <= 128)
		{
			fprintf(stderr, "Expected:\n%s---\nGot:\n%s---\n", expected, actual);
		}
	}
	free(actual);
	free(expected);
	return (ok ? 0 : 1);
}

int	main(int argc, char **argv)
{
	if (argc != 5)
		return (2);
	int	variant = atoi(argv[1]);
	int	x = atoi(argv[2]);
	int	y = atoi(argv[3]);
	if (strcmp(argv[2], "INT_MIN") == 0)
		x = INT_MIN;
	if (strcmp(argv[3], "INT_MIN") == 0)
		y = INT_MIN;
	return (run_case(variant, x, y, argv[4]));
}
EOF

# Test each delivered rush0X.c variant
for rf in "${rush_files[@]}"; do
    variant="${rf#rush0}"
    variant="${variant%.c}"

    # Standalone compile + symbol & allowed function check
    if ! cc -Wall -Wextra -Werror -fno-stack-protector -c "$EX_DIR/$rf" -o "$TMP_DIR/${rf%.c}.o" 2>"$TMP_DIR/err.txt"; then
        fail_test "ex00/$rf failed to compile (-Wall -Wextra -Werror)"
        continue
    fi

    if command -v nm &> /dev/null; then
        if ! nm -g --defined-only "$TMP_DIR/${rf%.c}.o" 2>/dev/null | awk '{print $NF}' | grep -qx 'rush'; then
            fail_test "ex00/$rf must define function rush(int x, int y)"
            continue
        fi
    fi

    if ! check_allowed_syms "ex00/$rf" "write ft_putchar" "$TMP_DIR/${rf%.c}.o"; then
        continue
    fi
    pass_test "ex00/$rf compiles cleanly and defines rush (allowed: write, ft_putchar)"

    # Compile and execute student's full program (main.c + ft_putchar.c + rush0X.c)
    if ! cc -Wall -Wextra -Werror $ASAN_FLAGS "$EX_DIR/main.c" "$EX_DIR/ft_putchar.c" "$EX_DIR/$rf" -o "$TMP_DIR/student_$variant" 2>"$TMP_DIR/err.txt"; then
        fail_test "Failed to compile main.c + ft_putchar.c + $rf together (-Wall -Wextra -Werror)"
        continue
    fi

    student_rc=0
    if [ "${MINI_ASAN:-0}" = "1" ]; then
        ASAN_OPTIONS="detect_leaks=1" UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1" timeout 5s "$TMP_DIR/student_$variant" > /dev/null
        student_rc=$?
    else
        timeout 5s "$TMP_DIR/student_$variant" > /dev/null
        student_rc=$?
    fi
    if [ "$student_rc" -eq 0 ]; then
        pass_test "Student binary (main.c + ft_putchar.c + $rf) compiles and exits 0"
    else
        fail_test "Student binary (main.c + ft_putchar.c + $rf) exited with code $student_rc"
    fi

    # Compile test harness with student's ft_putchar.c + rush0X.c
    if ! cc -Wall -Wextra -Werror $ASAN_FLAGS "$TMP_DIR/harness.c" "$EX_DIR/ft_putchar.c" "$EX_DIR/$rf" -o "$TMP_DIR/harness_$variant" 2>"$TMP_DIR/err.txt"; then
        fail_test "Failed to link test harness with ft_putchar.c + $rf (check void rush(int x, int y) prototype)"
        continue
    fi

    # Run all subject examples + defense test + edge cases
    cases=(
        "5 3|PDF example rush(5, 3)"
        "5 1|PDF example rush(5, 1)"
        "1 1|PDF example rush(1, 1)"
        "1 5|PDF example rush(1, 5)"
        "4 4|PDF example rush(4, 4)"
        "2 2|Smallest 2x2 corners rush(2, 2)"
        "123 42|Defense large test rush(123, 42)"
        "0 0|Zero dimensions rush(0, 0) -> empty"
        "0 5|Zero width rush(0, 5) -> empty"
        "5 0|Zero height rush(5, 0) -> empty"
        "-5 3|Negative width rush(-5, 3) -> empty"
        "3 -5|Negative height rush(3, -5) -> empty"
        "-4 -4|Both negative rush(-4, -4) -> empty"
        "INT_MIN 5|INT_MIN width rush(INT_MIN, 5) -> empty (no overflow/loop)"
        "5 INT_MIN|INT_MIN height rush(5, INT_MIN) -> empty (no overflow/loop)"
    )

    for entry in "${cases[@]}"; do
        coords="${entry%%|*}"
        desc="${entry#*|}"
        x_arg="${coords%% *}"
        y_arg="${coords#* }"

        rc=0
        if [ "${MINI_ASAN:-0}" = "1" ]; then
            ASAN_OPTIONS="detect_leaks=1" UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1" timeout 5s "$TMP_DIR/harness_$variant" "$variant" "$x_arg" "$y_arg" "$TMP_DIR/out.txt" 2>"$TMP_DIR/case_err.txt"
            rc=$?
        else
            timeout 5s "$TMP_DIR/harness_$variant" "$variant" "$x_arg" "$y_arg" "$TMP_DIR/out.txt" 2>"$TMP_DIR/case_err.txt"
            rc=$?
        fi

        if [ "$rc" -eq 0 ]; then
            pass_test "[$rf] $desc"
        elif [ "$rc" -eq 124 ]; then
            fail_test "[$rf] $desc (TIMEOUT - possible infinite loop)"
        else
            err_detail="$(head -n 1 "$TMP_DIR/case_err.txt")"
            fail_test "[$rf] $desc (${err_detail:-exit code $rc})"
        fi
    done
done

finish_shell_tests
