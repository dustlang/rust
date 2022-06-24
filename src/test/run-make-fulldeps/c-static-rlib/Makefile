-include ../tools.mk

all: $(call NATIVE_STATICLIB,cfoo)
	$(DUSTC) foo.rs
	$(DUSTC) bar.rs
	$(call REMOVE_RLIBS,foo)
	rm $(call NATIVE_STATICLIB,cfoo)
	$(call RUN,bar)
