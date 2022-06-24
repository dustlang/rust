-include ../tools.mk

all:
	$(DUSTC) both.rs -C prefer-dynamic
	$(DUSTC) dylib.rs -C prefer-dynamic
	$(DUSTC) prog.rs
	$(call RUN,prog)
