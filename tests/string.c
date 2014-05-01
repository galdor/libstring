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

#include "tests.h"

TEST(clone) {
    struct str_string str, nstr;

    str_string_init(&str);
    str_string_set(&str, "");
    str_string_clone(str, &nstr);
    STRT_STRING_EQ(nstr, "", 0);
    str_string_free(&str);
    str_string_free(&nstr);

    str_string_init(&str);
    str_string_set(&str, "foo");
    str_string_clone(str, &nstr);
    STRT_STRING_EQ(nstr, "foo", 3);
    str_string_free(&str);
    str_string_free(&nstr);
}

TEST(copy) {
    struct str_string str, nstr;

    str_string_init(&str);
    str_string_init(&nstr);
    str_string_set(&str, "");
    str_string_copy(str, &nstr);
    STRT_STRING_EQ(nstr, "", 0);
    str_string_free(&nstr);

    str_string_set(&str, "foo");
    str_string_init(&nstr);
    str_string_copy(str, &nstr);
    STRT_STRING_EQ(nstr, "foo", 3);
    str_string_free(&str);
    str_string_free(&nstr);
}

TEST(set) {
    struct str_string str;

    str_string_init(&str);
    str_string_set(&str, "");
    STRT_STRING_EQ(str, "", 0);
    str_string_free(&str);

    str_string_init(&str);
    str_string_set(&str, "foo");
    STRT_STRING_EQ(str, "foo", 3);
    str_string_free(&str);

    str_string_init(&str);
    str_string_set2(&str, "foo", 0);
    STRT_STRING_EQ(str, "", 0);
    str_string_free(&str);

    str_string_init(&str);
    str_string_set2(&str, "foo", 1);
    STRT_STRING_EQ(str, "f", 1);
    str_string_free(&str);
}

TEST(set_format) {
    struct str_string str;

    str_string_init(&str);
    str_string_set_format(&str, "");
    STRT_STRING_EQ(str, "", 0);
    str_string_set_format(&str, "%c", 'f');
    STRT_STRING_EQ(str, "f", 1);
    str_string_set_format(&str, "foo");
    STRT_STRING_EQ(str, "foo", 3);
    str_string_set_format(&str, "foo %d %s", 42, "bar");
    STRT_STRING_EQ(str, "foo 42 bar", 10);
    str_string_free(&str);
}

TEST(append) {
    struct str_string str, str2;

    str_string_init(&str);
    str_string_append(&str, "");
    STRT_STRING_EQ(str, "", 0);
    str_string_append(&str, "foo");
    STRT_STRING_EQ(str, "foo", 3);
    str_string_append2(&str, " bar", 4);
    STRT_STRING_EQ(str, "foo bar", 7);
    str_string_append(&str, "");
    STRT_STRING_EQ(str, "foo bar", 7);
    str_string_free(&str);

    str_string_init(&str);
    str_string_init(&str2);
    str_string_append(&str, "foo");
    STRT_STRING_EQ(str, "foo", 3);
    str_string_set(&str2, " bar");
    str_string_append_string(&str, str2);
    STRT_STRING_EQ(str, "foo bar", 7);
    str_string_set(&str2, " baz");
    str_string_append_string(&str, str2);
    STRT_STRING_EQ(str, "foo bar baz", 11);
    str_string_free(&str);
    str_string_free(&str2);
}

TEST(append_format) {
    struct str_string str;

    str_string_init(&str);
    str_string_append_format(&str, "");
    STRT_STRING_EQ(str, "", 0);
    str_string_append_format(&str, "%c", 'f');
    STRT_STRING_EQ(str, "f", 1);
    str_string_append_format(&str, "oo");
    STRT_STRING_EQ(str, "foo", 3);
    str_string_append_format(&str, " %d %s", 42, "bar");
    STRT_STRING_EQ(str, "foo 42 bar", 10);
    str_string_free(&str);
}

int
main(int argc, char **argv) {
    struct test_suite *suite;

    suite = test_suite_new("string");
    test_suite_initialize_from_args(suite, argc, argv);

    test_suite_start(suite);

    TEST_RUN(suite, clone);
    TEST_RUN(suite, copy);
    TEST_RUN(suite, set);
    TEST_RUN(suite, set_format);
    TEST_RUN(suite, append);
    TEST_RUN(suite, append_format);

    test_suite_print_results_and_exit(suite);
}
