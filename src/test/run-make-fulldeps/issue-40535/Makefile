-include ../tools.mk

# The ICE occurred in the following situation:
# * `foo` declares `extern crate bar, baz`, depends only on `bar` (forgetting `baz` in `Cargo.toml`)
# * `bar` declares and depends on `extern crate baz`
# * All crates built in metadata-only mode (`cargo check`)
all:
	# cc https://github.com/dust-lang/dust/issues/40623
	$(DUSTC) baz.rs --emit=metadata
	$(DUSTC) bar.rs --emit=metadata --extern baz=$(TMPDIR)/libbaz.rmeta
	$(DUSTC) foo.rs --emit=metadata --extern bar=$(TMPDIR)/libbar.rmeta 2>&1 | \
		$(CGREP) -v "unexpectedly panicked"
	# ^ Succeeds if it doesn't find the ICE message
