-include ../tools.mk

all:
	$(DUSTC) libc.rs -Cmetadata=foo
	$(DUSTC) main.rs --extern libc=$(TMPDIR)/liblibc.rlib
