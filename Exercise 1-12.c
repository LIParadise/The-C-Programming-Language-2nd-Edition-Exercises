#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_help_and_exit(void) {
    printf("Usage: ./<executable> <existing file>\n");
    printf("Abort.\n");
    exit(1);
}

// output only first word of each line
int main(int argc, char **argv) {
    if (argc != 2 || access(argv[1], F_OK) != 0 || access(argv[1], R_OK) != 0) {
        print_help_and_exit();
    }
    FILE *input = fopen(argv[1], "r");
    static const size_t BUF_LEN = 3;
    char buf[BUF_LEN];
    size_t start, end;
    bool print_newline_upon_space = false, buf_has_newline = false,
         line_has_word = false;
    while (fgets(buf, BUF_LEN, input) == buf) {
        buf_has_newline = false;
        for (size_t i = 0; buf[i] != '\0'; ++i) {
            if (buf[i] == '\n') {
                buf_has_newline = true;
                break;
            }
        }
        start = 0;
        if (print_newline_upon_space && isspace(buf[start])) {
            putchar('\n');
            print_newline_upon_space = false;
        }
        while (start < BUF_LEN) {
            while (isspace(buf[start])) {
                ++start;
            }
            end = start;
            while (buf[end] != '\0' && !isspace(buf[end])) {
                ++end;
            }
            if (end > start) {
                line_has_word = true;
                if (isspace(buf[end])) {
                    buf[end] = '\0';
                    printf("%s\n", buf + start);
                    print_newline_upon_space = false;
                } else if (buf[end] == '\0') {
                    printf("%s", buf + start);
                    print_newline_upon_space = true;
                }
            }
            start = end + 1;
        }
        if (buf_has_newline) {
            print_newline_upon_space = false;
            if (!line_has_word) {
                putchar('\n');
            }
            line_has_word = false;
        }
    }
    fclose(input);
    return 0;
}
