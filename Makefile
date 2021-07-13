#
#  Makefile
#
#  Copyright (c) 2021 by Daniel Kelley
#

DEBUG ?= -g -O0

PREFIX ?= /usr/local

# address thread undefined etc.
ifneq ($(SANITIZE),)
DEBUG += -fsanitize=$(SANITIZE)
endif

INC := -I/usr/local/include
CPPFLAGS := $(INC) -MP -MMD

WARN := -Wall
WARN += -Wextra
WARN += -Wdeclaration-after-statement
WARN += -Werror
CFLAGS := $(WARN) $(DEBUG) -fPIC

LDFLAGS := $(DEBUG) -L.
LDLIBS := -lm

PVOCFINFO_SRC := pvocf-info.c
PVOCFINFO_OBJ := $(PVOCFINFO_SRC:%.c=%.o)
PVOCFINFO_DEP := $(PVOCFINFO_SRC:%.c=%.d)
OBJ := $(PVOCFINFO_OBJ)
DEP := $(PVOCFINFO_DEP)

LIBMAJOR := 0
LIBMINOR := 1

LIBNAME := libpvocf
LIBRARY := $(LIBNAME).a
SHLIBRARY := $(LIBNAME).so
SHLIBRARY_VER := $(LIBNAME)-$(LIBMAJOR).$(LIBMINOR).so

PVOCFLIB_SRC := pvocf_open.c
PVOCFLIB_SRC += pvocf_close.c
PVOCFLIB_SRC += pvocf_frame_count.c
PVOCFLIB_SRC += pvocf_get_frame.c
PVOCFLIB_SRC += pvocf_get_info.c
PVOCFLIB_LDLIBS := -lriffr
PVOCFLIB_OBJ := $(PVOCFLIB_SRC:%.c=%.o)
PVOCFLIB_DEP := $(PVOCFLIB_SRC:%.c=%.d)
OBJ += $(PVOCFLIB_OBJ)
DEP += $(PVOCFLIB_DEP)

PROG := pvocf-info

VG ?= valgrind --leak-check=full

.PHONY: all install uninstall clean

all: $(PROG) $(LIBRARY) $(SHLIBRARY)

pvocf-info: $(PVOCFINFO_SRC) $(SHLIBRARY)

$(LIBRARY): $(PVOCFLIB_OBJ)
	$(AR) cr $@ $^

$(SHLIBRARY): $(SHLIBRARY_VER)
	ln -sf $< $@

$(SHLIBRARY_VER): $(PVOCFLIB_OBJ)
	$(CC) -shared -Wl,-soname,$@ -o $@ \
		$(LDFLAGS) $(PVOCFLIB_LDLIBS) $(PVOCFLIB_OBJ)

install: $(PROG) $(SHLIBRARY) $(LIBRARY)
	install -p -m 755 $(PROG) $(PREFIX)/bin
	install -p -m 644 $(SHLIBRARY_VER) $(LIBRARY) $(PREFIX)/lib
	ln -sf -r $(PREFIX)/lib/$(SHLIBRARY_VER) $(PREFIX)/lib/$(SHLIBRARY)

uninstall:
	-rm -f $(PREFIX)/bin/wktplot

#test: $(PROG)
#	LD_LIBRARY_PATH=. ...

#
# libplot is a bit leaky, but svg and X plotter is leakier than ps
#
#valgrind-test: $(PROG)
	LD_LIBRARY_PATH=. $(VG) ...

clean:
	-rm -f $(PROG) $(SHLIBRARY) $(SHLIBRARY_VER) $(LIBRARY) $(OBJ) $(DEP)

-include $(DEP)
