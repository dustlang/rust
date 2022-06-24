-include ../tools.mk

all:
	$(DUSTC) foo.rs
	$(DUSTC) -Z ls $(TMPDIR)/foo
	touch $(TMPDIR)/bar
	$(DUSTC) -Z ls $(TMPDIR)/bar
