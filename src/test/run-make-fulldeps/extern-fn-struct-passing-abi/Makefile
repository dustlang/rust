-include ../tools.mk

all: $(call NATIVE_STATICLIB,test)
	$(DUSTC) test.rs
	$(call RUN,test) || exit 1
