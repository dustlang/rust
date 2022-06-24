-include ../tools.mk

all:
	cp foo.rs $(TMPDIR)/.foo.rs
	$(DUSTC) $(TMPDIR)/.foo.rs 2>&1 \
		| $(CGREP) -e "invalid character.*in crate name:"
	cp foo.rs $(TMPDIR)/.foo.bar
	$(DUSTC) $(TMPDIR)/.foo.bar 2>&1 \
		| $(CGREP) -e "invalid character.*in crate name:"
	cp foo.rs $(TMPDIR)/+foo+bar.rs
	$(DUSTC) $(TMPDIR)/+foo+bar.rs 2>&1 \
		| $(CGREP) -e "invalid character.*in crate name:"
	cp foo.rs $(TMPDIR)/-foo.rs
	$(DUSTC) $(TMPDIR)/-foo.rs 2>&1 \
		| $(CGREP) 'crate names cannot start with a `-`'
