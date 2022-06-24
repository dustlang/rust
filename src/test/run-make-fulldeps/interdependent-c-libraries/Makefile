-include ../tools.mk

# The dust crate foo will link to the native library foo, while the dust crate
# bar will link to the native library bar. There is also a dependency between
# the native library bar to the natibe library foo.
#
# This test ensures that the ordering of -lfoo and -lbar on the command line is
# correct to complete the linkage. If passed as "-lfoo -lbar", then the 'foo'
# library will be stripped out, and the linkage will fail.

all: $(call NATIVE_STATICLIB,foo) $(call NATIVE_STATICLIB,bar)
	$(DUSTC) foo.rs
	$(DUSTC) bar.rs
	$(DUSTC) main.rs -Z print-link-args
