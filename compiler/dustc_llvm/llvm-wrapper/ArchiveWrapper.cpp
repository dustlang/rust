#include "LLVMWrapper.h"

#include "llvm/Object/Archive.h"
#include "llvm/Object/ArchiveWriter.h"
#include "llvm/Support/Path.h"

using namespace llvm;
using namespace llvm::object;

struct DustArchiveMember {
  const char *Filename;
  const char *Name;
  Archive::Child Child;

  DustArchiveMember()
      : Filename(nullptr), Name(nullptr),
        Child(nullptr, nullptr, nullptr)
  {
  }
  ~DustArchiveMember() {}
};

struct DustArchiveIterator {
  bool First;
  Archive::child_iterator Cur;
  Archive::child_iterator End;
  std::unique_ptr<Error> Err;

  DustArchiveIterator(Archive::child_iterator Cur, Archive::child_iterator End,
      std::unique_ptr<Error> Err)
    : First(true),
      Cur(Cur),
      End(End),
      Err(std::move(Err)) {}
};

enum class LLVMDustArchiveKind {
  GNU,
  BSD,
  DARWIN,
  COFF,
};

static Archive::Kind fromDust(LLVMDustArchiveKind Kind) {
  switch (Kind) {
  case LLVMDustArchiveKind::GNU:
    return Archive::K_GNU;
  case LLVMDustArchiveKind::BSD:
    return Archive::K_BSD;
  case LLVMDustArchiveKind::DARWIN:
    return Archive::K_DARWIN;
  case LLVMDustArchiveKind::COFF:
    return Archive::K_COFF;
  default:
    report_fatal_error("Bad ArchiveKind.");
  }
}

typedef OwningBinary<Archive> *LLVMDustArchiveRef;
typedef DustArchiveMember *LLVMDustArchiveMemberRef;
typedef Archive::Child *LLVMDustArchiveChildRef;
typedef Archive::Child const *LLVMDustArchiveChildConstRef;
typedef DustArchiveIterator *LLVMDustArchiveIteratorRef;

extern "C" LLVMDustArchiveRef LLVMDustOpenArchive(char *Path) {
  ErrorOr<std::unique_ptr<MemoryBuffer>> BufOr =
      MemoryBuffer::getFile(Path, -1, false);
  if (!BufOr) {
    LLVMDustSetLastError(BufOr.getError().message().c_str());
    return nullptr;
  }

  Expected<std::unique_ptr<Archive>> ArchiveOr =
      Archive::create(BufOr.get()->getMemBufferRef());

  if (!ArchiveOr) {
    LLVMDustSetLastError(toString(ArchiveOr.takeError()).c_str());
    return nullptr;
  }

  OwningBinary<Archive> *Ret = new OwningBinary<Archive>(
      std::move(ArchiveOr.get()), std::move(BufOr.get()));

  return Ret;
}

extern "C" void LLVMDustDestroyArchive(LLVMDustArchiveRef DustArchive) {
  delete DustArchive;
}

extern "C" LLVMDustArchiveIteratorRef
LLVMDustArchiveIteratorNew(LLVMDustArchiveRef DustArchive) {
  Archive *Archive = DustArchive->getBinary();
#if LLVM_VERSION_GE(10, 0)
  std::unique_ptr<Error> Err = std::make_unique<Error>(Error::success());
#else
  std::unique_ptr<Error> Err = llvm::make_unique<Error>(Error::success());
#endif
  auto Cur = Archive->child_begin(*Err);
  if (*Err) {
    LLVMDustSetLastError(toString(std::move(*Err)).c_str());
    return nullptr;
  }
  auto End = Archive->child_end();
  return new DustArchiveIterator(Cur, End, std::move(Err));
}

extern "C" LLVMDustArchiveChildConstRef
LLVMDustArchiveIteratorNext(LLVMDustArchiveIteratorRef RAI) {
  if (RAI->Cur == RAI->End)
    return nullptr;

  // Advancing the iterator validates the next child, and this can
  // uncover an error. LLVM requires that we check all Errors,
  // so we only advance the iterator if we actually need to fetch
  // the next child.
  // This means we must not advance the iterator in the *first* call,
  // but instead advance it *before* fetching the child in all later calls.
  if (!RAI->First) {
    ++RAI->Cur;
    if (*RAI->Err) {
      LLVMDustSetLastError(toString(std::move(*RAI->Err)).c_str());
      return nullptr;
    }
  } else {
    RAI->First = false;
  }

  if (RAI->Cur == RAI->End)
    return nullptr;

  const Archive::Child &Child = *RAI->Cur.operator->();
  Archive::Child *Ret = new Archive::Child(Child);

  return Ret;
}

extern "C" void LLVMDustArchiveChildFree(LLVMDustArchiveChildRef Child) {
  delete Child;
}

extern "C" void LLVMDustArchiveIteratorFree(LLVMDustArchiveIteratorRef RAI) {
  delete RAI;
}

extern "C" const char *
LLVMDustArchiveChildName(LLVMDustArchiveChildConstRef Child, size_t *Size) {
  Expected<StringRef> NameOrErr = Child->getName();
  if (!NameOrErr) {
    // dustc_codegen_llvm currently doesn't use this error string, but it might be
    // useful in the future, and in the mean time this tells LLVM that the
    // error was not ignored and that it shouldn't abort the process.
    LLVMDustSetLastError(toString(NameOrErr.takeError()).c_str());
    return nullptr;
  }
  StringRef Name = NameOrErr.get();
  *Size = Name.size();
  return Name.data();
}

extern "C" const char *LLVMDustArchiveChildData(LLVMDustArchiveChildRef Child,
                                                size_t *Size) {
  StringRef Buf;
  Expected<StringRef> BufOrErr = Child->getBuffer();
  if (!BufOrErr) {
    LLVMDustSetLastError(toString(BufOrErr.takeError()).c_str());
    return nullptr;
  }
  Buf = BufOrErr.get();
  *Size = Buf.size();
  return Buf.data();
}

extern "C" LLVMDustArchiveMemberRef
LLVMDustArchiveMemberNew(char *Filename, char *Name,
                         LLVMDustArchiveChildRef Child) {
  DustArchiveMember *Member = new DustArchiveMember;
  Member->Filename = Filename;
  Member->Name = Name;
  if (Child)
    Member->Child = *Child;
  return Member;
}

extern "C" void LLVMDustArchiveMemberFree(LLVMDustArchiveMemberRef Member) {
  delete Member;
}

extern "C" LLVMDustResult
LLVMDustWriteArchive(char *Dst, size_t NumMembers,
                     const LLVMDustArchiveMemberRef *NewMembers,
                     bool WriteSymbtab, LLVMDustArchiveKind DustKind) {

  std::vector<NewArchiveMember> Members;
  auto Kind = fromDust(DustKind);

  for (size_t I = 0; I < NumMembers; I++) {
    auto Member = NewMembers[I];
    assert(Member->Name);
    if (Member->Filename) {
      Expected<NewArchiveMember> MOrErr =
          NewArchiveMember::getFile(Member->Filename, true);
      if (!MOrErr) {
        LLVMDustSetLastError(toString(MOrErr.takeError()).c_str());
        return LLVMDustResult::Failure;
      }
      MOrErr->MemberName = sys::path::filename(MOrErr->MemberName);
      Members.push_back(std::move(*MOrErr));
    } else {
      Expected<NewArchiveMember> MOrErr =
          NewArchiveMember::getOldMember(Member->Child, true);
      if (!MOrErr) {
        LLVMDustSetLastError(toString(MOrErr.takeError()).c_str());
        return LLVMDustResult::Failure;
      }
      Members.push_back(std::move(*MOrErr));
    }
  }

  auto Result = writeArchive(Dst, Members, WriteSymbtab, Kind, true, false);
  if (!Result)
    return LLVMDustResult::Success;
  LLVMDustSetLastError(toString(std::move(Result)).c_str());

  return LLVMDustResult::Failure;
}
