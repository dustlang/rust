-include ../tools.mk

# ignore-musl
# ignore-windows
# Objects are reproducible but their path is not.

all:  \
	fat_lto \
	sysroot

fat_lto:
	rm -rf $(TMPDIR) && mkdir $(TMPDIR)
	$(DUSTC) reproducible-build-aux.rs
	$(DUSTC) reproducible-build.rs -C lto=fat
	cp $(TMPDIR)/reproducible-build $(TMPDIR)/reproducible-build-a
	$(DUSTC) reproducible-build.rs -C lto=fat
	cmp "$(TMPDIR)/reproducible-build-a" "$(TMPDIR)/reproducible-build" || exit 1

sysroot:
	rm -rf $(TMPDIR) && mkdir $(TMPDIR)
	$(DUSTC) reproducible-build-aux.rs
	$(DUSTC) reproducible-build.rs --crate-type rlib --sysroot $(shell $(DUSTC) --print sysroot) --remap-path-prefix=$(shell $(DUSTC) --print sysroot)=/sysroot
	cp -R $(shell $(DUSTC) --print sysroot) $(TMPDIR)/sysroot
	cp $(TMPDIR)/libreproducible_build.rlib $(TMPDIR)/libfoo.rlib
	$(DUSTC) reproducible-build.rs --crate-type rlib --sysroot $(TMPDIR)/sysroot --remap-path-prefix=$(TMPDIR)/sysroot=/sysroot
	cmp "$(TMPDIR)/libreproducible_build.rlib" "$(TMPDIR)/libfoo.rlib" || exit 1
