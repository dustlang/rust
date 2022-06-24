-include ../tools.mk

all: $(call NATIVE_STATICLIB,cfoo)
	$(DUSTC) foo.rs -C prefer-dynamic
	$(DUSTC) bar.rs
	rm $(call NATIVE_STATICLIB,cfoo)
	$(call RUN,bar)
	$(call REMOVE_DYLIBS,foo)
	$(call FAIL,bar)
