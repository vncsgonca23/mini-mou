#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../../../../ex12/ft_print_memory.c"
#include "../../../utils/constants.h"

typedef struct s_line
{
	const char	*hex;
	const char	*text;
}	t_line;

typedef struct s_test
{
	const char		*desc;
	unsigned char		src[32];
	unsigned int		size;
	t_line			lines[2];
}	t_test;

int	run_tests(t_test *tests, int count);

int	main(void)
{
	t_test tests[] = {
		{
			.desc = "size igual a zero",
			.size = 0
		},
		{
			.desc = "Um byte imprimivel",
			.src = "A",
			.size = 1,
			.lines = {{"41", "A"}}
		},
		{
			.desc = "Quantidade impar de bytes e coluna alinhada",
			.src = "ABC",
			.size = 3,
			.lines = {{"4142 43", "ABC"}}
		},
		{
			.desc = "NUL no meio, quebra de linha e bytes acima de 127",
			.src = {'A', '\0', 'B', '\n', 0x7f, 0x80, 0xff},
			.size = 7,
			.lines = {{"4100 420a 7f80 ff", "A.B...."}}
		},
		{
			.desc = "Limites do ASCII imprimivel",
			.src = {31, 32, 126, 127},
			.size = 4,
			.lines = {{"1f20 7e7f", ". ~."}}
		},
		{
			.desc = "Exatamente 16 bytes",
			.src = "0123456789ABCDEF",
			.size = 16,
			.lines = {
				{"3031 3233 3435 3637 3839 4142 4344 4546",
				 "0123456789ABCDEF"}
			}
		},
		{
			.desc = "17 bytes, com apenas um byte na segunda linha",
			.src = "0123456789ABCDEFG",
			.size = 17,
			.lines = {
				{"3031 3233 3435 3637 3839 4142 4344 4546",
				 "0123456789ABCDEF"},
				{"47", "G"}
			}
		}
	};

	return (run_tests(tests, sizeof(tests) / sizeof(tests[0])));
}

int	run_tests(t_test *tests, int count)
{
	int	failures;

	failures = 0;
	for (int i = 0; i < count; i++)
	{
		char	expected[256] = "";
		char	actual[256];
		size_t	expected_len = 0;
		size_t	actual_len;
		FILE	*output;
		int	saved_stdout;
		void	*result;

		for (unsigned int offset = 0; offset < tests[i].size; offset += 16)
		{
			t_line line = tests[i].lines[offset / 16];

			expected_len += snprintf(expected + expected_len,
				sizeof(expected) - expected_len,
				"%016" PRIxPTR ": %-39s %s\n",
				(uintptr_t)(tests[i].src + offset), line.hex, line.text);
		}

		fflush(stdout);
		output = tmpfile();
		if (output == NULL)
		{
			perror("tmpfile");
			return (1);
		}
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdout == -1 || dup2(fileno(output), STDOUT_FILENO) == -1)
		{
			perror("Redirecionar stdout");
			if (saved_stdout != -1)
				close(saved_stdout);
			fclose(output);
			return (1);
		}

		result = ft_print_memory(tests[i].src, tests[i].size);

		fflush(stdout);
		if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		{
			perror("Restaurar stdout");
			close(saved_stdout);
			fclose(output);
			return (1);
		}
		close(saved_stdout);

		rewind(output);
		actual_len = fread(actual, 1, sizeof(actual), output);
		if (ferror(output))
		{
			perror("fread");
			fclose(output);
			return (1);
		}
		fclose(output);

		if (actual_len != expected_len
			|| memcmp(actual, expected, expected_len) != 0)
		{
			printf("    " RED "[%d] %s\n" DEFAULT, i + 1, tests[i].desc);
			printf("Esperado (%zu bytes):\n%s", expected_len, expected);
			printf("Recebido (%zu bytes, ate %zu exibidos):\n",
				actual_len, sizeof(actual));
			fwrite(actual, 1, actual_len, stdout);
			fputc('\n', stdout);
			failures++;
		}
		else if (result != tests[i].src)
		{
			printf("    " RED "[%d] %s: retorno incorreto\n" DEFAULT,
				i + 1, tests[i].desc);
			failures++;
		}
		else
			printf("  " GREEN CHECKMARK GREY " [%d] %s\n" DEFAULT,
				i + 1, tests[i].desc);
	}
	return (failures != 0);
}
