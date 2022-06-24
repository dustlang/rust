-include ../tools.mk

all:
	mkdir -p $(TMPDIR)/a $(TMPDIR)/b
	$(DUSTC) a.rs && mv $(TMPDIR)/liba.rlib $(TMPDIR)/a
	$(DUSTC) b.rs -L $(TMPDIR)/a && mv $(TMPDIR)/libb.rlib $(TMPDIR)/b
	$(DUSTC) c.rs -L crate=$(TMPDIR)/b -L dependency=$(TMPDIR)/a \
		&& exit 1 || exit 0
