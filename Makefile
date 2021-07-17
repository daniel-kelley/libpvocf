#
#  Makefile
#
#  Copyright (c) 2021 by Daniel Kelley
#

DEBUG ?= -g

PREFIX ?= /usr/local

# address thread undefined etc.
ifneq ($(SANITIZE),)
DEBUG += -fsanitize=$(SANITIZE)
endif

INC := -I$(PREFIX)/include
CPPFLAGS := $(INC) -MP -MMD

WARN := -Wall
WARN += -Wextra
WARN += -Wdeclaration-after-statement
WARN += -Werror
CFLAGS := $(WARN) $(DEBUG) -fPIC

LDFLAGS := $(DEBUG) -L. -L$(PREFIX)/lib
LDLIBS := -lriffr -lm

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
PVOCFLIB_SRC += pvocf_frame_size.c
PVOCFLIB_LDLIBS := -lriffr
PVOCFLIB_OBJ := $(PVOCFLIB_SRC:%.c=%.o)
PVOCFLIB_DEP := $(PVOCFLIB_SRC:%.c=%.d)
OBJ += $(PVOCFLIB_OBJ)
DEP += $(PVOCFLIB_DEP)

PROG := pvocf-info

.PHONY: all install uninstall clean libs check

all: $(PROG) libs

libs: $(LIBRARY) $(SHLIBRARY)

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
	-ln -sf $(SHLIBRARY_VER) $(PREFIX)/lib/$(SHLIBRARY)

uninstall:
	-rm -f $(PREFIX)/bin/wktplot

chirp.wav: test/chirp.csd
	csound $<

chirp.pvx: chirp.wav
	csound -U pvanal -n 4096 -h 1024 $< $@

chirp.txt: ./pvocf-info chirp.pvx
	LD_LIBRARY_PATH=. ./$+ -A -F -b8 > $@ 2> chirp.err

chirp.all: ./pvocf-info chirp.pvx
	LD_LIBRARY_PATH=. ./$+ -A > $@ 2> chirp.all.err

chirp.asc: chirp.pvx
	csound -U pvlook $< 2> $@

check: chirp.txt
	cat $< chirp.err
	cmp -s $< test/chirp0.txt

clean:
	-rm -f $(PROG) $(SHLIBRARY) $(SHLIBRARY_VER) $(LIBRARY) \
		$(OBJ) $(DEP) \
		chirp.wav chirp.pvx chirp.txt chirp.err


-include $(DEP)
