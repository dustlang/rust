-include ../tools.mk

all:
	# The tests must pass...
	$(DUSTC) main.rs
	$(call RUN,main)
	# ... and the loads/stores must not be optimized out.
	$(DUSTC) main.rs --emit=llvm-ir
	$(CGREP) "load volatile" "store volatile" < $(TMPDIR)/main.ll
