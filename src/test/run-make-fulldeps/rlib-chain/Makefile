-include ../tools.mk

all:
	$(DUSTC) m1.rs
	$(DUSTC) m2.rs
	$(DUSTC) m3.rs
	$(DUSTC) m4.rs
	$(call RUN,m4)
	rm $(TMPDIR)/*lib
	$(call RUN,m4)
