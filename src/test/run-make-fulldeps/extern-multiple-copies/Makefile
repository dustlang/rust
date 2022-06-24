-include ../tools.mk

all:
	$(DUSTC) foo1.rs
	$(DUSTC) foo2.rs
	mkdir $(TMPDIR)/foo
	cp $(TMPDIR)/libfoo1.rlib $(TMPDIR)/foo/libfoo1.rlib
	$(DUSTC) bar.rs --extern foo1=$(TMPDIR)/libfoo1.rlib -L $(TMPDIR)/foo
