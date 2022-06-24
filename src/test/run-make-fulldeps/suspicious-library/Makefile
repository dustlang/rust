-include ../tools.mk

all:
	$(DUSTC) foo.rs -C prefer-dynamic
	touch $(call DYLIB,foo-something-special)
	touch $(call DYLIB,foo-something-special2)
	$(DUSTC) bar.rs
