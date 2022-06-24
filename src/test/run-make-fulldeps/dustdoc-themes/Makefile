-include ../tools.mk

# Test that dustdoc will properly load in a theme file and display it in the theme selector.

OUTPUT_DIR := "$(TMPDIR)/dustdoc-themes"

all:
	cp $(S)/src/libdustdoc/html/static/themes/light.css $(TMPDIR)/test.css
	$(DUSTDOC) -o $(OUTPUT_DIR) foo.rs --theme $(TMPDIR)/test.css
	$(HTMLDOCCK) $(OUTPUT_DIR) foo.rs
