-include ../tools.mk

# Checks that all the targets returned by `dustc --print target-list` are valid
# target specifications
all:
	for target in $(shell $(BARE_DUSTC) --print target-list); do \
		$(BARE_DUSTC) --target $$target --print sysroot; \
	done
