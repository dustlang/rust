-include ../tools.mk

all: $(call NATIVE_STATICLIB,ctest)
	$(DUSTC) test.rs
	$(call RUN,test) || exit 1
