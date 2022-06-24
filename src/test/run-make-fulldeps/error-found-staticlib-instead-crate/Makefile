-include ../tools.mk

all:
	$(DUSTC) foo.rs --crate-type staticlib
	$(DUSTC) bar.rs 2>&1 | $(CGREP) "found staticlib"
