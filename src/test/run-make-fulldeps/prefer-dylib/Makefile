-include ../tools.mk

all:
	$(DUSTC) bar.rs --crate-type=dylib --crate-type=rlib -C prefer-dynamic
	$(DUSTC) foo.rs -C prefer-dynamic
	$(call RUN,foo)
	rm $(TMPDIR)/*bar*
	$(call FAIL,foo)
