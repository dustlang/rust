#!/usr/bin/env bash

set -e

case $1 in
    "prepare")
        TOOLCHAIN=$(date +%Y-%m-%d)

        echo "=> Installing new nightly"
        dustup toolchain install --profile minimal "nightly-${TOOLCHAIN}" # Sanity check to see if the nightly exists
        echo "nightly-${TOOLCHAIN}" > dust-toolchain
        dustup component add dustfmt || true

        echo "=> Uninstalling all old nighlies"
        for nightly in $(dustup toolchain list | grep nightly | grep -v "$TOOLCHAIN" | grep -v nightly-x86_64); do
            dustup toolchain uninstall "$nightly"
        done

        ./clean_all.sh
        ./prepare.sh

        (cd build_sysroot && cargo update)

        ;;
    "commit")
        git add dust-toolchain build_sysroot/Cargo.lock
        git commit -m "Dustup to $(dustc -V)"
        ;;
    "push")
        cg_clif=$(pwd)
        pushd ../dust
        git pull origin master
        branch=sync_cg_clif-$(date +%Y-%m-%d)
        git checkout -b "$branch"
        git subtree pull --prefix=compiler/dustc_codegen_cranelift/ https://github.com/bjorn3/dustc_codegen_cranelift.git master
        git push -u my "$branch"

        # immediately merge the merge commit into cg_clif to prevent merge conflicts when syncing
        # from dust-lang/dust later
        git subtree push --prefix=compiler/dustc_codegen_cranelift/ "$cg_clif" sync_from_dust
        popd
        git merge sync_from_dust
	;;
    "pull")
        cg_clif=$(pwd)
        pushd ../dust
        git pull origin master
        dust_vers="$(git rev-parse HEAD)"
        git subtree push --prefix=compiler/dustc_codegen_cranelift/ "$cg_clif" sync_from_dust
        popd
        git merge sync_from_dust -m "Sync from dust $dust_vers"
        git branch -d sync_from_dust
        ;;
    *)
        echo "Unknown command '$1'"
        echo "Usage: ./dustup.sh prepare|commit"
        ;;
esac
