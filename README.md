# Mini-mou

![Mini-moulinette](mini-moulinette.jpg)

Mini-mou runs automated tests for 42 Piscine exercises. Run it from your
assignment directory to check your work before submission. The runner and
exercise suites work independently of the subject PDFs.

## How Does It Work?

![screenshot](screenshot.jpg)

- Mini moulinette runs through all the tests cases automatically, and checks if the test conditions are met.
- It will then output the result.
- The scores are based on 42's practice, where if easier/earlier questions are incorrect, the rest won't count.


## Updating

```bash
git -C ~/mini-mou pull --ff-only
```


## Get started

### 1. Clone the repository

```bash
git clone https://github.com/vncsgonca23/mini-mou.git "$HOME/mini-mou"
```

### 2. Create an alias for your shell

Run only the block for the shell you use. Each block saves the alias for future
sessions.

**Bash**

```bash
printf '%s\n' "alias mini='\"\$HOME/mini-mou/mini-moul.sh\"'" >> ~/.bashrc
source ~/.bashrc
```

**Zsh**

```zsh
printf '%s\n' "alias mini='\"\$HOME/mini-mou/mini-moul.sh\"'" >> "${ZDOTDIR:-$HOME}/.zshrc"
source "${ZDOTDIR:-$HOME}/.zshrc"
```

**Fish**

```fish
alias --save mini '"$HOME/mini-mou/mini-moul.sh"'
```

### 3. Run the tests

Change to the directory that contains your exercise folders (`ex00`, `ex01`,
and so on). Replace the example path with your own.

```bash
cd /path/to/assignment/C02
mini C02
```

Use the corresponding assignment name for Shell00, Shell01, Rush00, or C00–C13.

> The local suites cover the exercises listed below. The official evaluator may check additional cases.

To enable the sanitizers, run:

```bash
MINI_ASAN=1 mini C02
```

## Debugging

The error/success messages should be explicit enough. However sometimes you will get segmentation fault or your code doesn't compile

### If your code doesn't compile

- Check the headers
- Check if it contains main inside
- Check if your function name is similar to a function from standard library, if it is then it will have problem

### If you receive segmentation fault

- You need to find the test cases, go here:

```bash
cd ~/mini-mou/mini-moul/tests
```

The current directory contains all the test cases. Every test is in the file that is the same name as the function/program it is testing, e.g. C05/ex00/ft_iterative_factorial.c will contain test for C05 > ex00 > ft_iterative_factorial.c.

The test cases are usually put in a list of array in this structure, e.g. for ft_iterative_factorial test:

``` C
 t_test tests[] = {
        {
            .desc = "Factorial of 0",
            .n = 0,
            .expected = 1,
        },
        {
            .desc = "Factorial of 1",
            .n = 1,
            .expected = 1,
        },
        {
            .desc = "Factorial of 2",
            .n = 2,
            .expected = 2,
        },
        {
            .desc = "Factorial of 10",
            .n = 10,
            .expected = 3628800,
        },
        {
            .desc = "Factorial of a negative number",
            .n = -5,
            .expected = 0,
        },
 }
```


## Customizing

See the above, you can add more tests here. You need to manage your customization yourself when you pull for updates though.


## Status

| Assignment            | Coverage                   | Remarks         |
| :-------------------- | :------------------------: | :-------------- |
| Shell00               | 10/10                      | ex00–ex09 (permissions, timestamps, git, tar, magic file, etc.) |
| Shell01               | 8/8                        | ex01–ex08 (ex00 is an exam reminder in the subject) |
| Rush00                | 1/1                        | Auto-detects `rush00.c`..`rush04.c` (incl. multi-variant bonus), defense `(123, 42)` & edge cases |
| C00                   | 9/9                        | ex00–ex08 (deliverables, allowed functions, `INT_MIN`/`INT_MAX` & edge cases) |
| C01                   | 9/9                        | ex00–ex08 |
| C02                   | 13/13                      | ex00–ex12 |
| C03                   | 6/6                        | ex00–ex05 |
| C04                   | 6/6                        | ex00–ex05 (`INT_MIN`, invalid bases, signs/whitespaces) |
| C05                   | 9/9                        | ex00–ex08 (incl. `ft_ten_queens_puzzle` & overflow/performance checks) |
| C06                   | 4/4                        | ex00–ex03 (program binaries, ASCII sorting, edge args) |
| C07                   | 6/6                        | ex00–ex05 (`malloc` checks, `MINI_ASAN=1` compatible) |
| C08                   | 6/6                        | ex00–ex05 (headers, macro parentheses/side-effects, `ft_strs_to_tab` & `ft_show_tab`) |
| C09                   | 3/3                        | ex00 (`libft_creator.sh`), ex01 (`Makefile` rules/relink), ex02 (`ft_split`) |
| C10                   | 4/4                        | ex00–ex03 (`ft_display_file`, `ft_cat`, `ft_tail`, `ft_hexdump` + `Makefile`) |
| C11                   | 8/8                        | ex00–ex07 (incl. `do-op` + `Makefile`) |
| C12                   | 18/18                      | ex00–ex17 (linked lists, `ft_list.h`, `MINI_ASAN=1` compatible) |
| C13                   | 8/8                        | ex00–ex07 (binary trees, `ft_btree.h`, `MINI_ASAN=1` compatible) |


## Contributing

Open an issue or pull request with the module, exercise number, input, and
expected behavior. Test sources are under [mini-moul/tests](mini-moul/tests).

## License

See [LICENSE](LICENSE) for the test runner's MIT license.
