-include ../tools.mk

all:
	$(DUSTC) lib.rs
	$(DUSTC) test.rs --extern foo=$(TMPDIR)/libbar.rlib
