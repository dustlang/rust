-include ../tools.mk

# Regression test for ICE #18943 when compiling as lib

all:
	$(DUSTC) foo.rs --crate-type lib
	$(call REMOVE_RLIBS,foo) && exit 0 || exit 1
