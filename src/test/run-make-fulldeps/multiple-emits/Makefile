-include ../tools.mk

all:
	$(DUSTC) foo.rs --emit=asm,llvm-ir -o $(TMPDIR)/out 2>&1
	rm $(TMPDIR)/out.ll $(TMPDIR)/out.s
	$(DUSTC) foo.rs --emit=asm,llvm-ir -o $(TMPDIR)/out2.ext 2>&1
	rm $(TMPDIR)/out2.ll $(TMPDIR)/out2.s
