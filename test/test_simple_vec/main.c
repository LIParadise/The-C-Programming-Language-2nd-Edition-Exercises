#include "../../lib/simple_vec.h"

#include <assert.h>
#include <stdlib.h>

void destroy_and_exit(vec *v) {
    av.dtor(v);
    exit(1);
}

void test_push_back(void) {
    vec v = av.with_capacity(10);
    if (!av.empty(&v) || !(av.len(&v) == 0)) {
        destroy_and_exit(&v);
    }
    av.push_back(&v, 3);
    av.push_back(&v, 1);
    av.push_back(&v, 4);
    av.push_back(&v, 1);
    av.push_back(&v, 5);
    av.push_back(&v, 9);
    av.push_back(&v, 2);
    av.push_back(&v, 6);
    av.push_back(&v, 5);
    av.push_back(&v, 3);
    av.push_back(&v, 5);
    av.push_back(&v, 8);
    av.push_back(&v, 9);
    av.push_back(&v, 7);
    av.push_back(&v, 9);
    av.push_back(&v, 3);
    av.push_back(&v, 2);
    av.push_back(&v, 3);
    av.push_back(&v, 8);
    av.push_back(&v, 4);
    av.push_back(&v, 6);
    if (av.empty(&v) || !(av.len(&v) == 21)) {
        destroy_and_exit(&v);
    }

    option_unsigned opt;
    unsigned ans[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5,
                      8, 9, 7, 9, 3, 2, 3, 8, 4, 6};
    for (size_t i = 0; i < sizeof(ans) / sizeof(*ans); ++i) {
        opt = av.pop_front(&v);
        if (!(av.len(&v) == 20 - i)) {
            destroy_and_exit(&v);
        }
        if (!opt.ok || opt.u != ans[i]) {
            destroy_and_exit(&v);
        }
    }
    if (!av.empty(&v) || !(av.len(&v) == 0)) {
        destroy_and_exit(&v);
    }
    opt = av.pop_front(&v);
    if (opt.ok) {
        destroy_and_exit(&v);
    }
    opt = av.pop_back(&v);
    if (opt.ok) {
        destroy_and_exit(&v);
    }

    av.dtor(&v);
}

void test_push_front(void) {
    vec v = av.with_capacity(10);
    if (!av.empty(&v) || !(av.len(&v) == 0)) {
        destroy_and_exit(&v);
    }
    av.push_front(&v, 3);
    av.push_front(&v, 1);
    av.push_front(&v, 4);
    av.push_front(&v, 1);
    av.push_front(&v, 5);
    av.push_front(&v, 9);
    av.push_front(&v, 2);
    av.push_front(&v, 6);
    av.push_front(&v, 5);
    av.push_front(&v, 3);
    av.push_front(&v, 5);
    av.push_front(&v, 8);
    av.push_front(&v, 9);
    av.push_front(&v, 7);
    av.push_front(&v, 9);
    av.push_front(&v, 3);
    av.push_front(&v, 2);
    av.push_front(&v, 3);
    av.push_front(&v, 8);
    av.push_front(&v, 4);
    av.push_front(&v, 6);
    if (av.empty(&v) || !(av.len(&v) == 21)) {
        destroy_and_exit(&v);
    }

    option_unsigned opt;
    unsigned ans[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5,
                      8, 9, 7, 9, 3, 2, 3, 8, 4, 6};
    for (size_t i = 0; i < sizeof(ans) / sizeof(*ans); ++i) {
        opt = av.pop_back(&v);
        if (!(av.len(&v) == 20 - i)) {
            destroy_and_exit(&v);
        }
        if (!opt.ok || opt.u != ans[i]) {
            destroy_and_exit(&v);
        }
    }
    if (!av.empty(&v) || !(av.len(&v) == 0)) {
        destroy_and_exit(&v);
    }
    opt = av.pop_front(&v);
    if (opt.ok) {
        destroy_and_exit(&v);
    }
    opt = av.pop_back(&v);
    if (opt.ok) {
        destroy_and_exit(&v);
    }

    av.dtor(&v);
}

void test_insert_erase(void) {
    vec v = av.with_capacity(0);
    av.insert(&v, 0, 6);
    assert(av.len(&v) == 1);
    av.insert(&v, 1, 10);
    assert(av.len(&v) == 2);
    av.insert(&v, 0, 4);
    assert(av.len(&v) == 3);
    av.insert(&v, 1, 5);
    assert(av.len(&v) == 4);
    av.insert(&v, 3, 8);
    assert(av.len(&v) == 5);
    av.insert(&v, 3, 7);
    av.erase(&v, 3);
    av.insert(&v, 3, 7);
    assert(av.len(&v) == 6);
    av.insert(&v, 5, 9);
    assert(av.len(&v) == 7);
    av.insert(&v, 0, 3);
    assert(av.len(&v) == 8);
    av.insert(&v, 0, 2);
    assert(av.len(&v) == 9);
    av.insert(&v, 0, 1);
    assert(av.len(&v) == 10);
    for (unsigned i = 1; i <= 10; ++i) {
        option_unsigned opt = av.get(&v, i - 1);
        assert(opt.ok && i == opt.u);
    }
    while (av.pop_front(&v).ok)
        ;
    assert(av.empty(&v));

    assert(!av.erase(&v, 0).ok);
    av.insert(&v, 0, 16);
    assert(av.len(&v) == 1);
    av.insert(&v, 1, 20);
    assert(av.len(&v) == 2);
    av.insert(&v, 0, 14);
    assert(av.len(&v) == 3);
    av.insert(&v, 1, 15);
    assert(av.len(&v) == 4);
    av.insert(&v, 3, 18);
    assert(av.len(&v) == 5);
    av.insert(&v, 3, 17);
    av.erase(&v, 3);
    av.insert(&v, 3, 17);
    assert(av.len(&v) == 6);
    av.insert(&v, 5, 19);
    assert(av.len(&v) == 7);
    av.insert(&v, 0, 13);
    assert(av.len(&v) == 8);
    av.insert(&v, 0, 12);
    assert(av.len(&v) == 9);
    av.insert(&v, 0, 11);
    assert(av.len(&v) == 10);
    vec u = av.clone(&v);
    for (unsigned i = 1; i <= 10; ++i) {
        option_size_t opt = av.find(&u, i + 10);
        assert(opt.ok && i - 1 == opt.s);
    }
    while (av.pop_front(&u).ok)
        ;
    assert(av.empty(&u));
    av.dtor(&v);
    av.dtor(&u);
}

void test_partition_point(void) {
    vec v = av.with_capacity(0);
    assert(av.partition_point(&v, 0) == 0);
    assert(av.partition_point(&v, 2) == 0);

    // [2]
    av.insert(&v, 0, 2);
    assert(av.partition_point(&v, 0) == 0);
    assert(av.partition_point(&v, 1) == 0);
    assert(av.partition_point(&v, 2) == 0);
    assert(av.partition_point(&v, 3) == 1);

    // [1, 2]
    av.insert(&v, 0, 1);
    assert(av.partition_point(&v, 0) == 0);
    assert(av.partition_point(&v, 1) == 0);
    assert(av.partition_point(&v, 2) == 1);
    assert(av.partition_point(&v, 3) == 2);

    // [1, 2, 2]
    av.insert(&v, 1, 2);
    assert(av.partition_point(&v, 0) == 0);
    assert(av.partition_point(&v, 1) == 0);
    assert(av.partition_point(&v, 2) == 1);
    assert(av.partition_point(&v, 3) == 3);

    // [2, 2]
    av.erase(&v, 0);
    assert(av.partition_point(&v, 0) == 0);
    assert(av.partition_point(&v, 1) == 0);
    assert(av.partition_point(&v, 2) == 0);
    assert(av.partition_point(&v, 3) == 2);

    // [2, 2, 5, 7, 9]
    av.insert(&v, 2, 9);
    av.insert(&v, 2, 7);
    av.insert(&v, 2, 5);
    assert(av.partition_point(&v, 0) == 0);
    assert(av.partition_point(&v, 1) == 0);
    assert(av.partition_point(&v, 2) == 0);
    assert(av.partition_point(&v, 3) == 2);
    assert(av.partition_point(&v, 4) == 2);
    assert(av.partition_point(&v, 5) == 2);
    assert(av.partition_point(&v, 6) == 3);
    assert(av.partition_point(&v, 7) == 3);
    assert(av.partition_point(&v, 8) == 4);
    assert(av.partition_point(&v, 9) == 4);
    assert(av.partition_point(&v, 10) == 5);

    av.insert(&v, av.partition_point(&v, 8), 8);
    av.insert(&v, av.partition_point(&v, 3), 3);
    av.insert(&v, av.partition_point(&v, 10), 10);
    av.insert(&v, av.partition_point(&v, 0), 0);
    av.insert(&v, av.partition_point(&v, 0), 0);
    av.insert(&v, av.partition_point(&v, 1), 1);
    av.insert(&v, av.partition_point(&v, 6), 6);
    av.insert(&v, av.partition_point(&v, 42), 42);
    av.insert(&v, av.partition_point(&v, 19), 19);
    av.insert(&v, av.partition_point(&v, 4), 4);
    av.insert(&v, av.partition_point(&v, 13), 13);
    av.insert(&v, av.partition_point(&v, 14), 14);
    av.insert(&v, av.partition_point(&v, 2), 2);
    av.insert(&v, av.partition_point(&v, 5), 5);
    av.insert(&v, av.partition_point(&v, 6), 6);
    av.insert(&v, av.partition_point(&v, 69), 69);
    av.insert(&v, av.partition_point(&v, 1), 1);
    av.insert(&v, av.partition_point(&v, 1), 1);
    av.insert(&v, av.partition_point(&v, 1), 1);
    av.insert(&v, av.partition_point(&v, 1), 1);
    av.insert(&v, av.partition_point(&v, 1), 1);
    av.insert(&v, av.partition_point(&v, 1), 1);
    av.insert(&v, av.partition_point(&v, 1), 1);
    av.insert(&v, av.partition_point(&v, 0), 0);
    av.insert(&v, av.partition_point(&v, 42), 42);
    av.insert(&v, av.partition_point(&v, 42), 42);
    av.insert(&v, av.partition_point(&v, 20), 20);
    av.insert(&v, av.partition_point(&v, 20), 20);
    av.insert(&v, av.partition_point(&v, 37), 37);
    av.insert(&v, av.partition_point(&v, 37), 37);
    unsigned ans[] = {0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1, 2,
                      2,  2,  3,  4,  5,  5,  6,  6,  7,  8,  9, 10,
                      13, 14, 19, 20, 20, 37, 37, 42, 42, 42, 69};
    vec u = av.clone(&v);
    for (size_t i = 0; i < sizeof(ans) / sizeof(*ans); ++i) {
        option_unsigned opt = av.pop_front(&u);
        assert(opt.ok && opt.u == ans[i]);
    }

    av.dtor(&v);
    av.dtor(&u);
}

int main(void) {
    test_push_back();
    test_push_front();
    test_insert_erase();
    test_partition_point();
    return 0;
}
