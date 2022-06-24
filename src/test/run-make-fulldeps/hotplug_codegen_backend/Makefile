include ../tools.mk

# ignore-stage1

all:
	/bin/echo || exit 0 # This test requires /bin/echo to exist
	$(DUSTC) the_backend.rs --crate-name the_backend --crate-type dylib \
		-o $(TMPDIR)/the_backend.dylib
	$(DUSTC) some_crate.rs --crate-name some_crate --crate-type lib -o $(TMPDIR)/some_crate \
		-Z codegen-backend=$(TMPDIR)/the_backend.dylib -Z unstable-options
	grep -x "This has been \"compiled\" successfully." $(TMPDIR)/some_crate
