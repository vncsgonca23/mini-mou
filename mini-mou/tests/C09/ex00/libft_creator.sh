#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex00"
REQUIRED=(libft_creator.sh ft_putchar.c ft_swap.c ft_putstr.c ft_strlen.c ft_strcmp.c)

if [ ! -d "$EX_DIR" ]; then
    fail_test "Delivery directory ex00/ does not exist"
    finish_shell_tests
fi

# Verify all required files exist and no extra files exist
missing=()
for f in "${REQUIRED[@]}"; do
    [ -f "$EX_DIR/$f" ] || missing+=("$f")
done
if [ "${#missing[@]}" -gt 0 ]; then
    fail_test "Missing file(s) in ex00/: ${missing[*]}"
    finish_shell_tests
fi

extras=()
while IFS= read -r -d '' entry; do
    base="$(basename "$entry")"
    found=0
    for f in "${REQUIRED[@]}"; do
        [ "$base" = "$f" ] && found=1 && break
    done
    [ "$found" -eq 0 ] && extras+=("$base")
done < <(find "$EX_DIR" -mindepth 1 -maxdepth 1 -print0)

if [ "${#extras[@]}" -gt 0 ]; then
    fail_test "Extra file(s) in ex00/: ${extras[*]}"
    finish_shell_tests
else
    pass_test "All 6 required files present in ex00/ with no extra files"
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT
cp "$EX_DIR"/* "$TMP_DIR/"

# Check allowed functions (only write) on each .c file
if command -v nm &> /dev/null; then
    for cf in ft_putchar.c ft_swap.c ft_putstr.c ft_strlen.c ft_strcmp.c; do
        if ! cc -Wall -Wextra -Werror -fno-stack-protector -c "$TMP_DIR/$cf" -o "$TMP_DIR/${cf%.c}.o" 2>/dev/null; then
            fail_test "$cf failed compilation with -Wall -Wextra -Werror"
            finish_shell_tests
        fi
        bad_syms="$(nm -u "$TMP_DIR/${cf%.c}.o" 2>/dev/null | awk '{print $NF}' | grep -vE '^_|^write$' || true)"
        if [ -n "$bad_syms" ]; then
            fail_test "Forbidden function(s) in $cf: $bad_syms (allowed: write)"
            finish_shell_tests
        fi
        rm -f "$TMP_DIR/${cf%.c}.o"
    done
    pass_test "All .c files compile cleanly and use only allowed functions"
fi

# Run sh libft_creator.sh as specified in the subject
if (cd "$TMP_DIR" && /bin/sh libft_creator.sh >/dev/null 2>&1); then
    pass_test "sh libft_creator.sh exited with status 0"
else
    fail_test "sh libft_creator.sh failed"
    finish_shell_tests
fi

if [ -f "$TMP_DIR/libft.a" ]; then
    pass_test "libft.a was created"
else
    fail_test "libft.a was not created by libft_creator.sh"
    finish_shell_tests
fi

# Build and run a C harness linking against libft.a
cat <<'EOF' > "$TMP_DIR/main.c"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void	ft_putchar(char c);
void	ft_swap(int *a, int *b);
void	ft_putstr(char *str);
int		ft_strlen(char *str);
int		ft_strcmp(char *s1, char *s2);

int	main(void)
{
	int	a = 10, b = 42;
	ft_swap(&a, &b);
	if (a != 42 || b != 10)
		return (1);
	if (ft_strlen("") != 0 || ft_strlen("hello") != 5)
		return (2);
	if (ft_strcmp("abc", "abc") != 0 || ft_strcmp("ab", "ac") >= 0 || ft_strcmp("ac", "ab") <= 0)
		return (3);
	int	saved = dup(STDOUT_FILENO);
	int	fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	ft_putchar('4');
	ft_putstr("2!");
	fflush(stdout);
	dup2(saved, STDOUT_FILENO);
	close(saved);
	char	buf[16] = {0};
	FILE	*fp = fopen("out.txt", "rb");
	size_t	len = fread(buf, 1, sizeof(buf) - 1, fp);
	fclose(fp);
	if (len != 3 || memcmp(buf, "42!", 3) != 0)
		return (4);
	return (0);
}
EOF

if (cd "$TMP_DIR" && cc -Wall -Wextra -Werror main.c -L. -lft -o test_lib 2>/dev/null); then
    pass_test "Linked test program against libft.a"
else
    fail_test "Failed to link against libft.a (missing symbols or invalid archive)"
    finish_shell_tests
fi

if (cd "$TMP_DIR" && ./test_lib); then
    pass_test "All 5 functions (ft_putchar, ft_swap, ft_putstr, ft_strlen, ft_strcmp) work correctly"
else
    fail_test "Runtime test against libft.a failed"
fi

finish_shell_tests
