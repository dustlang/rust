#include "LLVMWrapper.h"
#include "llvm/ProfileData/Coverage/CoverageMapping.h"
#include "llvm/ProfileData/Coverage/CoverageMappingWriter.h"
#include "llvm/ProfileData/InstrProf.h"
#include "llvm/ADT/ArrayRef.h"

#include <iostream>

using namespace llvm;

struct LLVMDustCounterMappingRegion {
  coverage::Counter Count;
  uint32_t FileID;
  uint32_t ExpandedFileID;
  uint32_t LineStart;
  uint32_t ColumnStart;
  uint32_t LineEnd;
  uint32_t ColumnEnd;
  coverage::CounterMappingRegion::RegionKind Kind;
};

extern "C" void LLVMDustCoverageWriteFilenamesSectionToBuffer(
    const char* const Filenames[],
    size_t FilenamesLen,
    DustStringRef BufferOut) {
  SmallVector<StringRef,32> FilenameRefs;
  for (size_t i = 0; i < FilenamesLen; i++) {
    FilenameRefs.push_back(StringRef(Filenames[i]));
  }
  auto FilenamesWriter = coverage::CoverageFilenamesSectionWriter(
    makeArrayRef(FilenameRefs));
  RawDustStringOstream OS(BufferOut);
  FilenamesWriter.write(OS);
}

extern "C" void LLVMDustCoverageWriteMappingToBuffer(
    const unsigned *VirtualFileMappingIDs,
    unsigned NumVirtualFileMappingIDs,
    const coverage::CounterExpression *Expressions,
    unsigned NumExpressions,
    LLVMDustCounterMappingRegion *DustMappingRegions,
    unsigned NumMappingRegions,
    DustStringRef BufferOut) {
  // Convert from FFI representation to LLVM representation.
  SmallVector<coverage::CounterMappingRegion, 0> MappingRegions;
  MappingRegions.reserve(NumMappingRegions);
  for (const auto &Region : makeArrayRef(DustMappingRegions, NumMappingRegions)) {
    MappingRegions.emplace_back(
        Region.Count, Region.FileID, Region.ExpandedFileID,
        Region.LineStart, Region.ColumnStart, Region.LineEnd, Region.ColumnEnd,
        Region.Kind);
  }
  auto CoverageMappingWriter = coverage::CoverageMappingWriter(
      makeArrayRef(VirtualFileMappingIDs, NumVirtualFileMappingIDs),
      makeArrayRef(Expressions, NumExpressions),
      MappingRegions);
  RawDustStringOstream OS(BufferOut);
  CoverageMappingWriter.write(OS);
}

extern "C" LLVMValueRef LLVMDustCoverageCreatePGOFuncNameVar(LLVMValueRef F, const char *FuncName) {
  StringRef FuncNameRef(FuncName);
  return wrap(createPGOFuncNameVar(*cast<Function>(unwrap(F)), FuncNameRef));
}

extern "C" uint64_t LLVMDustCoverageHashCString(const char *StrVal) {
  StringRef StrRef(StrVal);
  return IndexedInstrProf::ComputeHash(StrRef);
}

extern "C" uint64_t LLVMDustCoverageHashByteArray(
    const char *Bytes,
    unsigned NumBytes) {
  StringRef StrRef(Bytes, NumBytes);
  return IndexedInstrProf::ComputeHash(StrRef);
}

static void WriteSectionNameToString(LLVMModuleRef M,
                                     InstrProfSectKind SK,
                                     DustStringRef Str) {
  Triple TargetTriple(unwrap(M)->getTargetTriple());
  auto name = getInstrProfSectionName(SK, TargetTriple.getObjectFormat());
  RawDustStringOstream OS(Str);
  OS << name;
}

extern "C" void LLVMDustCoverageWriteMapSectionNameToString(LLVMModuleRef M,
                                                            DustStringRef Str) {
  WriteSectionNameToString(M, IPSK_covmap, Str);
}

extern "C" void LLVMDustCoverageWriteFuncSectionNameToString(LLVMModuleRef M,
                                                             DustStringRef Str) {
#if LLVM_VERSION_GE(11, 0)
  WriteSectionNameToString(M, IPSK_covfun, Str);
// else do nothing; the `Version` check will abort codegen on the Dust side
#endif
}

extern "C" void LLVMDustCoverageWriteMappingVarNameToString(DustStringRef Str) {
  auto name = getCoverageMappingVarName();
  RawDustStringOstream OS(Str);
  OS << name;
}

extern "C" uint32_t LLVMDustCoverageMappingVersion() {
#if LLVM_VERSION_GE(11, 0)
  return coverage::CovMapVersion::Version4;
#else
  return coverage::CovMapVersion::Version3;
#endif
}
