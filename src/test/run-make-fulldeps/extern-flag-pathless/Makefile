-include ../tools.mk

# Test mixing pathless --extern with paths.

all:
	$(DUSTC) bar-static.rs --crate-name=bar --crate-type=rlib
	$(DUSTC) bar-dynamic.rs --crate-name=bar --crate-type=dylib -C prefer-dynamic
	# rlib preferred over dylib
	$(DUSTC) foo.rs --extern bar
	$(call RUN,foo) | $(CGREP) 'static'
	$(DUSTC) foo.rs --extern bar=$(TMPDIR)/libbar.rlib --extern bar
	$(call RUN,foo) | $(CGREP) 'static'
	# explicit --extern overrides pathless
	$(DUSTC) foo.rs --extern bar=$(call DYLIB,bar) --extern bar
	$(call RUN,foo) | $(CGREP) 'dynamic'
	# prefer-dynamic does what it says
	$(DUSTC) foo.rs --extern bar -C prefer-dynamic
	$(call RUN,foo) | $(CGREP) 'dynamic'
