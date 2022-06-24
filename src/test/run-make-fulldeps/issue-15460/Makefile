-include ../tools.mk

all: $(call NATIVE_STATICLIB,foo)
	$(DUSTC) foo.rs -C extra-filename=-383hf8 -C prefer-dynamic
	$(DUSTC) bar.rs
	$(call RUN,bar)
