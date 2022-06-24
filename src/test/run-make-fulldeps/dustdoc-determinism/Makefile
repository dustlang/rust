-include ../tools.mk

# Assert that the search index is generated deterministically, regardless of the
# order that crates are documented in.

# ignore-windows
# Uses `diff`.

all:
	$(DUSTDOC) foo.rs -o $(TMPDIR)/foo_first
	$(DUSTDOC) bar.rs -o $(TMPDIR)/foo_first

	$(DUSTDOC) bar.rs -o $(TMPDIR)/bar_first
	$(DUSTDOC) foo.rs -o $(TMPDIR)/bar_first

	diff $(TMPDIR)/foo_first/search-index.js $(TMPDIR)/bar_first/search-index.js
