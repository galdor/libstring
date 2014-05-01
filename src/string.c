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
#include <stdio.h>
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

int
str_string_set_vformat(struct str_string *str, const char *fmt, va_list ap) {
    char *nstr;
    size_t len;

    len = strlen(fmt) + 1;
    nstr = str_malloc(len);
    if (!nstr)
        return -1;

    for (;;) {
        int ret;
        va_list local_ap;
        char *nnstr;

        va_copy(local_ap, ap);
        ret = vsnprintf(nstr, len, fmt, local_ap);
        va_end(local_ap);

        if (ret == -1) {
            str_set_error("cannot format string: %s", strerror(errno));
            str_free(nstr);
            return -1;
        }

        if ((size_t)ret < len)
            break;

        len = (size_t)ret + 1;
        nnstr = str_realloc(nstr, len);
        if (!nnstr) {
            str_free(nstr);
            return -1;
        }
        nstr = nnstr;
    }

    str_free(str->str);

    str->str = nstr;
    str->len = strlen(nstr);

    return 0;
}

int
str_string_set_format(struct str_string *str, const char *fmt, ...) {
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = str_string_set_vformat(str, fmt, ap);
    va_end(ap);

    return ret;
}

int
str_string_append(struct str_string *str, const char *cstr) {
    return str_string_append2(str, cstr, strlen(cstr));
}

int
str_string_append2(struct str_string *str, const char *cstr, size_t len) {
    char *nstr;
    size_t nlen;

    nlen = str->len + len;
    nstr = str_realloc(str->str, nlen + 1);
    if (!nstr)
        return -1;

    memcpy(nstr + str->len, cstr, len);
    nstr[nlen] = '\0';

    str->str = nstr;
    str->len = nlen;

    return 0;
}

int
str_string_append_string(struct str_string *str, const struct str_string str2) {
    return str_string_append2(str, str2.str, str2.len);
}

int
str_string_append_vformat(struct str_string *str, const char *fmt, va_list ap) {
    struct str_string nstr;

    str_string_init(&nstr);
    if (str_string_set_vformat(&nstr, fmt, ap) == -1)
        return -1;

    if (str_string_append_string(str, nstr) == -1) {
        str_string_free(&nstr);
        return -1;
    }

    str_string_free(&nstr);
    return 0;
}

int
str_string_append_format(struct str_string *str, const char *fmt, ...) {
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = str_string_append_vformat(str, fmt, ap);
    va_end(ap);

    return ret;
}
