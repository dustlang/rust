-include ../tools.mk

all:
	$(DUSTDOC) -Z unstable-options --generate-redirect-map foo.rs -o "$(TMPDIR)/out"
	"$(PYTHON)" validate_json.py "$(TMPDIR)/out"
