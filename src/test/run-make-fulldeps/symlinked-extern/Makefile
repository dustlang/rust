-include ../tools.mk

# ignore-windows
# `ln` is actually `cp` on msys.

all:
	$(DUSTC) foo.rs
	mkdir -p $(TMPDIR)/other
	ln -nsf $(TMPDIR)/libfoo.rlib $(TMPDIR)/other
	$(DUSTC) bar.rs -L $(TMPDIR)
	$(DUSTC) baz.rs --extern foo=$(TMPDIR)/other/libfoo.rlib  -L $(TMPDIR)
