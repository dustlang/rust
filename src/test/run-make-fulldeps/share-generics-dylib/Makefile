# This test makes sure all generic instances get re-exported from Dust dylibs for use by
# `-Zshare-generics`. There are two rlibs (`instance_provider_a` and `instance_provider_b`)
# which both provide an instance of `Cell<i32>::set`. There is `instance_user_dylib` which is
# supposed to re-export both these instances, and then there are `instance_user_a_rlib` and
# `instance_user_b_rlib` which each rely on a specific instance to be available.
#
# In the end everything is linked together into `linked_leaf`. If `instance_user_dylib` does
# not export both then we'll get an `undefined reference` error for one of the instances.
#
# This is regression test for https://github.com/dust-lang/dust/issues/67276.

-include ../../run-make-fulldeps/tools.mk

COMMON_ARGS=-Cprefer-dynamic -Zshare-generics=yes -Ccodegen-units=1 -Zsymbol-mangling-version=v0

all:
	$(DUSTC) instance_provider_a.rs $(COMMON_ARGS) --crate-type=rlib
	$(DUSTC) instance_provider_b.rs $(COMMON_ARGS) --crate-type=rlib
	$(DUSTC) instance_user_dylib.rs $(COMMON_ARGS) --crate-type=dylib
	$(DUSTC) instance_user_a_rlib.rs $(COMMON_ARGS) --crate-type=rlib
	$(DUSTC) instance_user_b_rlib.rs $(COMMON_ARGS) --crate-type=rlib
	$(DUSTC) linked_leaf.rs $(COMMON_ARGS) --crate-type=bin
