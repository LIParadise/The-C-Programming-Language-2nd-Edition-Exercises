#ifndef SIMPLE_VEC
#define SIMPLE_VEC

#include <stdbool.h>
#include <stdlib.h>

typedef struct vec {
        size_t cap;
        size_t len;
        unsigned *arr;
} vec;

typedef struct option_unsigned {
        bool ok;
        unsigned u;
} option_unsigned;

typedef struct option_size_t {
        bool ok;
        size_t s;
} option_size_t;

typedef struct access_vec {
        vec (*with_capacity)(size_t);
        void (*dtor)(vec *);
        size_t (*len)(const vec *);
        bool (*empty)(const vec *);
        option_unsigned (*get)(const vec *, const size_t);
        option_unsigned (*replace)(vec *, const size_t, const unsigned);
        bool (*insert)(vec *, const size_t, const unsigned);
        option_unsigned (*erase)(vec *, const size_t);
        void (*push_back)(vec *, const unsigned);
        void (*push_front)(vec *, const unsigned);
        option_unsigned (*pop_back)(vec *);
        option_unsigned (*pop_front)(vec *);
        vec (*clone)(const vec *);
        option_size_t (*find)(const vec*, const unsigned);
        size_t (*partition_point)(const vec*, const unsigned);
} access_vec;

extern const access_vec av;

#endif
