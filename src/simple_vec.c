#include "../lib/simple_vec.h"

#include <string.h>

static const size_t MIN_CAP = 32;

static vec with_capacity(const size_t s) {
    size_t cap = s < MIN_CAP / 2 ? MIN_CAP : 2 * s;
    vec ret = {
        .cap = cap,
        .len = 0,
        .arr = (unsigned *)malloc(sizeof(unsigned) * cap),
    };
    return ret;
}

static void dtor(vec *v) {
    if (v->arr) {
        free(v->arr);
    }
    v->arr = NULL;
    v->cap = MIN_CAP;
    v->len = 0;
}

static size_t len(const vec *v) { return v->len; }

static bool empty(const vec *v) { return len(v) == 0; }

static option get(const vec *v, const size_t idx) {
    bool ok = idx < len(v);
    option ret = {.ok = ok, .u = ok ? v->arr[idx] : 0};
    return ret;
}

static option replace(vec *v, const size_t idx, const unsigned u) {
    option ret = get(v, idx);
    if (ret.ok) {
        v->arr[idx] = u;
    }
    return ret;
}

static void double_cap_if_required(vec *v) {
    if (len(v) == v->cap) {
        v->cap *= 2;
        v->arr = realloc(v->arr, v->cap);
    }
}

static bool insert(vec *v, const size_t idx, const unsigned u) {
    if (idx > len(v)) {
        return false;
    } else {
        double_cap_if_required(v);
        for (size_t j = len(v); j > idx; --j) {
            v->arr[j] = v->arr[j - 1];
        }
        v->arr[idx] = u;
        v->len++;
        return true;
    }
}

static option erase(vec *v, const size_t idx) {
    option ret = get(v, idx);
    if (ret.ok) {
        for (size_t j = idx + 1; j < len(v); ++j) {
            v->arr[j - 1] = v->arr[j];
        }
        v->len--;
    }
    return ret;
}

static void push_back(vec *v, const unsigned u) { insert(v, len(v), u); }

static void push_front(vec *v, const unsigned u) { insert(v, 0, u); }

static option pop_back(vec *v) {
    return empty(v) ? (option){.ok = false, .u = 0} : erase(v, v->len - 1);
}

static option pop_front(vec *v) { return erase(v, 0); }

static vec clone(const vec *v) {
    vec ret = {.cap = v->cap,
               .len = v->len,
               .arr = (unsigned *)malloc(sizeof(unsigned) * v->cap)};
    memcpy(ret.arr, v->arr, sizeof(unsigned) * len(v));
    return ret;
}

const access_vec av = {
    .with_capacity = with_capacity,
    .dtor = dtor,
    .len = len,
    .empty = empty,
    .get = get,
    .replace = replace,
    .insert = insert,
    .erase = erase,
    .push_back = push_back,
    .push_front = push_front,
    .pop_back = pop_back,
    .pop_front = pop_front,
    .clone = clone,
};
