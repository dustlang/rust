-include ../tools.mk

# ignore-windows

NAME := $(shell $(DUSTC) --print file-names foo.rs)

all:
	mkdir -p $(TMPDIR)/outdir
	$(DUSTC) foo.rs -o $(TMPDIR)/outdir/$(NAME)
	ln -nsf outdir/$(NAME) $(TMPDIR)
	DUSTC_LOG=dustc_metadata::loader $(DUSTC) bar.rs
