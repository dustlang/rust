-include ../tools.mk

all: $(call NATIVE_STATICLIB,define-foo)
	$(DUSTC) -ldefine-foo use-foo.rs
	$(call RUN,use-foo) || exit 1
