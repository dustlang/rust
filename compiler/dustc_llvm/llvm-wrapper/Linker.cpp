#include "llvm/Linker/Linker.h"

#include "LLVMWrapper.h"

using namespace llvm;

struct DustLinker {
  Linker L;
  LLVMContext &Ctx;

  DustLinker(Module &M) :
    L(M),
    Ctx(M.getContext())
  {}
};

extern "C" DustLinker*
LLVMDustLinkerNew(LLVMModuleRef DstRef) {
  Module *Dst = unwrap(DstRef);

  return new DustLinker(*Dst);
}

extern "C" void
LLVMDustLinkerFree(DustLinker *L) {
  delete L;
}

extern "C" bool
LLVMDustLinkerAdd(DustLinker *L, char *BC, size_t Len) {
  std::unique_ptr<MemoryBuffer> Buf =
      MemoryBuffer::getMemBufferCopy(StringRef(BC, Len));

  Expected<std::unique_ptr<Module>> SrcOrError =
      llvm::getLazyBitcodeModule(Buf->getMemBufferRef(), L->Ctx);
  if (!SrcOrError) {
    LLVMDustSetLastError(toString(SrcOrError.takeError()).c_str());
    return false;
  }

  auto Src = std::move(*SrcOrError);

  if (L->L.linkInModule(std::move(Src))) {
    LLVMDustSetLastError("");
    return false;
  }
  return true;
}
