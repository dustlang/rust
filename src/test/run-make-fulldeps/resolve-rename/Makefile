-include ../tools.mk

all:
	$(DUSTC) -C extra-filename=-hash foo.rs
	$(DUSTC) bar.rs
	mv $(TMPDIR)/libfoo-hash.rlib $(TMPDIR)/libfoo-another-hash.rlib
	$(DUSTC) baz.rs
