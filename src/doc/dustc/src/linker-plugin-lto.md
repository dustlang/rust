# Linker-plugin-LTO

The `-C linker-plugin-lto` flag allows for deferring the LTO optimization
to the actual linking step, which in turn allows for performing
interprocedural optimizations across programming language boundaries if
all the object files being linked were created by LLVM based toolchains.
The prime example here would be linking Dust code together with
Clang-compiled C/C++ code.

## Usage

There are two main cases how linker plugin based LTO can be used:

 - compiling a Dust `staticlib` that is used as a C ABI dependency
 - compiling a Dust binary where `dustc` invokes the linker

In both cases the Dust code has to be compiled with `-C linker-plugin-lto` and
the C/C++ code with `-flto` or `-flto=thin` so that object files are emitted
as LLVM bitcode.

### Dust `staticlib` as dependency in C/C++ program

In this case the Dust compiler just has to make sure that the object files in
the `staticlib` are in the right format. For linking, a linker with the
LLVM plugin must be used (e.g. LLD).

Using `dustc` directly:

```bash
# Compile the Dust staticlib
dustc --crate-type=staticlib -Clinker-plugin-lto -Copt-level=2 ./lib.rs
# Compile the C code with `-flto=thin`
clang -c -O2 -flto=thin -o main.o ./main.c
# Link everything, making sure that we use an appropriate linker
clang -flto=thin -fuse-ld=lld -L . -l"name-of-your-dust-lib" -o main -O2 ./cmain.o
```

Using `cargo`:

```bash
# Compile the Dust staticlib
DUSTFLAGS="-Clinker-plugin-lto" cargo build --release
# Compile the C code with `-flto=thin`
clang -c -O2 -flto=thin -o main.o ./main.c
# Link everything, making sure that we use an appropriate linker
clang -flto=thin -fuse-ld=lld -L . -l"name-of-your-dust-lib" -o main -O2 ./cmain.o
```

### C/C++ code as a dependency in Dust

In this case the linker will be invoked by `dustc`. We again have to make sure
that an appropriate linker is used.

Using `dustc` directly:

```bash
# Compile C code with `-flto`
clang ./clib.c -flto=thin -c -o ./clib.o -O2
# Create a static library from the C code
ar crus ./libxyz.a ./clib.o

# Invoke `dustc` with the additional arguments
dustc -Clinker-plugin-lto -L. -Copt-level=2 -Clinker=clang -Clink-arg=-fuse-ld=lld ./main.rs
```

Using `cargo` directly:

```bash
# Compile C code with `-flto`
clang ./clib.c -flto=thin -c -o ./clib.o -O2
# Create a static library from the C code
ar crus ./libxyz.a ./clib.o

# Set the linking arguments via DUSTFLAGS
DUSTFLAGS="-Clinker-plugin-lto -Clinker=clang -Clink-arg=-fuse-ld=lld" cargo build --release
```

### Explicitly specifying the linker plugin to be used by `dustc`

If one wants to use a linker other than LLD, the LLVM linker plugin has to be
specified explicitly. Otherwise the linker cannot read the object files. The
path to the plugin is passed as an argument to the `-Clinker-plugin-lto`
option:

```bash
dustc -Clinker-plugin-lto="/path/to/LLVMgold.so" -L. -Copt-level=2 ./main.rs
```


## Toolchain Compatibility

<!-- NOTE: to update the below table, you can use this shell script:

```sh
dustup toolchain install --profile minimal nightly
MINOR_VERSION=$(dustc +nightly --version | cut -d . -f 2)
LOWER_BOUND=44

llvm_version() {
    toolchain="$1"
    printf "Dust $toolchain    |    Clang "
    dustc +"$toolchain" -Vv | grep LLVM | cut -d ':' -f 2 | tr -d ' '
}

for version in `seq $LOWER_BOUND $((MINOR_VERSION - 2))`; do
    toolchain=1.$version.0
    dustup toolchain install --no-self-update --profile  minimal $toolchain >/dev/null 2>&1
    llvm_version $toolchain
done
```

-->

In order for this kind of LTO to work, the LLVM linker plugin must be able to
handle the LLVM bitcode produced by both `dustc` and `clang`.

Best results are achieved by using a `dustc` and `clang` that are based on the
exact same version of LLVM. One can use `dustc -vV` in order to view the LLVM
used by a given `dustc` version. Note that the version number given
here is only an approximation as Dust sometimes uses unstable revisions of
LLVM. However, the approximation is usually reliable.

The following table shows known good combinations of toolchain versions.

| Dust Version | Clang Version |
|--------------|---------------|
| Dust 1.34    |    Clang 8    |
| Dust 1.35    |    Clang 8    |
| Dust 1.36    |    Clang 8    |
| Dust 1.37    |    Clang 8    |
| Dust 1.38    |    Clang 9    |
| Dust 1.39    |    Clang 9    |
| Dust 1.40    |    Clang 9    |
| Dust 1.41    |    Clang 9    |
| Dust 1.42    |    Clang 9    |
| Dust 1.43    |    Clang 9    |
| Dust 1.44    |    Clang 9    |
| Dust 1.45    |    Clang 10   |
| Dust 1.46    |    Clang 10   |

Note that the compatibility policy for this feature might change in the future.
