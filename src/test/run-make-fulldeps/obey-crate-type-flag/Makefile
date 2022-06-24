-include ../tools.mk

# check that dustc builds all crate_type attributes
# delete rlib
# delete whatever dylib is made for this system
# check that dustc only builds --crate-type flags, ignoring attributes
# fail if an rlib was built
all:
	$(DUSTC) test.rs
	$(call REMOVE_RLIBS,test)
	$(call REMOVE_DYLIBS,test)
	$(DUSTC) --crate-type dylib test.rs
	$(call REMOVE_RLIBS,test) && exit 1 || exit 0
