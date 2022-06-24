-include ../tools.mk

# ignore-macos
#
# This hits an assertion in the linker on older versions of osx apparently

all: $(call DYLIB,cfoo)
	$(DUSTC) foo.rs -C prefer-dynamic
	$(DUSTC) bar.rs
	$(call RUN,bar)
	$(call REMOVE_DYLIBS,cfoo)
	$(call FAIL,bar)
