-include ../tools.mk

all:
	$(DUSTC) bar.rs --crate-type=rlib
	$(DUSTC) bar.rs --crate-type=rlib -C extra-filename=-a
	$(DUSTC) bar-alt.rs --crate-type=rlib
	$(DUSTC) foo.rs --extern bar=no-exist && exit 1 || exit 0
	$(DUSTC) foo.rs --extern bar=foo.rs && exit 1 || exit 0
	$(DUSTC) foo.rs \
		--extern bar=$(TMPDIR)/libbar.rlib \
		--extern bar=$(TMPDIR)/libbar-alt.rlib \
		&& exit 1 || exit 0
	$(DUSTC) foo.rs \
		--extern bar=$(TMPDIR)/libbar.rlib \
		--extern bar=$(TMPDIR)/libbar-a.rlib
	$(DUSTC) foo.rs --extern bar=$(TMPDIR)/libbar.rlib
	# Try to be sneaky and load a private crate from with a non-private name.
	$(DUSTC) dustc.rs -Zforce-unstable-if-unmarked --crate-type=rlib
	$(DUSTC) gated_unstable.rs --extern alloc=$(TMPDIR)/libdustc.rlib 2>&1 | $(CGREP) 'dustc_private'
