#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/../../../utils/shell_helpers.sh"

EX_DIR="../ex01"

if ! check_delivery_file "$EX_DIR" "Makefile"; then
    finish_shell_tests
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

cp "$EX_DIR/Makefile" "$TMP_DIR/Makefile"
mkdir -p "$TMP_DIR/srcs" "$TMP_DIR/includes"

# Provide standard subject files in srcs/ and includes/
cat <<'EOF' > "$TMP_DIR/includes/ft.h"
#ifndef FT_H
# define FT_H
void	ft_putchar(char c);
void	ft_swap(int *a, int *b);
void	ft_putstr(char *str);
int		ft_strlen(char *str);
int		ft_strcmp(char *s1, char *s2);
#endif
EOF

cat <<'EOF' > "$TMP_DIR/srcs/ft_putchar.c"
#include <unistd.h>
#include "ft.h"
void	ft_putchar(char c) { write(1, &c, 1); }
EOF

cat <<'EOF' > "$TMP_DIR/srcs/ft_swap.c"
#include "ft.h"
void	ft_swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }
EOF

cat <<'EOF' > "$TMP_DIR/srcs/ft_putstr.c"
#include <unistd.h>
#include "ft.h"
void	ft_putstr(char *str) { while (*str) write(1, str++, 1); }
EOF

cat <<'EOF' > "$TMP_DIR/srcs/ft_strlen.c"
#include "ft.h"
int		ft_strlen(char *str) { int i = 0; while (str[i]) i++; return (i); }
EOF

cat <<'EOF' > "$TMP_DIR/srcs/ft_strcmp.c"
#include "ft.h"
int		ft_strcmp(char *s1, char *s2) {
	while (*s1 && *s1 == *s2) { s1++; s2++; }
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
EOF

# 1. Default 'make' (equivalent to make all)
if (cd "$TMP_DIR" && make > make1.log 2>&1); then
    pass_test "'make' succeeded"
else
    fail_test "'make' failed"
    finish_shell_tests
fi

# Check flags -Wall -Wextra -Werror in order and command echo
if grep -qE 'cc[[:space:]].*-Wall[[:space:]]+-Wextra[[:space:]]+-Werror' "$TMP_DIR/make1.log"; then
    pass_test "Uses cc with -Wall -Wextra -Werror in order and echoes commands"
else
    fail_test "Did not observe 'cc ... -Wall -Wextra -Werror' in make output"
fi

# Check .o files next to .c files and libft.a at root
objs_ok=1
for f in ft_putchar ft_swap ft_putstr ft_strlen ft_strcmp; do
    [ -f "$TMP_DIR/srcs/${f}.o" ] || objs_ok=0
done
if [ "$objs_ok" -eq 1 ] && [ -f "$TMP_DIR/libft.a" ]; then
    pass_test "Object files created in srcs/*.o and libft.a created at root"
else
    fail_test "Missing srcs/*.o or root libft.a after 'make'"
fi

# 2. Relink check: second 'make' must NOT recompile anything
(cd "$TMP_DIR" && make > make2.log 2>&1)
if grep -qE '(^|[[:space:]])(cc|ar)[[:space:]]' "$TMP_DIR/make2.log"; then
    fail_test "Second 'make' unnecessarily recompiled or relinked"
else
    pass_test "Second 'make' does not relink unnecessarily"
fi

# 3. Verify libft.a actually links and works
cat <<'EOF' > "$TMP_DIR/main.c"
#include "ft.h"
int	main(void) {
	int a = 1, b = 2;
	ft_swap(&a, &b);
	return (a == 2 && b == 1 && ft_strlen("42") == 2 && ft_strcmp("a", "b") < 0) ? 0 : 1;
}
EOF
if (cd "$TMP_DIR" && cc -Wall -Wextra -Werror -Iincludes main.c -L. -lft -o test_lib 2>/dev/null && ./test_lib); then
    pass_test "Generated libft.a links and passes runtime check"
else
    fail_test "Generated libft.a failed link/runtime check"
fi
rm -f "$TMP_DIR/main.c" "$TMP_DIR/test_lib"

# 4. make clean: removes .o, keeps libft.a
if (cd "$TMP_DIR" && make clean >/dev/null 2>&1) \
    && [ ! -f "$TMP_DIR/srcs/ft_putchar.o" ] \
    && [ -f "$TMP_DIR/libft.a" ]; then
    pass_test "'make clean' removes srcs/*.o and preserves libft.a"
else
    fail_test "'make clean' failed to remove srcs/*.o or deleted libft.a"
fi

# 5. make fclean: removes .o and libft.a
if (cd "$TMP_DIR" && make fclean >/dev/null 2>&1) \
    && [ ! -f "$TMP_DIR/srcs/ft_putchar.o" ] \
    && [ ! -f "$TMP_DIR/libft.a" ]; then
    pass_test "'make fclean' removes srcs/*.o and libft.a"
else
    fail_test "'make fclean' failed to remove libft.a"
fi

# 6. make all & make re
if (cd "$TMP_DIR" && make all >/dev/null 2>&1) && [ -f "$TMP_DIR/libft.a" ]; then
    pass_test "'make all' builds libft.a"
else
    fail_test "'make all' failed"
fi

if (cd "$TMP_DIR" && make re >/dev/null 2>&1) && [ -f "$TMP_DIR/libft.a" ] && [ -f "$TMP_DIR/srcs/ft_strcmp.o" ]; then
    pass_test "'make re' rebuilds libft.a from scratch"
else
    fail_test "'make re' failed"
fi

finish_shell_tests
