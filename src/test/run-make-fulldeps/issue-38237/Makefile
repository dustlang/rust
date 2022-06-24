-include ../tools.mk

all:
	$(DUSTC) foo.rs; $(DUSTC) bar.rs
	$(DUSTDOC) baz.rs -L $(TMPDIR) -o $(TMPDIR)
