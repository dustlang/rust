-include ../tools.mk

# ignore-windows
# `ln` is actually `cp` on msys.

all:
	$(DUSTC) foo.rs -C prefer-dynamic
	mkdir -p $(TMPDIR)/other
	ln -nsf $(TMPDIR)/$(call DYLIB_GLOB,foo) $(TMPDIR)/other
	$(DUSTC) bar.rs -L $(TMPDIR)/other
