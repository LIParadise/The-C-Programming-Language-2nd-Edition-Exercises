#include "lib/simple_vec.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_help_and_exit(void) {
    printf("Usage: ./<executable> <existing file>\n");
    printf("Abort.\n");
    exit(1);
}

void counter_add_1(unsigned u, vec *len_of_words, vec *freq_of_words) {
    const size_t idx = av.partition_point(len_of_words, u);
    opt_u old_cnt = av.get(len_of_words, idx);
    if (old_cnt.ok && old_cnt.u == u) {
        old_cnt = av.get(freq_of_words, idx);
        av.replace(freq_of_words, idx, old_cnt.u + 1);
    } else {
        av.insert(len_of_words, idx, u);
        av.insert(freq_of_words, idx, 1);
    }
}

void easy_print(vec *len_of_words, vec *freq_of_words) {
    for (size_t i = 0; i < av.len(len_of_words); ++i) {
        printf("word len %u occurred %u times\n", av.get(len_of_words, i).u,
               av.get(freq_of_words, i).u);
    }
}

void print(vec *len_of_words, vec *freq_of_words) {
    vec widths_of_len = av.with_capacity(av.len(len_of_words));
    const size_t column_cnt = av.len(len_of_words);
    unsigned thres = 0;
    for (size_t i = 0; i < column_cnt; ++i) {
        av.push_back(&widths_of_len,
                     snprintf(0, 0, "%u", av.get(len_of_words, i).u));
        const unsigned h = av.get(freq_of_words, i).u;
        thres = h > thres ? h : thres;
    }
    while (thres > 0) {
        for (size_t i = 0; i < column_cnt; ++i) {
            unsigned w = av.get(&widths_of_len, i).u;
            for (unsigned j = 0; j < w; ++j) {
                putchar(' ');
            }
            putchar(av.get(freq_of_words, i).u >= thres ? '*' : ' ');
            putchar(' ');
            putchar(' ');
        }
        putchar('\n');
        thres--;
    }

    for (size_t i = 0; i < column_cnt; ++i) {
        for (unsigned j = 0; j < av.get(&widths_of_len, i).u + 2; ++j) {
            putchar('=');
        }
        putchar(i + 1 == column_cnt ? '\n' : '=');
    }

    for (size_t i = 0; i < column_cnt; ++i) {
        printf(" %d ", av.get(len_of_words, i).u);
        putchar(i + 1 == column_cnt ? '\n' : '|');
    }

    av.dtor(&widths_of_len);
}

// Read from first argument,
// create histogram where x axis is word length
// y axis is count of words of that length
int main(int argc, char **argv) {
    if (argc != 2 || access(argv[1], F_OK) != 0 || access(argv[1], R_OK) != 0) {
        print_help_and_exit();
    }
    FILE *input = fopen(argv[1], "r");

    static const size_t BUF_LEN = 3;
    char buf[BUF_LEN];
    vec len_of_words = av.with_capacity(0);
    vec freq_of_words = av.with_capacity(0);
    unsigned cur_word_len = 0;
    bool wrapped = false, buf_has_newline;

    while (buf == fgets(buf, BUF_LEN, input)) {
        buf_has_newline = false;
        for (size_t i = 0; buf[i] != '\0'; ++i) {
            if (buf[i] == '\n') {
                buf_has_newline = true;
                break;
            }
        }

        size_t start = 0, end;
        if (wrapped && isspace(buf[0])) {
            counter_add_1(cur_word_len, &len_of_words, &freq_of_words);
            cur_word_len = 0;
            wrapped = false;
        }
        while (buf[start] != '\0' && buf[start] != '\n') {
            while (isspace(buf[start])) {
                start++;
            }
            for (end = start; buf[end] != '\0' && !isspace(buf[end]);
                 ++end, ++cur_word_len)
                ;
            if (cur_word_len) {
                if (buf[end] == '\0' && !buf_has_newline) {
                    wrapped = true;
                } else {
                    wrapped = false;
                    counter_add_1(cur_word_len, &len_of_words, &freq_of_words);
                    cur_word_len = 0;
                }
            } else {
                wrapped = false;
            }
            start = end;
        }
    }

    easy_print(&len_of_words, &freq_of_words);
    print(&len_of_words, &freq_of_words);

    av.dtor(&len_of_words);
    av.dtor(&freq_of_words);
    fclose(input);
}
