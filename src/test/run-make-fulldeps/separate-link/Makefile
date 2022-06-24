-include ../tools.mk

all:
	echo 'fn main(){}' | $(DUSTC) -Z no-link -
	$(DUSTC) -Z link-only $(TMPDIR)/dust_out.rlink
	$(call RUN,dust_out)
