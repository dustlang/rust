-include ../tools.mk

all:
	$(DUSTC) bar.rs --crate-type=dylib --crate-type=rlib
	ls $(TMPDIR)/$(call RLIB_GLOB,bar)
	$(DUSTC) foo.rs
	rm $(TMPDIR)/*bar*
	$(call RUN,foo)
