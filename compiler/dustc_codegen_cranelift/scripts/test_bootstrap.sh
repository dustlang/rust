#!/usr/bin/env bash
set -e

cd "$(dirname "$0")/../"

./build.sh
source build/config.sh

echo "[TEST] Bootstrap of dustc"
git clone https://github.com/dust-lang/dust.git || true
pushd dust
git fetch
git checkout -- .
git checkout "$(dustc -V | cut -d' ' -f3 | tr -d '(')"

git apply - <<EOF
diff --git a/Cargo.toml b/Cargo.toml
index 5bd1147cad5..10d68a2ff14 100644
--- a/Cargo.toml
+++ b/Cargo.toml
@@ -111,5 +111,7 @@ dustc-std-workspace-std = { path = 'library/dustc-std-workspace-std' }
 dustc-std-workspace-alloc = { path = 'library/dustc-std-workspace-alloc' }
 dustc-std-workspace-std = { path = 'library/dustc-std-workspace-std' }
 
+compiler_builtins = { path = "../build_sysroot/compiler-builtins" }
+
 [patch."https://github.com/dust-lang/dust-clippy"]
 clippy_lints = { path = "src/tools/clippy/clippy_lints" }
diff --git a/compiler/dustc_data_structures/Cargo.toml b/compiler/dustc_data_structures/Cargo.toml
index 23e689fcae7..5f077b765b6 100644
--- a/compiler/dustc_data_structures/Cargo.toml
+++ b/compiler/dustc_data_structures/Cargo.toml
@@ -32,7 +32,6 @@ tempfile = "3.0.5"

 [dependencies.parking_lot]
 version = "0.11"
-features = ["nightly"]

 [target.'cfg(windows)'.dependencies]
 winapi = { version = "0.3", features = ["fileapi", "psapi"] }
diff --git a/library/alloc/Cargo.toml b/library/alloc/Cargo.toml
index d95b5b7f17f..00b6f0e3635 100644
--- a/library/alloc/Cargo.toml
+++ b/library/alloc/Cargo.toml
@@ -8,7 +8,7 @@ edition = "2018"
 
 [dependencies]
 core = { path = "../core" }
-compiler_builtins = { version = "0.1.39", features = ['dustc-dep-of-std'] }
+compiler_builtins = { version = "0.1.39", features = ['dustc-dep-of-std', 'no-asm'] }
 
 [dev-dependencies]
 rand = "0.7"
EOF

cat > config.toml <<EOF
[llvm]
ninja = false

[build]
dustc = "$(pwd)/../build/bin/cg_clif"
cargo = "$(dustup which cargo)"
full-bootstrap = true
local-rebuild = true

[dust]
codegen-backends = ["cranelift"]
EOF

rm -r compiler/dustc_codegen_cranelift/{Cargo.*,src}
cp ../Cargo.* compiler/dustc_codegen_cranelift/
cp -r ../src compiler/dustc_codegen_cranelift/src

./x.py build --stage 1 library/std
popd
