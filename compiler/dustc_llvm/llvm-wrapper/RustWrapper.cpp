#include "LLVMWrapper.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Object/Archive.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Bitcode/BitcodeWriterPass.h"
#include "llvm/Support/Signals.h"
#include "llvm/ADT/Optional.h"

#include <iostream>

//===----------------------------------------------------------------------===
//
// This file defines alternate interfaces to core functions that are more
// readily callable by Dust's FFI.
//
//===----------------------------------------------------------------------===

using namespace llvm;
using namespace llvm::sys;
using namespace llvm::object;

// LLVMAtomicOrdering is already an enum - don't create another
// one.
static AtomicOrdering fromDust(LLVMAtomicOrdering Ordering) {
  switch (Ordering) {
  case LLVMAtomicOrderingNotAtomic:
    return AtomicOrdering::NotAtomic;
  case LLVMAtomicOrderingUnordered:
    return AtomicOrdering::Unordered;
  case LLVMAtomicOrderingMonotonic:
    return AtomicOrdering::Monotonic;
  case LLVMAtomicOrderingAcquire:
    return AtomicOrdering::Acquire;
  case LLVMAtomicOrderingRelease:
    return AtomicOrdering::Release;
  case LLVMAtomicOrderingAcquireRelease:
    return AtomicOrdering::AcquireRelease;
  case LLVMAtomicOrderingSequentiallyConsistent:
    return AtomicOrdering::SequentiallyConsistent;
  }

  report_fatal_error("Invalid LLVMAtomicOrdering value!");
}

static LLVM_THREAD_LOCAL char *LastError;

// Custom error handler for fatal LLVM errors.
//
// Notably it exits the process with code 101, unlike LLVM's default of 1.
static void FatalErrorHandler(void *UserData,
                              const std::string& Reason,
                              bool GenCrashDiag) {
  // Do the same thing that the default error handler does.
  std::cerr << "LLVM ERROR: " << Reason << std::endl;

  // Since this error handler exits the process, we have to run any cleanup that
  // LLVM would run after handling the error. This might change with an LLVM
  // upgrade.
  sys::RunInterruptHandlers();

  exit(101);
}

extern "C" void LLVMDustInstallFatalErrorHandler() {
  install_fatal_error_handler(FatalErrorHandler);
}

extern "C" LLVMMemoryBufferRef
LLVMDustCreateMemoryBufferWithContentsOfFile(const char *Path) {
  ErrorOr<std::unique_ptr<MemoryBuffer>> BufOr =
      MemoryBuffer::getFile(Path, -1, false);
  if (!BufOr) {
    LLVMDustSetLastError(BufOr.getError().message().c_str());
    return nullptr;
  }
  return wrap(BufOr.get().release());
}

extern "C" char *LLVMDustGetLastError(void) {
  char *Ret = LastError;
  LastError = nullptr;
  return Ret;
}

extern "C" unsigned int LLVMDustGetInstructionCount(LLVMModuleRef M) {
  return unwrap(M)->getInstructionCount();
}

extern "C" void LLVMDustSetLastError(const char *Err) {
  free((void *)LastError);
  LastError = strdup(Err);
}

extern "C" LLVMContextRef LLVMDustContextCreate(bool shouldDiscardNames) {
  auto ctx = new LLVMContext();
  ctx->setDiscardValueNames(shouldDiscardNames);
  return wrap(ctx);
}

extern "C" void LLVMDustSetNormalizedTarget(LLVMModuleRef M,
                                            const char *Triple) {
  unwrap(M)->setTargetTriple(Triple::normalize(Triple));
}

extern "C" void LLVMDustPrintPassTimings() {
  raw_fd_ostream OS(2, false); // stderr.
  TimerGroup::printAll(OS);
}

extern "C" LLVMValueRef LLVMDustGetNamedValue(LLVMModuleRef M, const char *Name,
                                              size_t NameLen) {
  return wrap(unwrap(M)->getNamedValue(StringRef(Name, NameLen)));
}

extern "C" LLVMValueRef LLVMDustGetOrInsertFunction(LLVMModuleRef M,
                                                    const char *Name,
                                                    size_t NameLen,
                                                    LLVMTypeRef FunctionTy) {
  return wrap(unwrap(M)
                  ->getOrInsertFunction(StringRef(Name, NameLen),
                                        unwrap<FunctionType>(FunctionTy))
                  .getCallee()
  );
}

extern "C" LLVMValueRef
LLVMDustGetOrInsertGlobal(LLVMModuleRef M, const char *Name, size_t NameLen, LLVMTypeRef Ty) {
  StringRef NameRef(Name, NameLen);
  return wrap(unwrap(M)->getOrInsertGlobal(NameRef, unwrap(Ty)));
}

extern "C" LLVMValueRef
LLVMDustInsertPrivateGlobal(LLVMModuleRef M, LLVMTypeRef Ty) {
  return wrap(new GlobalVariable(*unwrap(M),
                                 unwrap(Ty),
                                 false,
                                 GlobalValue::PrivateLinkage,
                                 nullptr));
}

extern "C" LLVMTypeRef LLVMDustMetadataTypeInContext(LLVMContextRef C) {
  return wrap(Type::getMetadataTy(*unwrap(C)));
}

static Attribute::AttrKind fromDust(LLVMDustAttribute Kind) {
  switch (Kind) {
  case AlwaysInline:
    return Attribute::AlwaysInline;
  case ByVal:
    return Attribute::ByVal;
  case Cold:
    return Attribute::Cold;
  case InlineHint:
    return Attribute::InlineHint;
  case MinSize:
    return Attribute::MinSize;
  case Naked:
    return Attribute::Naked;
  case NoAlias:
    return Attribute::NoAlias;
  case NoCapture:
    return Attribute::NoCapture;
  case NoInline:
    return Attribute::NoInline;
  case NonNull:
    return Attribute::NonNull;
  case NoRedZone:
    return Attribute::NoRedZone;
  case NoReturn:
    return Attribute::NoReturn;
  case NoUnwind:
    return Attribute::NoUnwind;
  case OptimizeForSize:
    return Attribute::OptimizeForSize;
  case ReadOnly:
    return Attribute::ReadOnly;
  case SExt:
    return Attribute::SExt;
  case StructRet:
    return Attribute::StructRet;
  case UWTable:
    return Attribute::UWTable;
  case ZExt:
    return Attribute::ZExt;
  case InReg:
    return Attribute::InReg;
  case SanitizeThread:
    return Attribute::SanitizeThread;
  case SanitizeAddress:
    return Attribute::SanitizeAddress;
  case SanitizeMemory:
    return Attribute::SanitizeMemory;
  case NonLazyBind:
    return Attribute::NonLazyBind;
  case OptimizeNone:
    return Attribute::OptimizeNone;
  case ReturnsTwice:
    return Attribute::ReturnsTwice;
  case ReadNone:
    return Attribute::ReadNone;
  case InaccessibleMemOnly:
    return Attribute::InaccessibleMemOnly;
  case SanitizeHWAddress:
    return Attribute::SanitizeHWAddress;
  case WillReturn:
    return Attribute::WillReturn;
  }
  report_fatal_error("bad AttributeKind");
}

extern "C" void LLVMDustAddCallSiteAttribute(LLVMValueRef Instr, unsigned Index,
                                             LLVMDustAttribute DustAttr) {
  CallBase *Call = unwrap<CallBase>(Instr);
  Attribute Attr = Attribute::get(Call->getContext(), fromDust(DustAttr));
  Call->addAttribute(Index, Attr);
}

extern "C" void LLVMDustAddCallSiteAttrString(LLVMValueRef Instr, unsigned Index,
                                              const char *Name) {
  CallBase *Call = unwrap<CallBase>(Instr);
  Attribute Attr = Attribute::get(Call->getContext(), Name);
  Call->addAttribute(Index, Attr);
}


extern "C" void LLVMDustAddAlignmentCallSiteAttr(LLVMValueRef Instr,
                                                 unsigned Index,
                                                 uint32_t Bytes) {
  CallBase *Call = unwrap<CallBase>(Instr);
  AttrBuilder B;
  B.addAlignmentAttr(Bytes);
  Call->setAttributes(Call->getAttributes().addAttributes(
      Call->getContext(), Index, B));
}

extern "C" void LLVMDustAddDereferenceableCallSiteAttr(LLVMValueRef Instr,
                                                       unsigned Index,
                                                       uint64_t Bytes) {
  CallBase *Call = unwrap<CallBase>(Instr);
  AttrBuilder B;
  B.addDereferenceableAttr(Bytes);
  Call->setAttributes(Call->getAttributes().addAttributes(
      Call->getContext(), Index, B));
}

extern "C" void LLVMDustAddDereferenceableOrNullCallSiteAttr(LLVMValueRef Instr,
                                                             unsigned Index,
                                                             uint64_t Bytes) {
  CallBase *Call = unwrap<CallBase>(Instr);
  AttrBuilder B;
  B.addDereferenceableOrNullAttr(Bytes);
  Call->setAttributes(Call->getAttributes().addAttributes(
      Call->getContext(), Index, B));
}

extern "C" void LLVMDustAddByValCallSiteAttr(LLVMValueRef Instr, unsigned Index,
                                             LLVMTypeRef Ty) {
  CallBase *Call = unwrap<CallBase>(Instr);
  Attribute Attr = Attribute::getWithByValType(Call->getContext(), unwrap(Ty));
  Call->addAttribute(Index, Attr);
}

extern "C" void LLVMDustAddStructRetCallSiteAttr(LLVMValueRef Instr, unsigned Index,
                                                 LLVMTypeRef Ty) {
  CallBase *Call = unwrap<CallBase>(Instr);
#if LLVM_VERSION_GE(12, 0)
  Attribute Attr = Attribute::getWithStructRetType(Call->getContext(), unwrap(Ty));
#else
  Attribute Attr = Attribute::get(Call->getContext(), Attribute::StructRet);
#endif
  Call->addAttribute(Index, Attr);
}

extern "C" void LLVMDustAddFunctionAttribute(LLVMValueRef Fn, unsigned Index,
                                             LLVMDustAttribute DustAttr) {
  Function *A = unwrap<Function>(Fn);
  Attribute Attr = Attribute::get(A->getContext(), fromDust(DustAttr));
  AttrBuilder B(Attr);
  A->addAttributes(Index, B);
}

extern "C" void LLVMDustAddAlignmentAttr(LLVMValueRef Fn,
                                         unsigned Index,
                                         uint32_t Bytes) {
  Function *A = unwrap<Function>(Fn);
  AttrBuilder B;
  B.addAlignmentAttr(Bytes);
  A->addAttributes(Index, B);
}

extern "C" void LLVMDustAddDereferenceableAttr(LLVMValueRef Fn, unsigned Index,
                                               uint64_t Bytes) {
  Function *A = unwrap<Function>(Fn);
  AttrBuilder B;
  B.addDereferenceableAttr(Bytes);
  A->addAttributes(Index, B);
}

extern "C" void LLVMDustAddDereferenceableOrNullAttr(LLVMValueRef Fn,
                                                     unsigned Index,
                                                     uint64_t Bytes) {
  Function *A = unwrap<Function>(Fn);
  AttrBuilder B;
  B.addDereferenceableOrNullAttr(Bytes);
  A->addAttributes(Index, B);
}

extern "C" void LLVMDustAddByValAttr(LLVMValueRef Fn, unsigned Index,
                                     LLVMTypeRef Ty) {
  Function *F = unwrap<Function>(Fn);
  Attribute Attr = Attribute::getWithByValType(F->getContext(), unwrap(Ty));
  F->addAttribute(Index, Attr);
}

extern "C" void LLVMDustAddStructRetAttr(LLVMValueRef Fn, unsigned Index,
                                         LLVMTypeRef Ty) {
  Function *F = unwrap<Function>(Fn);
#if LLVM_VERSION_GE(12, 0)
  Attribute Attr = Attribute::getWithStructRetType(F->getContext(), unwrap(Ty));
#else
  Attribute Attr = Attribute::get(F->getContext(), Attribute::StructRet);
#endif
  F->addAttribute(Index, Attr);
}

extern "C" void LLVMDustAddFunctionAttrStringValue(LLVMValueRef Fn,
                                                   unsigned Index,
                                                   const char *Name,
                                                   const char *Value) {
  Function *F = unwrap<Function>(Fn);
  AttrBuilder B;
  B.addAttribute(Name, Value);
  F->addAttributes(Index, B);
}

extern "C" void LLVMDustRemoveFunctionAttributes(LLVMValueRef Fn,
                                                 unsigned Index,
                                                 LLVMDustAttribute DustAttr) {
  Function *F = unwrap<Function>(Fn);
  Attribute Attr = Attribute::get(F->getContext(), fromDust(DustAttr));
  AttrBuilder B(Attr);
  auto PAL = F->getAttributes();
  auto PALNew = PAL.removeAttributes(F->getContext(), Index, B);
  F->setAttributes(PALNew);
}

// enable fpmath flag UnsafeAlgebra
extern "C" void LLVMDustSetHasUnsafeAlgebra(LLVMValueRef V) {
  if (auto I = dyn_cast<Instruction>(unwrap<Value>(V))) {
    I->setFast(true);
  }
}

extern "C" LLVMValueRef
LLVMDustBuildAtomicLoad(LLVMBuilderRef B, LLVMValueRef Source, const char *Name,
                        LLVMAtomicOrdering Order) {
  Value *Ptr = unwrap(Source);
  Type *Ty = Ptr->getType()->getPointerElementType();
  LoadInst *LI = unwrap(B)->CreateLoad(Ty, Ptr, Name);
  LI->setAtomic(fromDust(Order));
  return wrap(LI);
}

extern "C" LLVMValueRef LLVMDustBuildAtomicStore(LLVMBuilderRef B,
                                                 LLVMValueRef V,
                                                 LLVMValueRef Target,
                                                 LLVMAtomicOrdering Order) {
  StoreInst *SI = unwrap(B)->CreateStore(unwrap(V), unwrap(Target));
  SI->setAtomic(fromDust(Order));
  return wrap(SI);
}

// FIXME: Use the C-API LLVMBuildAtomicCmpXchg and LLVMSetWeak
// once we raise our minimum support to LLVM 10.
extern "C" LLVMValueRef
LLVMDustBuildAtomicCmpXchg(LLVMBuilderRef B, LLVMValueRef Target,
                           LLVMValueRef Old, LLVMValueRef Source,
                           LLVMAtomicOrdering Order,
                           LLVMAtomicOrdering FailureOrder, LLVMBool Weak) {
  AtomicCmpXchgInst *ACXI = unwrap(B)->CreateAtomicCmpXchg(
      unwrap(Target), unwrap(Old), unwrap(Source), fromDust(Order),
      fromDust(FailureOrder));
  ACXI->setWeak(Weak);
  return wrap(ACXI);
}

enum class LLVMDustSynchronizationScope {
  SingleThread,
  CrossThread,
};

static SyncScope::ID fromDust(LLVMDustSynchronizationScope Scope) {
  switch (Scope) {
  case LLVMDustSynchronizationScope::SingleThread:
    return SyncScope::SingleThread;
  case LLVMDustSynchronizationScope::CrossThread:
    return SyncScope::System;
  default:
    report_fatal_error("bad SynchronizationScope.");
  }
}

extern "C" LLVMValueRef
LLVMDustBuildAtomicFence(LLVMBuilderRef B, LLVMAtomicOrdering Order,
                         LLVMDustSynchronizationScope Scope) {
  return wrap(unwrap(B)->CreateFence(fromDust(Order), fromDust(Scope)));
}

enum class LLVMDustAsmDialect {
  Att,
  Intel,
};

static InlineAsm::AsmDialect fromDust(LLVMDustAsmDialect Dialect) {
  switch (Dialect) {
  case LLVMDustAsmDialect::Att:
    return InlineAsm::AD_ATT;
  case LLVMDustAsmDialect::Intel:
    return InlineAsm::AD_Intel;
  default:
    report_fatal_error("bad AsmDialect.");
  }
}

extern "C" LLVMValueRef
LLVMDustInlineAsm(LLVMTypeRef Ty, char *AsmString, size_t AsmStringLen,
                  char *Constraints, size_t ConstraintsLen,
                  LLVMBool HasSideEffects, LLVMBool IsAlignStack,
                  LLVMDustAsmDialect Dialect) {
  return wrap(InlineAsm::get(unwrap<FunctionType>(Ty),
                             StringRef(AsmString, AsmStringLen),
                             StringRef(Constraints, ConstraintsLen),
                             HasSideEffects, IsAlignStack, fromDust(Dialect)));
}

extern "C" bool LLVMDustInlineAsmVerify(LLVMTypeRef Ty, char *Constraints,
                                        size_t ConstraintsLen) {
  return InlineAsm::Verify(unwrap<FunctionType>(Ty),
                           StringRef(Constraints, ConstraintsLen));
}

extern "C" void LLVMDustAppendModuleInlineAsm(LLVMModuleRef M, const char *Asm,
                                              size_t AsmLen) {
  unwrap(M)->appendModuleInlineAsm(StringRef(Asm, AsmLen));
}

typedef DIBuilder *LLVMDustDIBuilderRef;

template <typename DIT> DIT *unwrapDIPtr(LLVMMetadataRef Ref) {
  return (DIT *)(Ref ? unwrap<MDNode>(Ref) : nullptr);
}

#define DIDescriptor DIScope
#define DIArray DINodeArray
#define unwrapDI unwrapDIPtr

// These values **must** match debuginfo::DIFlags! They also *happen*
// to match LLVM, but that isn't required as we do giant sets of
// matching below. The value shouldn't be directly passed to LLVM.
enum class LLVMDustDIFlags : uint32_t {
  FlagZero = 0,
  FlagPrivate = 1,
  FlagProtected = 2,
  FlagPublic = 3,
  FlagFwdDecl = (1 << 2),
  FlagAppleBlock = (1 << 3),
  FlagBlockByrefStruct = (1 << 4),
  FlagVirtual = (1 << 5),
  FlagArtificial = (1 << 6),
  FlagExplicit = (1 << 7),
  FlagPrototyped = (1 << 8),
  FlagObjcClassComplete = (1 << 9),
  FlagObjectPointer = (1 << 10),
  FlagVector = (1 << 11),
  FlagStaticMember = (1 << 12),
  FlagLValueReference = (1 << 13),
  FlagRValueReference = (1 << 14),
  FlagExternalTypeRef = (1 << 15),
  FlagIntroducedVirtual = (1 << 18),
  FlagBitField = (1 << 19),
  FlagNoReturn = (1 << 20),
  // Do not add values that are not supported by the minimum LLVM
  // version we support! see llvm/include/llvm/IR/DebugInfoFlags.def
};

inline LLVMDustDIFlags operator&(LLVMDustDIFlags A, LLVMDustDIFlags B) {
  return static_cast<LLVMDustDIFlags>(static_cast<uint32_t>(A) &
                                      static_cast<uint32_t>(B));
}

inline LLVMDustDIFlags operator|(LLVMDustDIFlags A, LLVMDustDIFlags B) {
  return static_cast<LLVMDustDIFlags>(static_cast<uint32_t>(A) |
                                      static_cast<uint32_t>(B));
}

inline LLVMDustDIFlags &operator|=(LLVMDustDIFlags &A, LLVMDustDIFlags B) {
  return A = A | B;
}

inline bool isSet(LLVMDustDIFlags F) { return F != LLVMDustDIFlags::FlagZero; }

inline LLVMDustDIFlags visibility(LLVMDustDIFlags F) {
  return static_cast<LLVMDustDIFlags>(static_cast<uint32_t>(F) & 0x3);
}

static DINode::DIFlags fromDust(LLVMDustDIFlags Flags) {
  DINode::DIFlags Result = DINode::DIFlags::FlagZero;

  switch (visibility(Flags)) {
  case LLVMDustDIFlags::FlagPrivate:
    Result |= DINode::DIFlags::FlagPrivate;
    break;
  case LLVMDustDIFlags::FlagProtected:
    Result |= DINode::DIFlags::FlagProtected;
    break;
  case LLVMDustDIFlags::FlagPublic:
    Result |= DINode::DIFlags::FlagPublic;
    break;
  default:
    // The rest are handled below
    break;
  }

  if (isSet(Flags & LLVMDustDIFlags::FlagFwdDecl)) {
    Result |= DINode::DIFlags::FlagFwdDecl;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagAppleBlock)) {
    Result |= DINode::DIFlags::FlagAppleBlock;
  }
#if LLVM_VERSION_LT(10, 0)
  if (isSet(Flags & LLVMDustDIFlags::FlagBlockByrefStruct)) {
    Result |= DINode::DIFlags::FlagBlockByrefStruct;
  }
#endif
  if (isSet(Flags & LLVMDustDIFlags::FlagVirtual)) {
    Result |= DINode::DIFlags::FlagVirtual;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagArtificial)) {
    Result |= DINode::DIFlags::FlagArtificial;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagExplicit)) {
    Result |= DINode::DIFlags::FlagExplicit;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagPrototyped)) {
    Result |= DINode::DIFlags::FlagPrototyped;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagObjcClassComplete)) {
    Result |= DINode::DIFlags::FlagObjcClassComplete;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagObjectPointer)) {
    Result |= DINode::DIFlags::FlagObjectPointer;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagVector)) {
    Result |= DINode::DIFlags::FlagVector;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagStaticMember)) {
    Result |= DINode::DIFlags::FlagStaticMember;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagLValueReference)) {
    Result |= DINode::DIFlags::FlagLValueReference;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagRValueReference)) {
    Result |= DINode::DIFlags::FlagRValueReference;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagIntroducedVirtual)) {
    Result |= DINode::DIFlags::FlagIntroducedVirtual;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagBitField)) {
    Result |= DINode::DIFlags::FlagBitField;
  }
  if (isSet(Flags & LLVMDustDIFlags::FlagNoReturn)) {
    Result |= DINode::DIFlags::FlagNoReturn;
  }

  return Result;
}

// These values **must** match debuginfo::DISPFlags! They also *happen*
// to match LLVM, but that isn't required as we do giant sets of
// matching below. The value shouldn't be directly passed to LLVM.
enum class LLVMDustDISPFlags : uint32_t {
  SPFlagZero = 0,
  SPFlagVirtual = 1,
  SPFlagPureVirtual = 2,
  SPFlagLocalToUnit = (1 << 2),
  SPFlagDefinition = (1 << 3),
  SPFlagOptimized = (1 << 4),
  SPFlagMainSubprogram = (1 << 5),
  // Do not add values that are not supported by the minimum LLVM
  // version we support! see llvm/include/llvm/IR/DebugInfoFlags.def
  // (In LLVM < 8, createFunction supported these as separate bool arguments.)
};

inline LLVMDustDISPFlags operator&(LLVMDustDISPFlags A, LLVMDustDISPFlags B) {
  return static_cast<LLVMDustDISPFlags>(static_cast<uint32_t>(A) &
                                      static_cast<uint32_t>(B));
}

inline LLVMDustDISPFlags operator|(LLVMDustDISPFlags A, LLVMDustDISPFlags B) {
  return static_cast<LLVMDustDISPFlags>(static_cast<uint32_t>(A) |
                                      static_cast<uint32_t>(B));
}

inline LLVMDustDISPFlags &operator|=(LLVMDustDISPFlags &A, LLVMDustDISPFlags B) {
  return A = A | B;
}

inline bool isSet(LLVMDustDISPFlags F) { return F != LLVMDustDISPFlags::SPFlagZero; }

inline LLVMDustDISPFlags virtuality(LLVMDustDISPFlags F) {
  return static_cast<LLVMDustDISPFlags>(static_cast<uint32_t>(F) & 0x3);
}

static DISubprogram::DISPFlags fromDust(LLVMDustDISPFlags SPFlags) {
  DISubprogram::DISPFlags Result = DISubprogram::DISPFlags::SPFlagZero;

  switch (virtuality(SPFlags)) {
  case LLVMDustDISPFlags::SPFlagVirtual:
    Result |= DISubprogram::DISPFlags::SPFlagVirtual;
    break;
  case LLVMDustDISPFlags::SPFlagPureVirtual:
    Result |= DISubprogram::DISPFlags::SPFlagPureVirtual;
    break;
  default:
    // The rest are handled below
    break;
  }

  if (isSet(SPFlags & LLVMDustDISPFlags::SPFlagLocalToUnit)) {
    Result |= DISubprogram::DISPFlags::SPFlagLocalToUnit;
  }
  if (isSet(SPFlags & LLVMDustDISPFlags::SPFlagDefinition)) {
    Result |= DISubprogram::DISPFlags::SPFlagDefinition;
  }
  if (isSet(SPFlags & LLVMDustDISPFlags::SPFlagOptimized)) {
    Result |= DISubprogram::DISPFlags::SPFlagOptimized;
  }
  if (isSet(SPFlags & LLVMDustDISPFlags::SPFlagMainSubprogram)) {
    Result |= DISubprogram::DISPFlags::SPFlagMainSubprogram;
  }

  return Result;
}

enum class LLVMDustDebugEmissionKind {
  NoDebug,
  FullDebug,
  LineTablesOnly,
};

static DICompileUnit::DebugEmissionKind fromDust(LLVMDustDebugEmissionKind Kind) {
  switch (Kind) {
  case LLVMDustDebugEmissionKind::NoDebug:
    return DICompileUnit::DebugEmissionKind::NoDebug;
  case LLVMDustDebugEmissionKind::FullDebug:
    return DICompileUnit::DebugEmissionKind::FullDebug;
  case LLVMDustDebugEmissionKind::LineTablesOnly:
    return DICompileUnit::DebugEmissionKind::LineTablesOnly;
  default:
    report_fatal_error("bad DebugEmissionKind.");
  }
}

enum class LLVMDustChecksumKind {
  None,
  MD5,
  SHA1,
  SHA256,
};

static Optional<DIFile::ChecksumKind> fromDust(LLVMDustChecksumKind Kind) {
  switch (Kind) {
  case LLVMDustChecksumKind::None:
    return None;
  case LLVMDustChecksumKind::MD5:
    return DIFile::ChecksumKind::CSK_MD5;
  case LLVMDustChecksumKind::SHA1:
    return DIFile::ChecksumKind::CSK_SHA1;
#if (LLVM_VERSION_MAJOR >= 11)
  case LLVMDustChecksumKind::SHA256:
    return DIFile::ChecksumKind::CSK_SHA256;
#endif
  default:
    report_fatal_error("bad ChecksumKind.");
  }
}

extern "C" uint32_t LLVMDustDebugMetadataVersion() {
  return DEBUG_METADATA_VERSION;
}

extern "C" uint32_t LLVMDustVersionPatch() { return LLVM_VERSION_PATCH; }

extern "C" uint32_t LLVMDustVersionMinor() { return LLVM_VERSION_MINOR; }

extern "C" uint32_t LLVMDustVersionMajor() { return LLVM_VERSION_MAJOR; }

extern "C" void LLVMDustAddModuleFlag(LLVMModuleRef M, const char *Name,
                                      uint32_t Value) {
  unwrap(M)->addModuleFlag(Module::Warning, Name, Value);
}

extern "C" LLVMValueRef LLVMDustMetadataAsValue(LLVMContextRef C, LLVMMetadataRef MD) {
  return wrap(MetadataAsValue::get(*unwrap(C), unwrap(MD)));
}

extern "C" LLVMDustDIBuilderRef LLVMDustDIBuilderCreate(LLVMModuleRef M) {
  return new DIBuilder(*unwrap(M));
}

extern "C" void LLVMDustDIBuilderDispose(LLVMDustDIBuilderRef Builder) {
  delete Builder;
}

extern "C" void LLVMDustDIBuilderFinalize(LLVMDustDIBuilderRef Builder) {
  Builder->finalize();
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateCompileUnit(
    LLVMDustDIBuilderRef Builder, unsigned Lang, LLVMMetadataRef FileRef,
    const char *Producer, size_t ProducerLen, bool isOptimized,
    const char *Flags, unsigned RuntimeVer,
    const char *SplitName, size_t SplitNameLen,
    LLVMDustDebugEmissionKind Kind,
    uint64_t DWOId, bool SplitDebugInlining) {
  auto *File = unwrapDI<DIFile>(FileRef);

  return wrap(Builder->createCompileUnit(Lang, File, StringRef(Producer, ProducerLen),
                                         isOptimized, Flags, RuntimeVer,
                                         StringRef(SplitName, SplitNameLen),
                                         fromDust(Kind), DWOId, SplitDebugInlining));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateFile(
    LLVMDustDIBuilderRef Builder,
    const char *Filename, size_t FilenameLen,
    const char *Directory, size_t DirectoryLen, LLVMDustChecksumKind CSKind,
    const char *Checksum, size_t ChecksumLen) {
  Optional<DIFile::ChecksumKind> llvmCSKind = fromDust(CSKind);
  Optional<DIFile::ChecksumInfo<StringRef>> CSInfo{};
  if (llvmCSKind)
    CSInfo.emplace(*llvmCSKind, StringRef{Checksum, ChecksumLen});
  return wrap(Builder->createFile(StringRef(Filename, FilenameLen),
                                  StringRef(Directory, DirectoryLen),
                                  CSInfo));
}

extern "C" LLVMMetadataRef
LLVMDustDIBuilderCreateSubroutineType(LLVMDustDIBuilderRef Builder,
                                      LLVMMetadataRef ParameterTypes) {
  return wrap(Builder->createSubroutineType(
      DITypeRefArray(unwrap<MDTuple>(ParameterTypes))));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateFunction(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen,
    const char *LinkageName, size_t LinkageNameLen,
    LLVMMetadataRef File, unsigned LineNo,
    LLVMMetadataRef Ty, unsigned ScopeLine, LLVMDustDIFlags Flags,
    LLVMDustDISPFlags SPFlags, LLVMValueRef MaybeFn, LLVMMetadataRef TParam,
    LLVMMetadataRef Decl) {
  DITemplateParameterArray TParams =
      DITemplateParameterArray(unwrap<MDTuple>(TParam));
  DISubprogram::DISPFlags llvmSPFlags = fromDust(SPFlags);
  DINode::DIFlags llvmFlags = fromDust(Flags);
  DISubprogram *Sub = Builder->createFunction(
      unwrapDI<DIScope>(Scope),
      StringRef(Name, NameLen),
      StringRef(LinkageName, LinkageNameLen),
      unwrapDI<DIFile>(File), LineNo,
      unwrapDI<DISubroutineType>(Ty), ScopeLine, llvmFlags,
      llvmSPFlags, TParams, unwrapDIPtr<DISubprogram>(Decl));
  if (MaybeFn)
    unwrap<Function>(MaybeFn)->setSubprogram(Sub);
  return wrap(Sub);
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateBasicType(
    LLVMDustDIBuilderRef Builder, const char *Name, size_t NameLen,
    uint64_t SizeInBits, unsigned Encoding) {
  return wrap(Builder->createBasicType(StringRef(Name, NameLen), SizeInBits, Encoding));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateTypedef(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Type, const char *Name, size_t NameLen,
    LLVMMetadataRef File, unsigned LineNo, LLVMMetadataRef Scope) {
  return wrap(Builder->createTypedef(
    unwrap<DIType>(Type), StringRef(Name, NameLen), unwrap<DIFile>(File),
    LineNo, unwrapDIPtr<DIScope>(Scope)));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreatePointerType(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef PointeeTy,
    uint64_t SizeInBits, uint32_t AlignInBits, unsigned AddressSpace,
    const char *Name, size_t NameLen) {
  return wrap(Builder->createPointerType(unwrapDI<DIType>(PointeeTy),
                                         SizeInBits, AlignInBits,
                                         AddressSpace,
                                         StringRef(Name, NameLen)));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateStructType(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen,
    LLVMMetadataRef File, unsigned LineNumber, uint64_t SizeInBits,
    uint32_t AlignInBits, LLVMDustDIFlags Flags,
    LLVMMetadataRef DerivedFrom, LLVMMetadataRef Elements,
    unsigned RunTimeLang, LLVMMetadataRef VTableHolder,
    const char *UniqueId, size_t UniqueIdLen) {
  return wrap(Builder->createStructType(
      unwrapDI<DIDescriptor>(Scope), StringRef(Name, NameLen),
      unwrapDI<DIFile>(File), LineNumber,
      SizeInBits, AlignInBits, fromDust(Flags), unwrapDI<DIType>(DerivedFrom),
      DINodeArray(unwrapDI<MDTuple>(Elements)), RunTimeLang,
      unwrapDI<DIType>(VTableHolder), StringRef(UniqueId, UniqueIdLen)));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateVariantPart(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen,
    LLVMMetadataRef File, unsigned LineNumber, uint64_t SizeInBits,
    uint32_t AlignInBits, LLVMDustDIFlags Flags, LLVMMetadataRef Discriminator,
    LLVMMetadataRef Elements, const char *UniqueId, size_t UniqueIdLen) {
  return wrap(Builder->createVariantPart(
      unwrapDI<DIDescriptor>(Scope), StringRef(Name, NameLen),
      unwrapDI<DIFile>(File), LineNumber,
      SizeInBits, AlignInBits, fromDust(Flags), unwrapDI<DIDerivedType>(Discriminator),
      DINodeArray(unwrapDI<MDTuple>(Elements)), StringRef(UniqueId, UniqueIdLen)));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateMemberType(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen,
    LLVMMetadataRef File, unsigned LineNo, uint64_t SizeInBits,
    uint32_t AlignInBits, uint64_t OffsetInBits, LLVMDustDIFlags Flags,
    LLVMMetadataRef Ty) {
  return wrap(Builder->createMemberType(unwrapDI<DIDescriptor>(Scope),
                                        StringRef(Name, NameLen),
                                        unwrapDI<DIFile>(File), LineNo,
                                        SizeInBits, AlignInBits, OffsetInBits,
                                        fromDust(Flags), unwrapDI<DIType>(Ty)));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateVariantMemberType(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen, LLVMMetadataRef File, unsigned LineNo,
    uint64_t SizeInBits, uint32_t AlignInBits, uint64_t OffsetInBits, LLVMValueRef Discriminant,
    LLVMDustDIFlags Flags, LLVMMetadataRef Ty) {
  llvm::ConstantInt* D = nullptr;
  if (Discriminant) {
    D = unwrap<llvm::ConstantInt>(Discriminant);
  }
  return wrap(Builder->createVariantMemberType(unwrapDI<DIDescriptor>(Scope),
                                               StringRef(Name, NameLen),
                                               unwrapDI<DIFile>(File), LineNo,
                                               SizeInBits, AlignInBits, OffsetInBits, D,
                                               fromDust(Flags), unwrapDI<DIType>(Ty)));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateLexicalBlock(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    LLVMMetadataRef File, unsigned Line, unsigned Col) {
  return wrap(Builder->createLexicalBlock(unwrapDI<DIDescriptor>(Scope),
                                          unwrapDI<DIFile>(File), Line, Col));
}

extern "C" LLVMMetadataRef
LLVMDustDIBuilderCreateLexicalBlockFile(LLVMDustDIBuilderRef Builder,
                                        LLVMMetadataRef Scope,
                                        LLVMMetadataRef File) {
  return wrap(Builder->createLexicalBlockFile(unwrapDI<DIDescriptor>(Scope),
                                              unwrapDI<DIFile>(File)));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateStaticVariable(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Context,
    const char *Name, size_t NameLen,
    const char *LinkageName, size_t LinkageNameLen,
    LLVMMetadataRef File, unsigned LineNo,
    LLVMMetadataRef Ty, bool IsLocalToUnit, LLVMValueRef V,
    LLVMMetadataRef Decl = nullptr, uint32_t AlignInBits = 0) {
  llvm::GlobalVariable *InitVal = cast<llvm::GlobalVariable>(unwrap(V));

  llvm::DIExpression *InitExpr = nullptr;
  if (llvm::ConstantInt *IntVal = llvm::dyn_cast<llvm::ConstantInt>(InitVal)) {
    InitExpr = Builder->createConstantValueExpression(
        IntVal->getValue().getSExtValue());
  } else if (llvm::ConstantFP *FPVal =
                 llvm::dyn_cast<llvm::ConstantFP>(InitVal)) {
    InitExpr = Builder->createConstantValueExpression(
        FPVal->getValueAPF().bitcastToAPInt().getZExtValue());
  }

  llvm::DIGlobalVariableExpression *VarExpr = Builder->createGlobalVariableExpression(
      unwrapDI<DIDescriptor>(Context), StringRef(Name, NameLen),
      StringRef(LinkageName, LinkageNameLen),
      unwrapDI<DIFile>(File), LineNo, unwrapDI<DIType>(Ty), IsLocalToUnit,
#if LLVM_VERSION_GE(10, 0)
      /* isDefined */ true,
#endif
      InitExpr, unwrapDIPtr<MDNode>(Decl),
      /* templateParams */ nullptr,
      AlignInBits);

  InitVal->setMetadata("dbg", VarExpr);

  return wrap(VarExpr);
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateVariable(
    LLVMDustDIBuilderRef Builder, unsigned Tag, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen,
    LLVMMetadataRef File, unsigned LineNo,
    LLVMMetadataRef Ty, bool AlwaysPreserve, LLVMDustDIFlags Flags,
    unsigned ArgNo, uint32_t AlignInBits) {
  if (Tag == 0x100) { // DW_TAG_auto_variable
    return wrap(Builder->createAutoVariable(
        unwrapDI<DIDescriptor>(Scope), StringRef(Name, NameLen),
        unwrapDI<DIFile>(File), LineNo,
        unwrapDI<DIType>(Ty), AlwaysPreserve, fromDust(Flags), AlignInBits));
  } else {
    return wrap(Builder->createParameterVariable(
        unwrapDI<DIDescriptor>(Scope), StringRef(Name, NameLen), ArgNo,
        unwrapDI<DIFile>(File), LineNo,
        unwrapDI<DIType>(Ty), AlwaysPreserve, fromDust(Flags)));
  }
}

extern "C" LLVMMetadataRef
LLVMDustDIBuilderCreateArrayType(LLVMDustDIBuilderRef Builder, uint64_t Size,
                                 uint32_t AlignInBits, LLVMMetadataRef Ty,
                                 LLVMMetadataRef Subscripts) {
  return wrap(
      Builder->createArrayType(Size, AlignInBits, unwrapDI<DIType>(Ty),
                               DINodeArray(unwrapDI<MDTuple>(Subscripts))));
}

extern "C" LLVMMetadataRef
LLVMDustDIBuilderGetOrCreateSubrange(LLVMDustDIBuilderRef Builder, int64_t Lo,
                                     int64_t Count) {
  return wrap(Builder->getOrCreateSubrange(Lo, Count));
}

extern "C" LLVMMetadataRef
LLVMDustDIBuilderGetOrCreateArray(LLVMDustDIBuilderRef Builder,
                                  LLVMMetadataRef *Ptr, unsigned Count) {
  Metadata **DataValue = unwrap(Ptr);
  return wrap(
      Builder->getOrCreateArray(ArrayRef<Metadata *>(DataValue, Count)).get());
}

extern "C" LLVMValueRef LLVMDustDIBuilderInsertDeclareAtEnd(
    LLVMDustDIBuilderRef Builder, LLVMValueRef V, LLVMMetadataRef VarInfo,
    int64_t *AddrOps, unsigned AddrOpsCount, LLVMMetadataRef DL,
    LLVMBasicBlockRef InsertAtEnd) {
  return wrap(Builder->insertDeclare(
      unwrap(V), unwrap<DILocalVariable>(VarInfo),
      Builder->createExpression(llvm::ArrayRef<int64_t>(AddrOps, AddrOpsCount)),
      DebugLoc(cast<MDNode>(unwrap(DL))),
      unwrap(InsertAtEnd)));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateEnumerator(
    LLVMDustDIBuilderRef Builder, const char *Name, size_t NameLen,
    int64_t Value, bool IsUnsigned) {
  return wrap(Builder->createEnumerator(StringRef(Name, NameLen), Value, IsUnsigned));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateEnumerationType(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen,
    LLVMMetadataRef File, unsigned LineNumber, uint64_t SizeInBits,
    uint32_t AlignInBits, LLVMMetadataRef Elements,
    LLVMMetadataRef ClassTy, bool IsScoped) {
  return wrap(Builder->createEnumerationType(
      unwrapDI<DIDescriptor>(Scope), StringRef(Name, NameLen),
      unwrapDI<DIFile>(File), LineNumber,
      SizeInBits, AlignInBits, DINodeArray(unwrapDI<MDTuple>(Elements)),
      unwrapDI<DIType>(ClassTy), "", IsScoped));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateUnionType(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen,
    LLVMMetadataRef File, unsigned LineNumber, uint64_t SizeInBits,
    uint32_t AlignInBits, LLVMDustDIFlags Flags, LLVMMetadataRef Elements,
    unsigned RunTimeLang, const char *UniqueId, size_t UniqueIdLen) {
  return wrap(Builder->createUnionType(
      unwrapDI<DIDescriptor>(Scope), StringRef(Name, NameLen), unwrapDI<DIFile>(File),
      LineNumber, SizeInBits, AlignInBits, fromDust(Flags),
      DINodeArray(unwrapDI<MDTuple>(Elements)), RunTimeLang,
      StringRef(UniqueId, UniqueIdLen)));
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateTemplateTypeParameter(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen, LLVMMetadataRef Ty) {
#if LLVM_VERSION_GE(11, 0)
  bool IsDefault = false; // FIXME: should we ever set this true?
  return wrap(Builder->createTemplateTypeParameter(
      unwrapDI<DIDescriptor>(Scope), StringRef(Name, NameLen), unwrapDI<DIType>(Ty), IsDefault));
#else
  return wrap(Builder->createTemplateTypeParameter(
      unwrapDI<DIDescriptor>(Scope), StringRef(Name, NameLen), unwrapDI<DIType>(Ty)));
#endif
}

extern "C" LLVMMetadataRef LLVMDustDIBuilderCreateNameSpace(
    LLVMDustDIBuilderRef Builder, LLVMMetadataRef Scope,
    const char *Name, size_t NameLen, bool ExportSymbols) {
  return wrap(Builder->createNameSpace(
      unwrapDI<DIDescriptor>(Scope), StringRef(Name, NameLen), ExportSymbols
  ));
}

extern "C" void
LLVMDustDICompositeTypeReplaceArrays(LLVMDustDIBuilderRef Builder,
                                     LLVMMetadataRef CompositeTy,
                                     LLVMMetadataRef Elements,
                                     LLVMMetadataRef Params) {
  DICompositeType *Tmp = unwrapDI<DICompositeType>(CompositeTy);
  Builder->replaceArrays(Tmp, DINodeArray(unwrap<MDTuple>(Elements)),
                         DINodeArray(unwrap<MDTuple>(Params)));
}

extern "C" LLVMMetadataRef
LLVMDustDIBuilderCreateDebugLocation(unsigned Line, unsigned Column,
                                     LLVMMetadataRef ScopeRef,
                                     LLVMMetadataRef InlinedAt) {
#if LLVM_VERSION_GE(12, 0)
  MDNode *Scope = unwrapDIPtr<MDNode>(ScopeRef);
  DILocation *Loc = DILocation::get(
      Scope->getContext(), Line, Column, Scope,
      unwrapDIPtr<MDNode>(InlinedAt));
  return wrap(Loc);
#else
  DebugLoc debug_loc = DebugLoc::get(Line, Column, unwrapDIPtr<MDNode>(ScopeRef),
                                     unwrapDIPtr<MDNode>(InlinedAt));
  return wrap(debug_loc.getAsMDNode());
#endif
}

extern "C" int64_t LLVMDustDIBuilderCreateOpDeref() {
  return dwarf::DW_OP_deref;
}

extern "C" int64_t LLVMDustDIBuilderCreateOpPlusUconst() {
  return dwarf::DW_OP_plus_uconst;
}

extern "C" void LLVMDustWriteTypeToString(LLVMTypeRef Ty, DustStringRef Str) {
  RawDustStringOstream OS(Str);
  unwrap<llvm::Type>(Ty)->print(OS);
}

extern "C" void LLVMDustWriteValueToString(LLVMValueRef V,
                                           DustStringRef Str) {
  RawDustStringOstream OS(Str);
  if (!V) {
    OS << "(null)";
  } else {
    OS << "(";
    unwrap<llvm::Value>(V)->getType()->print(OS);
    OS << ":";
    unwrap<llvm::Value>(V)->print(OS);
    OS << ")";
  }
}

// Note that the two following functions look quite similar to the
// LLVMGetSectionName function. Sadly, it appears that this function only
// returns a char* pointer, which isn't guaranteed to be null-terminated. The
// function provided by LLVM doesn't return the length, so we've created our own
// function which returns the length as well as the data pointer.
//
// For an example of this not returning a null terminated string, see
// lib/Object/COFFObjectFile.cpp in the getSectionName function. One of the
// branches explicitly creates a StringRef without a null terminator, and then
// that's returned.

inline section_iterator *unwrap(LLVMSectionIteratorRef SI) {
  return reinterpret_cast<section_iterator *>(SI);
}

extern "C" size_t LLVMDustGetSectionName(LLVMSectionIteratorRef SI,
                                         const char **Ptr) {
#if LLVM_VERSION_GE(10, 0)
  auto NameOrErr = (*unwrap(SI))->getName();
  if (!NameOrErr)
    report_fatal_error(NameOrErr.takeError());
  *Ptr = NameOrErr->data();
  return NameOrErr->size();
#else
  StringRef Ret;
  if (std::error_code EC = (*unwrap(SI))->getName(Ret))
    report_fatal_error(EC.message());
  *Ptr = Ret.data();
  return Ret.size();
#endif
}

// LLVMArrayType function does not support 64-bit ElementCount
extern "C" LLVMTypeRef LLVMDustArrayType(LLVMTypeRef ElementTy,
                                         uint64_t ElementCount) {
  return wrap(ArrayType::get(unwrap(ElementTy), ElementCount));
}

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(Twine, LLVMTwineRef)

extern "C" void LLVMDustWriteTwineToString(LLVMTwineRef T, DustStringRef Str) {
  RawDustStringOstream OS(Str);
  unwrap(T)->print(OS);
}

extern "C" void LLVMDustUnpackOptimizationDiagnostic(
    LLVMDiagnosticInfoRef DI, DustStringRef PassNameOut,
    LLVMValueRef *FunctionOut, unsigned* Line, unsigned* Column,
    DustStringRef FilenameOut, DustStringRef MessageOut) {
  // Undefined to call this not on an optimization diagnostic!
  llvm::DiagnosticInfoOptimizationBase *Opt =
      static_cast<llvm::DiagnosticInfoOptimizationBase *>(unwrap(DI));

  RawDustStringOstream PassNameOS(PassNameOut);
  PassNameOS << Opt->getPassName();
  *FunctionOut = wrap(&Opt->getFunction());

  RawDustStringOstream FilenameOS(FilenameOut);
  DiagnosticLocation loc = Opt->getLocation();
  if (loc.isValid()) {
    *Line = loc.getLine();
    *Column = loc.getColumn();
    FilenameOS << loc.getAbsolutePath();
  }

  RawDustStringOstream MessageOS(MessageOut);
  MessageOS << Opt->getMsg();
}

enum class LLVMDustDiagnosticLevel {
    Error,
    Warning,
    Note,
    Remark,
};

extern "C" void
LLVMDustUnpackInlineAsmDiagnostic(LLVMDiagnosticInfoRef DI,
                                  LLVMDustDiagnosticLevel *LevelOut,
                                  unsigned *CookieOut,
                                  LLVMTwineRef *MessageOut,
                                  LLVMValueRef *InstructionOut) {
  // Undefined to call this not on an inline assembly diagnostic!
  llvm::DiagnosticInfoInlineAsm *IA =
      static_cast<llvm::DiagnosticInfoInlineAsm *>(unwrap(DI));

  *CookieOut = IA->getLocCookie();
  *MessageOut = wrap(&IA->getMsgStr());
  *InstructionOut = wrap(IA->getInstruction());

  switch (IA->getSeverity()) {
    case DS_Error:
      *LevelOut = LLVMDustDiagnosticLevel::Error;
      break;
    case DS_Warning:
      *LevelOut = LLVMDustDiagnosticLevel::Warning;
      break;
    case DS_Note:
      *LevelOut = LLVMDustDiagnosticLevel::Note;
      break;
    case DS_Remark:
      *LevelOut = LLVMDustDiagnosticLevel::Remark;
      break;
    default:
      report_fatal_error("Invalid LLVMDustDiagnosticLevel value!");
  }
}

extern "C" void LLVMDustWriteDiagnosticInfoToString(LLVMDiagnosticInfoRef DI,
                                                    DustStringRef Str) {
  RawDustStringOstream OS(Str);
  DiagnosticPrinterRawOStream DP(OS);
  unwrap(DI)->print(DP);
}

enum class LLVMDustDiagnosticKind {
  Other,
  InlineAsm,
  StackSize,
  DebugMetadataVersion,
  SampleProfile,
  OptimizationRemark,
  OptimizationRemarkMissed,
  OptimizationRemarkAnalysis,
  OptimizationRemarkAnalysisFPCommute,
  OptimizationRemarkAnalysisAliasing,
  OptimizationRemarkOther,
  OptimizationFailure,
  PGOProfile,
  Linker,
  Unsupported,
};

static LLVMDustDiagnosticKind toDust(DiagnosticKind Kind) {
  switch (Kind) {
  case DK_InlineAsm:
    return LLVMDustDiagnosticKind::InlineAsm;
  case DK_StackSize:
    return LLVMDustDiagnosticKind::StackSize;
  case DK_DebugMetadataVersion:
    return LLVMDustDiagnosticKind::DebugMetadataVersion;
  case DK_SampleProfile:
    return LLVMDustDiagnosticKind::SampleProfile;
  case DK_OptimizationRemark:
    return LLVMDustDiagnosticKind::OptimizationRemark;
  case DK_OptimizationRemarkMissed:
    return LLVMDustDiagnosticKind::OptimizationRemarkMissed;
  case DK_OptimizationRemarkAnalysis:
    return LLVMDustDiagnosticKind::OptimizationRemarkAnalysis;
  case DK_OptimizationRemarkAnalysisFPCommute:
    return LLVMDustDiagnosticKind::OptimizationRemarkAnalysisFPCommute;
  case DK_OptimizationRemarkAnalysisAliasing:
    return LLVMDustDiagnosticKind::OptimizationRemarkAnalysisAliasing;
  case DK_PGOProfile:
    return LLVMDustDiagnosticKind::PGOProfile;
  case DK_Linker:
    return LLVMDustDiagnosticKind::Linker;
  case DK_Unsupported:
    return LLVMDustDiagnosticKind::Unsupported;
  default:
    return (Kind >= DK_FirstRemark && Kind <= DK_LastRemark)
               ? LLVMDustDiagnosticKind::OptimizationRemarkOther
               : LLVMDustDiagnosticKind::Other;
  }
}

extern "C" LLVMDustDiagnosticKind
LLVMDustGetDiagInfoKind(LLVMDiagnosticInfoRef DI) {
  return toDust((DiagnosticKind)unwrap(DI)->getKind());
}

// This is kept distinct from LLVMGetTypeKind, because when
// a new type kind is added, the Dust-side enum must be
// updated or UB will result.
extern "C" LLVMTypeKind LLVMDustGetTypeKind(LLVMTypeRef Ty) {
  switch (unwrap(Ty)->getTypeID()) {
  case Type::VoidTyID:
    return LLVMVoidTypeKind;
  case Type::HalfTyID:
    return LLVMHalfTypeKind;
  case Type::FloatTyID:
    return LLVMFloatTypeKind;
  case Type::DoubleTyID:
    return LLVMDoubleTypeKind;
  case Type::X86_FP80TyID:
    return LLVMX86_FP80TypeKind;
  case Type::FP128TyID:
    return LLVMFP128TypeKind;
  case Type::PPC_FP128TyID:
    return LLVMPPC_FP128TypeKind;
  case Type::LabelTyID:
    return LLVMLabelTypeKind;
  case Type::MetadataTyID:
    return LLVMMetadataTypeKind;
  case Type::IntegerTyID:
    return LLVMIntegerTypeKind;
  case Type::FunctionTyID:
    return LLVMFunctionTypeKind;
  case Type::StructTyID:
    return LLVMStructTypeKind;
  case Type::ArrayTyID:
    return LLVMArrayTypeKind;
  case Type::PointerTyID:
    return LLVMPointerTypeKind;
#if LLVM_VERSION_GE(11, 0)
  case Type::FixedVectorTyID:
    return LLVMVectorTypeKind;
#else
  case Type::VectorTyID:
    return LLVMVectorTypeKind;
#endif
  case Type::X86_MMXTyID:
    return LLVMX86_MMXTypeKind;
  case Type::TokenTyID:
    return LLVMTokenTypeKind;
#if LLVM_VERSION_GE(11, 0)
  case Type::ScalableVectorTyID:
    return LLVMScalableVectorTypeKind;
  case Type::BFloatTyID:
    return LLVMBFloatTypeKind;
#endif
#if LLVM_VERSION_GE(12, 0)
  case Type::X86_AMXTyID:
    return LLVMX86_AMXTypeKind;
#endif
  }
  report_fatal_error("Unhandled TypeID.");
}

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(SMDiagnostic, LLVMSMDiagnosticRef)

extern "C" void LLVMDustSetInlineAsmDiagnosticHandler(
    LLVMContextRef C, LLVMContext::InlineAsmDiagHandlerTy H, void *CX) {
  unwrap(C)->setInlineAsmDiagnosticHandler(H, CX);
}

extern "C" bool LLVMDustUnpackSMDiagnostic(LLVMSMDiagnosticRef DRef,
                                           DustStringRef MessageOut,
                                           DustStringRef BufferOut,
                                           LLVMDustDiagnosticLevel* LevelOut,
                                           unsigned* LocOut,
                                           unsigned* RangesOut,
                                           size_t* NumRanges) {
  SMDiagnostic& D = *unwrap(DRef);
  RawDustStringOstream MessageOS(MessageOut);
  MessageOS << D.getMessage();

  switch (D.getKind()) {
    case SourceMgr::DK_Error:
      *LevelOut = LLVMDustDiagnosticLevel::Error;
      break;
    case SourceMgr::DK_Warning:
      *LevelOut = LLVMDustDiagnosticLevel::Warning;
      break;
    case SourceMgr::DK_Note:
      *LevelOut = LLVMDustDiagnosticLevel::Note;
      break;
    case SourceMgr::DK_Remark:
      *LevelOut = LLVMDustDiagnosticLevel::Remark;
      break;
    default:
      report_fatal_error("Invalid LLVMDustDiagnosticLevel value!");
  }

  if (D.getLoc() == SMLoc())
    return false;

  const SourceMgr &LSM = *D.getSourceMgr();
  const MemoryBuffer *LBuf = LSM.getMemoryBuffer(LSM.FindBufferContainingLoc(D.getLoc()));
  LLVMDustStringWriteImpl(BufferOut, LBuf->getBufferStart(), LBuf->getBufferSize());

  *LocOut = D.getLoc().getPointer() - LBuf->getBufferStart();

  *NumRanges = std::min(*NumRanges, D.getRanges().size());
  size_t LineStart = *LocOut - (size_t)D.getColumnNo();
  for (size_t i = 0; i < *NumRanges; i++) {
    RangesOut[i * 2] = LineStart + D.getRanges()[i].first;
    RangesOut[i * 2 + 1] = LineStart + D.getRanges()[i].second;
  }

  return true;
}

extern "C" LLVMValueRef LLVMDustBuildCleanupPad(LLVMBuilderRef B,
                                                LLVMValueRef ParentPad,
                                                unsigned ArgCount,
                                                LLVMValueRef *LLArgs,
                                                const char *Name) {
  Value **Args = unwrap(LLArgs);
  if (ParentPad == nullptr) {
    Type *Ty = Type::getTokenTy(unwrap(B)->getContext());
    ParentPad = wrap(Constant::getNullValue(Ty));
  }
  return wrap(unwrap(B)->CreateCleanupPad(
      unwrap(ParentPad), ArrayRef<Value *>(Args, ArgCount), Name));
}

extern "C" LLVMValueRef LLVMDustBuildCleanupRet(LLVMBuilderRef B,
                                                LLVMValueRef CleanupPad,
                                                LLVMBasicBlockRef UnwindBB) {
  CleanupPadInst *Inst = cast<CleanupPadInst>(unwrap(CleanupPad));
  return wrap(unwrap(B)->CreateCleanupRet(Inst, unwrap(UnwindBB)));
}

extern "C" LLVMValueRef
LLVMDustBuildCatchPad(LLVMBuilderRef B, LLVMValueRef ParentPad,
                      unsigned ArgCount, LLVMValueRef *LLArgs, const char *Name) {
  Value **Args = unwrap(LLArgs);
  return wrap(unwrap(B)->CreateCatchPad(
      unwrap(ParentPad), ArrayRef<Value *>(Args, ArgCount), Name));
}

extern "C" LLVMValueRef LLVMDustBuildCatchRet(LLVMBuilderRef B,
                                              LLVMValueRef Pad,
                                              LLVMBasicBlockRef BB) {
  return wrap(unwrap(B)->CreateCatchRet(cast<CatchPadInst>(unwrap(Pad)),
                                              unwrap(BB)));
}

extern "C" LLVMValueRef LLVMDustBuildCatchSwitch(LLVMBuilderRef B,
                                                 LLVMValueRef ParentPad,
                                                 LLVMBasicBlockRef BB,
                                                 unsigned NumHandlers,
                                                 const char *Name) {
  if (ParentPad == nullptr) {
    Type *Ty = Type::getTokenTy(unwrap(B)->getContext());
    ParentPad = wrap(Constant::getNullValue(Ty));
  }
  return wrap(unwrap(B)->CreateCatchSwitch(unwrap(ParentPad), unwrap(BB),
                                                 NumHandlers, Name));
}

extern "C" void LLVMDustAddHandler(LLVMValueRef CatchSwitchRef,
                                   LLVMBasicBlockRef Handler) {
  Value *CatchSwitch = unwrap(CatchSwitchRef);
  cast<CatchSwitchInst>(CatchSwitch)->addHandler(unwrap(Handler));
}

extern "C" OperandBundleDef *LLVMDustBuildOperandBundleDef(const char *Name,
                                                           LLVMValueRef *Inputs,
                                                           unsigned NumInputs) {
  return new OperandBundleDef(Name, makeArrayRef(unwrap(Inputs), NumInputs));
}

extern "C" void LLVMDustFreeOperandBundleDef(OperandBundleDef *Bundle) {
  delete Bundle;
}

extern "C" LLVMValueRef LLVMDustBuildCall(LLVMBuilderRef B, LLVMValueRef Fn,
                                          LLVMValueRef *Args, unsigned NumArgs,
                                          OperandBundleDef *Bundle) {
  Value *Callee = unwrap(Fn);
  FunctionType *FTy = cast<FunctionType>(Callee->getType()->getPointerElementType());
  unsigned Len = Bundle ? 1 : 0;
  ArrayRef<OperandBundleDef> Bundles = makeArrayRef(Bundle, Len);
  return wrap(unwrap(B)->CreateCall(
      FTy, Callee, makeArrayRef(unwrap(Args), NumArgs), Bundles));
}

extern "C" LLVMValueRef LLVMDustGetInstrProfIncrementIntrinsic(LLVMModuleRef M) {
  return wrap(llvm::Intrinsic::getDeclaration(unwrap(M),
              (llvm::Intrinsic::ID)llvm::Intrinsic::instrprof_increment));
}

extern "C" LLVMValueRef LLVMDustBuildMemCpy(LLVMBuilderRef B,
                                            LLVMValueRef Dst, unsigned DstAlign,
                                            LLVMValueRef Src, unsigned SrcAlign,
                                            LLVMValueRef Size, bool IsVolatile) {
#if LLVM_VERSION_GE(10, 0)
  return wrap(unwrap(B)->CreateMemCpy(
      unwrap(Dst), MaybeAlign(DstAlign),
      unwrap(Src), MaybeAlign(SrcAlign),
      unwrap(Size), IsVolatile));
#else
  return wrap(unwrap(B)->CreateMemCpy(
      unwrap(Dst), DstAlign,
      unwrap(Src), SrcAlign,
      unwrap(Size), IsVolatile));
#endif
}

extern "C" LLVMValueRef LLVMDustBuildMemMove(LLVMBuilderRef B,
                                             LLVMValueRef Dst, unsigned DstAlign,
                                             LLVMValueRef Src, unsigned SrcAlign,
                                             LLVMValueRef Size, bool IsVolatile) {
#if LLVM_VERSION_GE(10, 0)
  return wrap(unwrap(B)->CreateMemMove(
      unwrap(Dst), MaybeAlign(DstAlign),
      unwrap(Src), MaybeAlign(SrcAlign),
      unwrap(Size), IsVolatile));
#else
  return wrap(unwrap(B)->CreateMemMove(
      unwrap(Dst), DstAlign,
      unwrap(Src), SrcAlign,
      unwrap(Size), IsVolatile));
#endif
}

extern "C" LLVMValueRef LLVMDustBuildMemSet(LLVMBuilderRef B,
                                            LLVMValueRef Dst, unsigned DstAlign,
                                            LLVMValueRef Val,
                                            LLVMValueRef Size, bool IsVolatile) {
#if LLVM_VERSION_GE(10, 0)
  return wrap(unwrap(B)->CreateMemSet(
      unwrap(Dst), unwrap(Val), unwrap(Size), MaybeAlign(DstAlign), IsVolatile));
#else
  return wrap(unwrap(B)->CreateMemSet(
      unwrap(Dst), unwrap(Val), unwrap(Size), DstAlign, IsVolatile));
#endif
}

extern "C" LLVMValueRef
LLVMDustBuildInvoke(LLVMBuilderRef B, LLVMValueRef Fn, LLVMValueRef *Args,
                    unsigned NumArgs, LLVMBasicBlockRef Then,
                    LLVMBasicBlockRef Catch, OperandBundleDef *Bundle,
                    const char *Name) {
  Value *Callee = unwrap(Fn);
  FunctionType *FTy = cast<FunctionType>(Callee->getType()->getPointerElementType());
  unsigned Len = Bundle ? 1 : 0;
  ArrayRef<OperandBundleDef> Bundles = makeArrayRef(Bundle, Len);
  return wrap(unwrap(B)->CreateInvoke(FTy, Callee, unwrap(Then), unwrap(Catch),
                                      makeArrayRef(unwrap(Args), NumArgs),
                                      Bundles, Name));
}

extern "C" void LLVMDustPositionBuilderAtStart(LLVMBuilderRef B,
                                               LLVMBasicBlockRef BB) {
  auto Point = unwrap(BB)->getFirstInsertionPt();
  unwrap(B)->SetInsertPoint(unwrap(BB), Point);
}

extern "C" void LLVMDustSetComdat(LLVMModuleRef M, LLVMValueRef V,
                                  const char *Name, size_t NameLen) {
  Triple TargetTriple(unwrap(M)->getTargetTriple());
  GlobalObject *GV = unwrap<GlobalObject>(V);
  if (TargetTriple.supportsCOMDAT()) {
    StringRef NameRef(Name, NameLen);
    GV->setComdat(unwrap(M)->getOrInsertComdat(NameRef));
  }
}

extern "C" void LLVMDustUnsetComdat(LLVMValueRef V) {
  GlobalObject *GV = unwrap<GlobalObject>(V);
  GV->setComdat(nullptr);
}

enum class LLVMDustLinkage {
  ExternalLinkage = 0,
  AvailableExternallyLinkage = 1,
  LinkOnceAnyLinkage = 2,
  LinkOnceODRLinkage = 3,
  WeakAnyLinkage = 4,
  WeakODRLinkage = 5,
  AppendingLinkage = 6,
  InternalLinkage = 7,
  PrivateLinkage = 8,
  ExternalWeakLinkage = 9,
  CommonLinkage = 10,
};

static LLVMDustLinkage toDust(LLVMLinkage Linkage) {
  switch (Linkage) {
  case LLVMExternalLinkage:
    return LLVMDustLinkage::ExternalLinkage;
  case LLVMAvailableExternallyLinkage:
    return LLVMDustLinkage::AvailableExternallyLinkage;
  case LLVMLinkOnceAnyLinkage:
    return LLVMDustLinkage::LinkOnceAnyLinkage;
  case LLVMLinkOnceODRLinkage:
    return LLVMDustLinkage::LinkOnceODRLinkage;
  case LLVMWeakAnyLinkage:
    return LLVMDustLinkage::WeakAnyLinkage;
  case LLVMWeakODRLinkage:
    return LLVMDustLinkage::WeakODRLinkage;
  case LLVMAppendingLinkage:
    return LLVMDustLinkage::AppendingLinkage;
  case LLVMInternalLinkage:
    return LLVMDustLinkage::InternalLinkage;
  case LLVMPrivateLinkage:
    return LLVMDustLinkage::PrivateLinkage;
  case LLVMExternalWeakLinkage:
    return LLVMDustLinkage::ExternalWeakLinkage;
  case LLVMCommonLinkage:
    return LLVMDustLinkage::CommonLinkage;
  default:
    report_fatal_error("Invalid LLVMDustLinkage value!");
  }
}

static LLVMLinkage fromDust(LLVMDustLinkage Linkage) {
  switch (Linkage) {
  case LLVMDustLinkage::ExternalLinkage:
    return LLVMExternalLinkage;
  case LLVMDustLinkage::AvailableExternallyLinkage:
    return LLVMAvailableExternallyLinkage;
  case LLVMDustLinkage::LinkOnceAnyLinkage:
    return LLVMLinkOnceAnyLinkage;
  case LLVMDustLinkage::LinkOnceODRLinkage:
    return LLVMLinkOnceODRLinkage;
  case LLVMDustLinkage::WeakAnyLinkage:
    return LLVMWeakAnyLinkage;
  case LLVMDustLinkage::WeakODRLinkage:
    return LLVMWeakODRLinkage;
  case LLVMDustLinkage::AppendingLinkage:
    return LLVMAppendingLinkage;
  case LLVMDustLinkage::InternalLinkage:
    return LLVMInternalLinkage;
  case LLVMDustLinkage::PrivateLinkage:
    return LLVMPrivateLinkage;
  case LLVMDustLinkage::ExternalWeakLinkage:
    return LLVMExternalWeakLinkage;
  case LLVMDustLinkage::CommonLinkage:
    return LLVMCommonLinkage;
  }
  report_fatal_error("Invalid LLVMDustLinkage value!");
}

extern "C" LLVMDustLinkage LLVMDustGetLinkage(LLVMValueRef V) {
  return toDust(LLVMGetLinkage(V));
}

extern "C" void LLVMDustSetLinkage(LLVMValueRef V,
                                   LLVMDustLinkage DustLinkage) {
  LLVMSetLinkage(V, fromDust(DustLinkage));
}

// Returns true if both high and low were successfully set. Fails in case constant wasn’t any of
// the common sizes (1, 8, 16, 32, 64, 128 bits)
extern "C" bool LLVMDustConstInt128Get(LLVMValueRef CV, bool sext, uint64_t *high, uint64_t *low)
{
    auto C = unwrap<llvm::ConstantInt>(CV);
    if (C->getBitWidth() > 128) { return false; }
    APInt AP;
    if (sext) {
        AP = C->getValue().sextOrSelf(128);
    } else {
        AP = C->getValue().zextOrSelf(128);
    }
    *low = AP.getLoBits(64).getZExtValue();
    *high = AP.getHiBits(64).getZExtValue();
    return true;
}

enum class LLVMDustVisibility {
  Default = 0,
  Hidden = 1,
  Protected = 2,
};

static LLVMDustVisibility toDust(LLVMVisibility Vis) {
  switch (Vis) {
  case LLVMDefaultVisibility:
    return LLVMDustVisibility::Default;
  case LLVMHiddenVisibility:
    return LLVMDustVisibility::Hidden;
  case LLVMProtectedVisibility:
    return LLVMDustVisibility::Protected;
  }
  report_fatal_error("Invalid LLVMDustVisibility value!");
}

static LLVMVisibility fromDust(LLVMDustVisibility Vis) {
  switch (Vis) {
  case LLVMDustVisibility::Default:
    return LLVMDefaultVisibility;
  case LLVMDustVisibility::Hidden:
    return LLVMHiddenVisibility;
  case LLVMDustVisibility::Protected:
    return LLVMProtectedVisibility;
  }
  report_fatal_error("Invalid LLVMDustVisibility value!");
}

extern "C" LLVMDustVisibility LLVMDustGetVisibility(LLVMValueRef V) {
  return toDust(LLVMGetVisibility(V));
}

// Oh hey, a binding that makes sense for once? (because LLVM’s own do not)
extern "C" LLVMValueRef LLVMDustBuildIntCast(LLVMBuilderRef B, LLVMValueRef Val,
                                             LLVMTypeRef DestTy, bool isSigned) {
  return wrap(unwrap(B)->CreateIntCast(unwrap(Val), unwrap(DestTy), isSigned, ""));
}

extern "C" void LLVMDustSetVisibility(LLVMValueRef V,
                                      LLVMDustVisibility DustVisibility) {
  LLVMSetVisibility(V, fromDust(DustVisibility));
}

struct LLVMDustModuleBuffer {
  std::string data;
};

extern "C" LLVMDustModuleBuffer*
LLVMDustModuleBufferCreate(LLVMModuleRef M) {
#if LLVM_VERSION_GE(10, 0)
  auto Ret = std::make_unique<LLVMDustModuleBuffer>();
#else
  auto Ret = llvm::make_unique<LLVMDustModuleBuffer>();
#endif
  {
    raw_string_ostream OS(Ret->data);
    {
      legacy::PassManager PM;
      PM.add(createBitcodeWriterPass(OS));
      PM.run(*unwrap(M));
    }
  }
  return Ret.release();
}

extern "C" void
LLVMDustModuleBufferFree(LLVMDustModuleBuffer *Buffer) {
  delete Buffer;
}

extern "C" const void*
LLVMDustModuleBufferPtr(const LLVMDustModuleBuffer *Buffer) {
  return Buffer->data.data();
}

extern "C" size_t
LLVMDustModuleBufferLen(const LLVMDustModuleBuffer *Buffer) {
  return Buffer->data.length();
}

extern "C" uint64_t
LLVMDustModuleCost(LLVMModuleRef M) {
  auto f = unwrap(M)->functions();
  return std::distance(std::begin(f), std::end(f));
}

// Vector reductions:
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceFAdd(LLVMBuilderRef B, LLVMValueRef Acc, LLVMValueRef Src) {
    return wrap(unwrap(B)->CreateFAddReduce(unwrap(Acc),unwrap(Src)));
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceFMul(LLVMBuilderRef B, LLVMValueRef Acc, LLVMValueRef Src) {
    return wrap(unwrap(B)->CreateFMulReduce(unwrap(Acc),unwrap(Src)));
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceAdd(LLVMBuilderRef B, LLVMValueRef Src) {
    return wrap(unwrap(B)->CreateAddReduce(unwrap(Src)));
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceMul(LLVMBuilderRef B, LLVMValueRef Src) {
    return wrap(unwrap(B)->CreateMulReduce(unwrap(Src)));
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceAnd(LLVMBuilderRef B, LLVMValueRef Src) {
    return wrap(unwrap(B)->CreateAndReduce(unwrap(Src)));
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceOr(LLVMBuilderRef B, LLVMValueRef Src) {
    return wrap(unwrap(B)->CreateOrReduce(unwrap(Src)));
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceXor(LLVMBuilderRef B, LLVMValueRef Src) {
    return wrap(unwrap(B)->CreateXorReduce(unwrap(Src)));
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceMin(LLVMBuilderRef B, LLVMValueRef Src, bool IsSigned) {
    return wrap(unwrap(B)->CreateIntMinReduce(unwrap(Src), IsSigned));
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceMax(LLVMBuilderRef B, LLVMValueRef Src, bool IsSigned) {
    return wrap(unwrap(B)->CreateIntMaxReduce(unwrap(Src), IsSigned));
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceFMin(LLVMBuilderRef B, LLVMValueRef Src, bool NoNaN) {
#if LLVM_VERSION_GE(12, 0)
  Instruction *I = unwrap(B)->CreateFPMinReduce(unwrap(Src));
  I->setHasNoNaNs(NoNaN);
  return wrap(I);
#else
  return wrap(unwrap(B)->CreateFPMinReduce(unwrap(Src), NoNaN));
#endif
}
extern "C" LLVMValueRef
LLVMDustBuildVectorReduceFMax(LLVMBuilderRef B, LLVMValueRef Src, bool NoNaN) {
#if LLVM_VERSION_GE(12, 0)
  Instruction *I = unwrap(B)->CreateFPMaxReduce(unwrap(Src));
  I->setHasNoNaNs(NoNaN);
  return wrap(I);
#else
  return wrap(unwrap(B)->CreateFPMaxReduce(unwrap(Src), NoNaN));
#endif
}

extern "C" LLVMValueRef
LLVMDustBuildMinNum(LLVMBuilderRef B, LLVMValueRef LHS, LLVMValueRef RHS) {
    return wrap(unwrap(B)->CreateMinNum(unwrap(LHS),unwrap(RHS)));
}
extern "C" LLVMValueRef
LLVMDustBuildMaxNum(LLVMBuilderRef B, LLVMValueRef LHS, LLVMValueRef RHS) {
    return wrap(unwrap(B)->CreateMaxNum(unwrap(LHS),unwrap(RHS)));
}
