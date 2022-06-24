-include ../tools.mk

all:
	$(DUSTC) a.rs && $(DUSTC) b.rs
	$(BARE_DUSTC) c.rs -L dependency=$(TMPDIR) --extern b=$(TMPDIR)/libb.rlib \
		--out-dir=$(TMPDIR)
