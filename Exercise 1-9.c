#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_help_and_exit(void) {
    printf("Usage: ./<executable> <existing file> <new file>\n");
    printf("Abort.\n");
    exit(1);
}

// Read from first argument,
// line by line, make any contiguous 1 or more spaces to 1 space,
// put them to second argument.
// The second argument has to be a new file.
int main(int argc, char **argv) {
    if (argc != 3 || access(argv[2], F_OK) == 0) {
        print_help_and_exit();
    }
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w+");
    static const size_t BUF_LEN = 256;
    for (char buf[BUF_LEN]; fgets(buf, BUF_LEN, input) == buf;) {
        for (size_t i = 0; i < BUF_LEN; ++i) {
            if (buf[i] == '\n') {
                break;
            }
            if (isspace(buf[i])) {
                size_t j = i;
                while (j < BUF_LEN && isspace(buf[j])) {
                    j += 1;
                }
                if (j > i + 1) {
                    // contiguous multiple spaces
                    for (size_t k = 0; j + k < BUF_LEN; ++k) {
                        buf[i + 1 + k] = buf[j + k];
                    }
                }
                i = j;
            }
        }
        fputs(buf, output);
    }
    fclose(input);
    fclose(output);
    return 0;
}
