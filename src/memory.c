/*
 * Copyright (c) 2014 Nicolas Martyanoff
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "sstring.h"
#include "internal.h"

#define STR_DEFAULT_ALLOCATOR \
    {                        \
        .malloc = malloc,    \
        .free = free,        \
        .calloc = calloc,    \
        .realloc = realloc   \
    }

static const struct str_memory_allocator str_default_allocator =
    STR_DEFAULT_ALLOCATOR;

static struct str_memory_allocator str_allocator = STR_DEFAULT_ALLOCATOR;

const struct str_memory_allocator *
str_default_memory_allocator = &str_default_allocator;

void
str_set_memory_allocator(const struct str_memory_allocator *allocator) {
    if (allocator) {
        str_allocator = *allocator;
    } else {
        str_allocator = str_default_allocator;
    }
}

void *
str_malloc(size_t sz) {
    void *ptr;

    ptr = str_allocator.malloc(sz);
    if (!ptr) {
        str_set_error("cannot allocate %zu bytes: %s",
                      sz, strerror(errno));
        return NULL;
    }

    return ptr;
}

void
str_free(void *ptr) {
    str_allocator.free(ptr);
}

void *
str_calloc(size_t nb, size_t sz) {
    void *ptr;

    ptr = str_allocator.calloc(nb, sz);
    if (!ptr) {
        str_set_error("cannot allocate %zux%zu bytes: %s",
                      nb, sz, strerror(errno));
        return NULL;
    }

    return ptr;
}

void *
str_realloc(void *ptr, size_t sz) {
    void *nptr;

    nptr = str_allocator.realloc(ptr, sz);
    if (!nptr) {
        str_set_error("cannot reallocate %zu bytes: %s",
                      sz, strerror(errno));
        return NULL;
    }

    return nptr;
}

