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

#include <string.h>

#include "sstring.h"
#include "internal.h"

void
str_string_init(struct str_string *str) {
    *str = (struct str_string){NULL, 0};
}

void
str_string_free(struct str_string *str) {
    if (!str)
        return;

    str_free(str->str);

    *str = (struct str_string){NULL, 0};
}

int
str_string_clone(const struct str_string str, struct str_string *nstr) {
    str_string_init(nstr);
    return str_string_set2(nstr, str.str, str.len);
}

int
str_string_copy(const struct str_string str, struct str_string *nstr) {
    str_string_free(nstr);
    str_string_init(nstr);
    return str_string_set2(nstr, str.str, str.len);
}

void
str_string_set_ptr(struct str_string *str, char *cstr) {
    str_free(str->str);

    str->str = cstr;
    str->len = strlen(cstr);
}

int
str_string_set(struct str_string *str, const char *cstr) {
    return str_string_set2(str, cstr, strlen(cstr));
}

int
str_string_set2(struct str_string *str, const char *cstr, size_t len) {
    char *nstr;

    nstr = str_malloc(len + 1);
    if (!nstr)
        return -1;

    memcpy(nstr, cstr, len);
    nstr[len] = '\0';

    str_free(str->str);

    str->str = nstr;
    str->len = len;

    return 0;
}
