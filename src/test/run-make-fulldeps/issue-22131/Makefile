-include ../tools.mk

all: foo.rs
	$(DUSTC) --cfg 'feature="bar"' --crate-type lib foo.rs
	$(DUSTDOC) --test --cfg 'feature="bar"' \
		-L $(TMPDIR) foo.rs |\
		$(CGREP) 'foo.rs - foo (line 1) ... ok'
