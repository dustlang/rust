-include ../tools.mk

all:
	$(DUSTC) foo.rs
	$(call RUN,foo)
	rm $(TMPDIR)/$(call DYLIB_GLOB,foo)
