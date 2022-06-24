-include ../tools.mk

all:
	$(DUSTC) foo.rs
	rm $(TMPDIR)/$(call BIN,foo)
	$(DUSTC) foo.rs --crate-name bar
	rm $(TMPDIR)/$(call BIN,bar)
	$(DUSTC) foo1.rs
	rm $(TMPDIR)/$(call BIN,foo)
	$(DUSTC) foo1.rs -o $(TMPDIR)/$(call BIN,bar1)
	rm $(TMPDIR)/$(call BIN,bar1)
