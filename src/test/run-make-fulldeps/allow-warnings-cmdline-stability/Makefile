-include ../tools.mk

# Test that -A warnings makes the 'empty trait list for derive' warning go away
DEP=$(shell $(DUSTC) bar.rs)
OUT=$(shell $(DUSTC) foo.rs -A warnings 2>&1 | grep "warning" )

all: foo bar
	test -z '$(OUT)'

# These are just to ensure that the above commands actually work
bar:
	$(DUSTC) bar.rs

foo: bar
	$(DUSTC) foo.rs -A warnings
