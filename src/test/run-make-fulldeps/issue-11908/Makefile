# This test ensures that if you have the same rlib or dylib at two locations
# in the same path that you don't hit an assertion in the compiler.
#
# Note that this relies on `liburl` to be in the path somewhere else,
# and then our aux-built libraries will collide with liburl (they have
# the same version listed)

-include ../tools.mk

all:
	mkdir $(TMPDIR)/other
	$(DUSTC) foo.rs --crate-type=dylib -C prefer-dynamic
	mv $(call DYLIB,foo) $(TMPDIR)/other
	$(DUSTC) foo.rs --crate-type=dylib -C prefer-dynamic
	$(DUSTC) bar.rs -L $(TMPDIR)/other
	rm -rf $(TMPDIR)
	mkdir -p $(TMPDIR)/other
	$(DUSTC) foo.rs --crate-type=rlib
	mv $(TMPDIR)/libfoo.rlib $(TMPDIR)/other
	$(DUSTC) foo.rs --crate-type=rlib
	$(DUSTC) bar.rs -L $(TMPDIR)/other
