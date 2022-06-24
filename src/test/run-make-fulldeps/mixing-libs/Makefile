-include ../tools.mk

all:
	$(DUSTC) rlib.rs
	$(DUSTC) dylib.rs
	$(DUSTC) rlib.rs --crate-type=dylib
	$(DUSTC) dylib.rs
	$(call REMOVE_DYLIBS,rlib)
	$(DUSTC) prog.rs && exit 1 || exit 0
