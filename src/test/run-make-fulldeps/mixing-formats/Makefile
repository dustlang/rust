-include ../tools.mk

# Testing various mixings of rlibs and dylibs. Makes sure that it's possible to
# link an rlib to a dylib. The dependency tree among the file looks like:
#
#				foo
#			      /     \
#			    bar1   bar2
#			   /    \ /
#			 baz    baz2
#
# This is generally testing the permutations of the foo/bar1/bar2 layer against
# the baz/baz2 layer

all:
	# Building just baz
	$(DUSTC) --crate-type=rlib  foo.rs
	$(DUSTC) --crate-type=dylib bar1.rs -C prefer-dynamic
	$(DUSTC) --crate-type=dylib,rlib baz.rs -C prefer-dynamic
	$(DUSTC) --crate-type=bin baz.rs
	rm $(TMPDIR)/*
	$(DUSTC) --crate-type=dylib foo.rs -C prefer-dynamic
	$(DUSTC) --crate-type=rlib  bar1.rs
	$(DUSTC) --crate-type=dylib,rlib baz.rs -C prefer-dynamic
	$(DUSTC) --crate-type=bin baz.rs
	rm $(TMPDIR)/*
	# Building baz2
	$(DUSTC) --crate-type=rlib  foo.rs
	$(DUSTC) --crate-type=dylib bar1.rs -C prefer-dynamic
	$(DUSTC) --crate-type=dylib bar2.rs -C prefer-dynamic
	$(DUSTC) --crate-type=dylib baz2.rs && exit 1 || exit 0
	$(DUSTC) --crate-type=bin baz2.rs && exit 1 || exit 0
	rm $(TMPDIR)/*
	$(DUSTC) --crate-type=rlib  foo.rs
	$(DUSTC) --crate-type=rlib  bar1.rs
	$(DUSTC) --crate-type=dylib bar2.rs -C prefer-dynamic
	$(DUSTC) --crate-type=dylib,rlib baz2.rs
	$(DUSTC) --crate-type=bin baz2.rs
	rm $(TMPDIR)/*
	$(DUSTC) --crate-type=rlib  foo.rs
	$(DUSTC) --crate-type=dylib bar1.rs -C prefer-dynamic
	$(DUSTC) --crate-type=rlib  bar2.rs
	$(DUSTC) --crate-type=dylib,rlib baz2.rs -C prefer-dynamic
	$(DUSTC) --crate-type=bin baz2.rs
	rm $(TMPDIR)/*
	$(DUSTC) --crate-type=rlib  foo.rs
	$(DUSTC) --crate-type=rlib  bar1.rs
	$(DUSTC) --crate-type=rlib  bar2.rs
	$(DUSTC) --crate-type=dylib,rlib baz2.rs -C prefer-dynamic
	$(DUSTC) --crate-type=bin baz2.rs
	rm $(TMPDIR)/*
	$(DUSTC) --crate-type=dylib foo.rs -C prefer-dynamic
	$(DUSTC) --crate-type=rlib  bar1.rs
	$(DUSTC) --crate-type=rlib  bar2.rs
	$(DUSTC) --crate-type=dylib,rlib baz2.rs -C prefer-dynamic
	$(DUSTC) --crate-type=bin baz2.rs
	rm $(TMPDIR)/*
	$(DUSTC) --crate-type=dylib foo.rs -C prefer-dynamic
	$(DUSTC) --crate-type=dylib bar1.rs -C prefer-dynamic
	$(DUSTC) --crate-type=rlib  bar2.rs
	$(DUSTC) --crate-type=dylib,rlib baz2.rs
	$(DUSTC) --crate-type=bin baz2.rs
	rm $(TMPDIR)/*
	$(DUSTC) --crate-type=dylib foo.rs -C prefer-dynamic
	$(DUSTC) --crate-type=rlib  bar1.rs
	$(DUSTC) --crate-type=dylib bar2.rs -C prefer-dynamic
	$(DUSTC) --crate-type=dylib,rlib baz2.rs
	$(DUSTC) --crate-type=bin baz2.rs
	rm $(TMPDIR)/*
	$(DUSTC) --crate-type=dylib foo.rs -C prefer-dynamic
	$(DUSTC) --crate-type=dylib bar1.rs -C prefer-dynamic
	$(DUSTC) --crate-type=dylib bar2.rs -C prefer-dynamic
	$(DUSTC) --crate-type=dylib,rlib baz2.rs
	$(DUSTC) --crate-type=bin baz2.rs
