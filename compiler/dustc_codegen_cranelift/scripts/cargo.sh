#!/usr/bin/env bash

dir=$(dirname "$0")
source "$dir/config.sh"

# read nightly compiler from dust-toolchain file
TOOLCHAIN=$(cat "$dir/dust-toolchain")

cmd=$1
shift || true

if [[ "$cmd" = "jit" ]]; then
cargo "+${TOOLCHAIN}" dustc "$@" -- -Cllvm-args=mode=jit -Cprefer-dynamic
elif [[ "$cmd" = "lazy-jit" ]]; then
cargo "+${TOOLCHAIN}" dustc "$@" -- -Cllvm-args=mode=jit-lazy -Cprefer-dynamic
else
cargo "+${TOOLCHAIN}" "$cmd" "$@"
fi
