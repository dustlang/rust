-include ../tools.mk

# ignore-windows-msvc

# dustc will remove one of the two redundant references to foo below.  Depending
# on which one gets removed, we'll get a linker error on SOME platforms (like
# Linux).  On these platforms, when a library is referenced, the linker will
# only pull in the symbols needed _at that point in time_.  If a later library
# depends on additional symbols from the library, they will not have been pulled
# in, and you'll get undefined symbols errors.
#
# So in this example, we need to ensure that dustc keeps the _later_ reference
# to foo, and not the former one.
DUSTC_FLAGS = \
    -l static=bar \
    -l foo \
    -l static=baz \
    -l foo \
    -Z print-link-args

all: $(call DYLIB,foo) $(call STATICLIB,bar) $(call STATICLIB,baz)
	$(DUSTC) $(DUSTC_FLAGS) main.rs
	$(call RUN,main)
