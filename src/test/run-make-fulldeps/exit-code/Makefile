-include ../tools.mk

all:
	$(DUSTC) success.rs; [ $$? -eq 0 ]
	$(DUSTC) --invalid-arg-foo; [ $$? -eq 1 ]
	$(DUSTC) compile-error.rs; [ $$? -eq 1 ]
	$(DUSTC) -Ztreat-err-as-bug compile-error.rs; [ $$? -eq 101 ]
	$(DUSTDOC) -o $(TMPDIR)/exit-code success.rs; [ $$? -eq 0 ]
	$(DUSTDOC) --invalid-arg-foo; [ $$? -eq 1 ]
	$(DUSTDOC) compile-error.rs; [ $$? -eq 1 ]
	$(DUSTDOC) lint-failure.rs; [ $$? -eq 1 ]
