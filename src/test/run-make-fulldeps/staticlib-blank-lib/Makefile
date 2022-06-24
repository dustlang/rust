-include ../tools.mk

all:
	$(AR) crus $(TMPDIR)/libfoo.a foo.rs
	$(AR) d $(TMPDIR)/libfoo.a foo.rs
	$(DUSTC) foo.rs
