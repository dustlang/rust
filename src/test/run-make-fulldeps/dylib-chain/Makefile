-include ../tools.mk

all:
	$(DUSTC) m1.rs -C prefer-dynamic
	$(DUSTC) m2.rs -C prefer-dynamic
	$(DUSTC) m3.rs -C prefer-dynamic
	$(DUSTC) m4.rs
	$(call RUN,m4)
	$(call REMOVE_DYLIBS,m1)
	$(call REMOVE_DYLIBS,m2)
	$(call REMOVE_DYLIBS,m3)
	$(call FAIL,m4)
