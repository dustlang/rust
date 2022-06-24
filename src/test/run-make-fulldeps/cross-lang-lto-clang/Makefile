# needs-matching-clang

# This test makes sure that cross-language inlining actually works by checking
# the generated machine code.

-include ../tools.mk

all: cpp-executable dust-executable

cpp-executable:
	$(DUSTC) -Clinker-plugin-lto=on -o $(TMPDIR)/libdustlib-xlto.a -Copt-level=2 -Ccodegen-units=1 ./dustlib.rs
	$(CLANG) -flto=thin -fuse-ld=lld -L $(TMPDIR) -ldustlib-xlto -o $(TMPDIR)/cmain ./cmain.c -O3
	# Make sure we don't find a call instruction to the function we expect to
	# always be inlined.
	"$(LLVM_BIN_DIR)"/llvm-objdump -d $(TMPDIR)/cmain | $(CGREP) -v -e "call.*dust_always_inlined"
	# As a sanity check, make sure we do find a call instruction to a
	# non-inlined function
	"$(LLVM_BIN_DIR)"/llvm-objdump -d $(TMPDIR)/cmain | $(CGREP) -e "call.*dust_never_inlined"

dust-executable:
	$(CLANG) ./clib.c -flto=thin -c -o $(TMPDIR)/clib.o -O2
	(cd $(TMPDIR); $(AR) crus ./libxyz.a ./clib.o)
	$(DUSTC) -Clinker-plugin-lto=on -L$(TMPDIR) -Copt-level=2 -Clinker=$(CLANG) -Clink-arg=-fuse-ld=lld ./main.rs -o $(TMPDIR)/rsmain
	"$(LLVM_BIN_DIR)"/llvm-objdump -d $(TMPDIR)/rsmain | $(CGREP) -e "call.*c_never_inlined"
	"$(LLVM_BIN_DIR)"/llvm-objdump -d $(TMPDIR)/rsmain | $(CGREP) -v -e "call.*c_always_inlined"
