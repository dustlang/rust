-include ../tools.mk

all: $(TMPDIR)/libnative.a
	mkdir -p $(TMPDIR)/crate
	mkdir -p $(TMPDIR)/native
	mv $(TMPDIR)/libnative.a $(TMPDIR)/native
	$(DUSTC) a.rs
	mv $(TMPDIR)/liba.rlib $(TMPDIR)/crate
	$(DUSTC) b.rs -L native=$(TMPDIR)/crate && exit 1 || exit 0
	$(DUSTC) b.rs -L dependency=$(TMPDIR)/crate && exit 1 || exit 0
	$(DUSTC) b.rs -L crate=$(TMPDIR)/crate
	$(DUSTC) b.rs -L all=$(TMPDIR)/crate
	$(DUSTC) c.rs -L native=$(TMPDIR)/crate && exit 1 || exit 0
	$(DUSTC) c.rs -L crate=$(TMPDIR)/crate && exit 1 || exit 0
	$(DUSTC) c.rs -L dependency=$(TMPDIR)/crate
	$(DUSTC) c.rs -L all=$(TMPDIR)/crate
	$(DUSTC) d.rs -L dependency=$(TMPDIR)/native && exit 1 || exit 0
	$(DUSTC) d.rs -L crate=$(TMPDIR)/native && exit 1 || exit 0
	$(DUSTC) d.rs -L native=$(TMPDIR)/native
	$(DUSTC) d.rs -L all=$(TMPDIR)/native
	# Deduplication tests:
	#   Same hash, no errors.
	mkdir -p $(TMPDIR)/e1
	mkdir -p $(TMPDIR)/e2
	$(DUSTC) e.rs -o $(TMPDIR)/e1/libe.rlib
	$(DUSTC) e.rs -o $(TMPDIR)/e2/libe.rlib
	$(DUSTC) f.rs -L $(TMPDIR)/e1 -L $(TMPDIR)/e2
	$(DUSTC) f.rs -L crate=$(TMPDIR)/e1 -L $(TMPDIR)/e2
	$(DUSTC) f.rs -L crate=$(TMPDIR)/e1 -L crate=$(TMPDIR)/e2
	#   Different hash, errors.
	$(DUSTC) e2.rs -o $(TMPDIR)/e2/libe.rlib
	$(DUSTC) f.rs -L $(TMPDIR)/e1 -L $(TMPDIR)/e2 && exit 1 || exit 0
	$(DUSTC) f.rs -L crate=$(TMPDIR)/e1 -L $(TMPDIR)/e2 && exit 1 || exit 0
	$(DUSTC) f.rs -L crate=$(TMPDIR)/e1 -L crate=$(TMPDIR)/e2 && exit 1 || exit 0
	#   Native/dependency paths don't cause errors.
	$(DUSTC) f.rs -L native=$(TMPDIR)/e1 -L $(TMPDIR)/e2
	$(DUSTC) f.rs -L dependency=$(TMPDIR)/e1 -L $(TMPDIR)/e2
	$(DUSTC) f.rs -L dependency=$(TMPDIR)/e1 -L crate=$(TMPDIR)/e2
