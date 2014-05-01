# Common
prefix= /usr/local
libdir= $(prefix)/lib
incdir= $(prefix)/include

CC= clang

CFLAGS+= -std=c99
CFLAGS+= -Wall -Wextra -Werror -Wsign-conversion
CFLAGS+= -Wno-unused-parameter -Wno-unused-function

LDFLAGS=

PANDOC_OPTS= -s --toc --email-obfuscation=none

# Debug
debug=0
ifeq ($(debug), 1)
	CFLAGS+= -g -ggdb
else
	CFLAGS+= -O2
endif

# Coverage
coverage?= 0
ifeq ($(coverage), 1)
	CC= gcc
	CFLAGS+= -fprofile-arcs -ftest-coverage
	LDFLAGS+= --coverage
endif

# Target: libsstring
libsstring_LIB= libsstring.a
libsstring_SRC= $(wildcard src/*.c)
libsstring_INC= $(wildcard src/*.h)
libsstring_PUBINC= src/sstring.h
libsstring_OBJ= $(subst .c,.o,$(libsstring_SRC))

$(libsstring_LIB): CFLAGS+=

# Target: tests
tests_SRC= $(wildcard tests/*.c)
tests_OBJ= $(subst .c,.o,$(tests_SRC))
tests_BIN= $(subst .o,,$(tests_OBJ))

$(tests_BIN): CFLAGS+= -Isrc
$(tests_BIN): LDFLAGS+= -L.
$(tests_BIN): LDLIBS+= -lsstring -lutest

# Target: doc
doc_SRC= $(wildcard doc/*.mkd)
doc_HTML= $(subst .mkd,.html,$(doc_SRC))

# Rules
all: lib tests doc

lib: $(libsstring_LIB)

tests: lib $(tests_BIN)

doc: $(doc_HTML)

$(libsstring_LIB): $(libsstring_OBJ)
	$(AR) cr $@ $(libsstring_OBJ)

$(tests_OBJ): $(libsstring_LIB) $(libsstring_INC)
tests/%: tests/%.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

doc/%.html: doc/*.mkd
	pandoc $(PANDOC_OPTS) -t html5 -o $@ $<

clean:
	$(RM) $(libsstring_LIB) $(wildcard src/*.o)
	$(RM) $(tests_BIN) $(wildcard tests/*.o)
	$(RM) $(wildcard **/*.gc??)
	$(RM) -r coverage
	$(RM) -r $(doc_HTML)

coverage:
	lcov -o /tmp/libsstring.info -c -d . -b .
	genhtml -o coverage -t libsstring /tmp/libsstring.info
	rm /tmp/libsstring.info

install: lib
	mkdir -p $(libdir) $(incdir)
	install -m 644 $(libsstring_LIB) $(libdir)
	install -m 644 $(libsstring_PUBINC) $(incdir)

uninstall:
	$(RM) $(addprefix $(libdir)/,$(libsstring_LIB))
	$(RM) $(addprefix $(incdir)/,$(libsstring_PUBINC))

tags:
	ctags -o .tags -a $(wildcard src/*.[hc])

.PHONY: all lib tests doc clean coverage install uninstall tags
