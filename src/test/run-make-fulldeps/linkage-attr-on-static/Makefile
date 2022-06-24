-include ../tools.mk

all: $(call NATIVE_STATICLIB,foo)
	$(DUSTC) bar.rs
	$(call RUN,bar) || exit 1
