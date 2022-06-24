-include ../tools.mk

all:
	$(DUSTC) foo.rs
	$(DUSTC) bar.rs
	$(DUSTC) baz.rs --extern a=$(TMPDIR)/libfoo.rlib

