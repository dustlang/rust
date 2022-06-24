-include ../tools.mk

all:
	$(DUSTC) foo.rs -C metadata=a -C extra-filename=-a
	$(DUSTC) foo.rs -C metadata=b -C extra-filename=-b
	$(DUSTC) bar.rs \
		--extern foo1=$(TMPDIR)/libfoo-a.rlib \
		--extern foo2=$(TMPDIR)/libfoo-b.rlib \
		-Z print-link-args
	$(call RUN,bar)
