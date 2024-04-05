#ifndef SIMPLE_VEC
#define SIMPLE_VEC

#include <stdbool.h>
#include <stdlib.h>

typedef struct vec {
        size_t cap;
        size_t len;
        unsigned *arr;
} vec;

typedef struct option {
        bool ok;
        unsigned u;
} option;

typedef struct access_vec {
        vec (*with_capacity)(size_t);
        void (*dtor)(vec *);
        size_t (*len)(const vec *);
        bool (*empty)(const vec *);
        option (*get)(const vec *, const size_t);
        option (*replace)(vec *, const size_t, const unsigned);
        bool (*insert)(vec *, const size_t, const unsigned);
        void (*push_back)(vec *, const unsigned);
        void (*push_front)(vec *, const unsigned);
        option (*pop_back)(vec *);
        option (*pop_front)(vec *);
} access_vec;

extern const access_vec av;

#endif
