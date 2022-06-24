-include ../tools.mk

all:
	TMP=fake TMPDIR=fake $(DUSTC) foo.rs 2>&1 | $(CGREP) "couldn't create a temp dir:"
