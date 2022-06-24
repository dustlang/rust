-include ../tools.mk

all: $(call NATIVE_STATICLIB,ctest)
	$(DUSTC) testcrate.rs
	$(DUSTC) test.rs
	$(call RUN,test) || exit 1
