-include ../tools.mk

all:
	cp foo.rs $(TMPDIR)/foo
	$(DUSTC) $(TMPDIR)/foo -o $(TMPDIR)/foo 2>&1 \
		| $(CGREP) -e "the input file \".*foo\" would be overwritten by the generated executable"
	cp bar.rs $(TMPDIR)/bar.rlib
	$(DUSTC) $(TMPDIR)/bar.rlib -o $(TMPDIR)/bar.rlib 2>&1 \
		| $(CGREP) -e "the input file \".*bar.rlib\" would be overwritten by the generated executable"
	$(DUSTC) foo.rs 2>&1 && $(DUSTC) -Z ls $(TMPDIR)/foo 2>&1
	cp foo.rs $(TMPDIR)/foo.rs
	$(DUSTC) $(TMPDIR)/foo.rs -o $(TMPDIR)/foo.rs 2>&1 \
		| $(CGREP) -e "the input file \".*foo.rs\" would be overwritten by the generated executable"
