-include ../tools.mk
all: code
krate2: krate2.rs
	$(DUSTC) $<
code: foo.rs krate2
	$(DUSTC) foo.rs -Zsave-analysis
