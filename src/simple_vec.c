#include "../lib/simple_vec.h"

#include <string.h>

#define FIELD_SIZEOF(t, f)              (sizeof(((t *)0)->f))
#define FIELD_PTR_POINT_TO_SIZEOF(t, f) (sizeof(*((t *)0)->f))

static const size_t MIN_CAP = 32;

static vec with_capacity(const size_t s) {
    size_t cap = s < MIN_CAP / 2 ? MIN_CAP : 2 * s;
    vec ret = {
        .cap = cap,
        .len = 0,
        .arr = malloc(FIELD_PTR_POINT_TO_SIZEOF(vec, arr) * cap),
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

static option_unsigned get(const vec *v, const size_t idx) {
    bool ok = idx < len(v);
    option_unsigned ret = {.ok = ok, .u = ok ? v->arr[idx] : 0};
    return ret;
}

static option_unsigned replace(vec *v, const size_t idx, const unsigned u) {
    option_unsigned ret = get(v, idx);
    if (ret.ok) {
        v->arr[idx] = u;
    }
    return ret;
}

static void double_cap_if_required(vec *v) {
    if (len(v) == v->cap) {
        v->cap *= 2;
        v->arr = realloc(v->arr, FIELD_PTR_POINT_TO_SIZEOF(vec, arr) * v->cap);
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

static option_unsigned erase(vec *v, const size_t idx) {
    option_unsigned ret = get(v, idx);
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

static option_unsigned pop_back(vec *v) {
    return empty(v) ? (option_unsigned){.ok = false, .u = 0}
                    : erase(v, v->len - 1);
}

static option_unsigned pop_front(vec *v) { return erase(v, 0); }

static vec clone(const vec *v) {
    vec ret = {.cap = v->cap,
               .len = len(v),
               .arr = malloc(FIELD_PTR_POINT_TO_SIZEOF(vec, arr) * v->cap)};
    memcpy(ret.arr, v->arr, FIELD_PTR_POINT_TO_SIZEOF(vec, arr) * len(v));
    return ret;
}

static option_size_t find(const vec *v, const unsigned u) {
    option_size_t ret = {.ok = false, .s = 0};
    for (size_t i = 0; i < len(v); ++i) {
        if (get(v, i).ok && get(v, i).u == u) {
            ret.ok = true;
            ret.s = i;
            break;
        }
    }
    return ret;
}

// [Rust](https://doc.rust-lang.org/std/primitive.slice.html#method.partition_point)
//
// Input criteria: "partitioned according to the given predicate"
// here it means sorted in non-decreasing order
//
// Output: the index into which insert the element the array remains sorted
static size_t partition_point(const vec *v, const unsigned u) {
    // loop invariant:
    // array at left is less than u
    // array at right is no less than u
    // both always within boundary
    if (empty(v)) {
        return 0;
    } else {
        size_t left = 0, right = len(v) - 1;
        {
            // loop invariant may not hold at first;
            // exclude them
            if (get(v, left).u >= u) {
                return left;
            } else if (get(v, right).u < u) {
                return len(v);
            }
        }
        do {
            // loop invariant holds
            size_t mid = (left + right) / 2;
            if (mid == left) {
                // left and right differ by only 1
                break;
            } else {
                if (get(v, mid).u < u) {
                    left = mid;
                } else {
                    right = mid;
                }
            }
        } while (right > left);
        return right;
    }
}

const access_vec av = {.with_capacity = with_capacity,
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
                       .find = find,
                       .partition_point = partition_point};
