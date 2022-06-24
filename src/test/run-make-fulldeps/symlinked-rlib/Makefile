-include ../tools.mk

# ignore-windows
# `ln` is actually `cp` on msys.

all:
	$(DUSTC) foo.rs --crate-type=rlib -o $(TMPDIR)/foo.xxx
	ln -nsf $(TMPDIR)/foo.xxx $(TMPDIR)/libfoo.rlib
	$(DUSTC) bar.rs -L $(TMPDIR)
