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
    bool may_print = true;
    bool wrapped_word = false;
    char buf[BUF_LEN];
    while (fgets(buf, BUF_LEN, input) == buf) {
        size_t start, end, newline;
        // fgets always stores '\0' at the end (man 3 fgets)
        for (newline = 0; buf[newline] != '\0' && buf[newline] != '\n';
             ++newline) {
            // Better not use `< BUF_LEN` as boundary condition,
            // since fgets buffer may be "dirty", containing relics from last
            // refill.
            // Consider if the last line is not ended with newline, being
            // shorter than the second last line, and the second shorter line
            // fits inside buffer: the newline may not be from current line.
            //
            // POSIX requires every text file to end with newline
            // but who knows if all files behave
        }
        if (buf[newline] == '\0') {
            // indicate newline not found in current line
            newline = BUF_LEN;
        }

        if (may_print) {
            start = 0;
            if (!wrapped_word) {
                // '\0' is not space (man 3 isspace)
                // fgets always stores '\0' at the end (man 3 fgets)
                for (; isspace(buf[start]); ++start)
                    ;
            }
            for (end = start; buf[end] != '\0' && !isspace(buf[end]); ++end)
                ;
            if (newline == BUF_LEN) {
#ifndef MY_LONG_VER
                if (start == end) {
                    // no print required
                    wrapped_word = false;
                    may_print = buf[end] == '\0';
                } else {
                    // print word
                    may_print = wrapped_word = buf[end] == '\0';
                    buf[end] = '\0';
                    printf("%s", buf + start);
                }
#else
                if (wrapped_word) {
                    if (end == 0) {
                        // buffer starts with space
                        // word already ended and printed
                        // no need to print and ignore the rest
                        wrapped_word = false;
                        may_print = false;
                    } else {
                        // word does wrap across buffers
                        may_print = wrapped_word = buf[end] == '\0';
                        printf("%s", buf);
                    }
                } else {
                    if (end > start) {
                        wrapped_word = may_print = buf[end] == '\0';
                        printf("%s", buf + start);
                    } else {
                        wrapped_word = false;
                        may_print = true;
                    }
                }
#endif
            } else {
                wrapped_word = false;
                may_print = true;
                buf[newline] = '\0';
                buf[end] = '\0';
                printf("%s\n", buf + start);
            }
        } else {
            if (newline != BUF_LEN) {
                putchar('\n');
                may_print = true;
                wrapped_word = false;
            }
        }
    }
    fclose(input);
    return 0;
}
