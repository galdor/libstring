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

#ifndef LIBSTRING_STRING_H
#define LIBSTRING_STRING_H

#include <stdarg.h>
#include <stdlib.h>

struct str_memory_allocator {
   void *(*malloc)(size_t sz);
   void (*free)(void *ptr);
   void *(*calloc)(size_t nb, size_t sz);
   void *(*realloc)(void *ptr, size_t sz);
};

extern const struct str_memory_allocator *str_default_memory_allocator;

void str_set_memory_allocator(const struct str_memory_allocator *allocator);


const char *str_get_error(void);


struct str_string {
    char *str;
    size_t len;
};

void str_string_init(struct str_string *);
void str_string_free(struct str_string *);

void str_string_set_ptr(struct str_string *, char *);
int str_string_set(struct str_string *, const char *);
int str_string_set2(struct str_string *, const char *, size_t);

#endif
