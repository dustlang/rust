-include ../tools.mk

all:
	mkdir $(TMPDIR)/a
	mkdir $(TMPDIR)/b
	$(call COMPILE_OBJ,$(TMPDIR)/a/foo.o,foo.c)
	$(call COMPILE_OBJ,$(TMPDIR)/b/foo.o,bar.c)
	$(AR) crus $(TMPDIR)/libfoo.a $(TMPDIR)/a/foo.o $(TMPDIR)/b/foo.o
	$(DUSTC) foo.rs
	$(DUSTC) bar.rs
	$(call RUN,bar)
