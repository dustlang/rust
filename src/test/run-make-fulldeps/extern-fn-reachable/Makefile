-include ../tools.mk

# This overrides the LD_LIBRARY_PATH for RUN
TARGET_RPATH_DIR:=$(TARGET_RPATH_DIR):$(TMPDIR)

all:
	$(DUSTC) dylib.rs -o $(TMPDIR)/libdylib.so -C prefer-dynamic
	$(DUSTC) main.rs -C prefer-dynamic
	$(call RUN,main)
