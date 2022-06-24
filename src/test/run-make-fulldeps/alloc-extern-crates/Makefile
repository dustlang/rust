-include ../tools.mk

all:
	$(DUSTC) fakealloc.rs
	$(DUSTC) --edition=2018 --crate-type=rlib ../../../../library/alloc/src/lib.rs --cfg feature=\"external_crate\" --extern external=$(TMPDIR)/$(shell $(DUSTC) --print file-names fakealloc.rs)
