-include ../tools.mk

all:
	$(DUSTC) foo-bar.rs
	[ -f $(TMPDIR)/$(call BIN,foo-bar) ]
	[ -f $(TMPDIR)/libfoo_bar.rlib ]
