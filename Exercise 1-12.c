////////////////////////////////////// test
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
    static const size_t BUF_LEN = 2;
    bool word_span_across_lines = false;
    bool skip_remaining_parts = false;
    for (char buf[BUF_LEN]; fgets(buf, BUF_LEN, input) == buf;) {
        size_t start, end, newline;
        for (newline = 0; newline < BUF_LEN && buf[newline] != '\n'; ++newline)
            ;
        if (skip_remaining_parts) {
            // A really long line spanning across multiple buffer refills,
            // and we've printed the first word.
            // Ignore all till newline is found
            if (newline != BUF_LEN) {
                skip_remaining_parts = false;
                putchar('\n');
            }
        } else {
            // Probably have something to print...

            if (!word_span_across_lines) {
                // basic case
                // search for word start and end, print if found
                //
                // Case A. the buf contains newline (line ends within buf)
                // Case B. the buf contains no newline (line longer than buf)
                //
                // For case A., just print the word and newline.
                //
                // For case B., either it's space all the way down, in which
                // case we print nothing, or there's some non space characters
                // that form a word which may or may not extend over the next
                // buffer refill

                // '\0' is always appended at the end by fgets (man 3 fgets)
                // '\0' is not a space (man 3 isspace)
                // hence no boundary check required
                for (start = 0; isspace(buf[start]); ++start)
                    ;
                for (end = start; buf[end] != '\0' && !isspace(buf[end]); ++end)
                    ;

                if (newline != BUF_LEN) {
                    // case A
                    buf[end] = 0;
                    buf[newline] = 0;
                    printf("%s\n", buf + start);
                } else {
                    // case B
                    if (buf[start] == '\0') {
                        // space all the way down
                        // ignore it
                    } else {
                        // a word to print
                        // determine either the word ends in this buffer or not.
                        // if word end within this buffer, skip remaining parts
                        word_span_across_lines = buf[end] == '\0';
                        skip_remaining_parts = buf[end] != '\0';
                        buf[end] = '\0';
                        printf("%s", buf + start);
                    }
                }
            } else {
                // first word span across buffers
                //
                // Case A. line doesn't end in this buffer
                // Case B. line ends in this buffer
                //
                // Case A.a neither does the word end in the buffer
                // Case A.b word ends but line continues
                for (end = 0; buf[end] != '\0' && !isspace(buf[end]); ++end)
                    ;
                if (newline == BUF_LEN) {
                    // Case A
                    word_span_across_lines = buf[end] == '\0';
                    skip_remaining_parts = buf[end] != '\0';
                    buf[end] = '\0';
                    printf("%s", buf);
                } else {
                    // Case B.
                    word_span_across_lines = false;
                    buf[end] = 0;
                    buf[newline] = 0;
                    printf("%s\n", buf);
                }
            }
        }
    }
    fclose(input);
    return 0;
}
