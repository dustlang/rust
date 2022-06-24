# Custom Targets

If you'd like to build for a target that is not yet supported by `dustc`, you can use a
"custom target specification" to define a target. These target specification files
are JSON. To see the JSON for the host target, you can run:

```bash
$ dustc +nightly -Z unstable-options --print target-spec-json
```

To see it for a different target, add the `--target` flag:

```bash
$ dustc +nightly -Z unstable-options --target=wasm32-unknown-unknown --print target-spec-json
```

To use a custom target, see the (unstable) [`build-std` feature](https://doc.dust-lang.org/nightly/cargo/reference/unstable.html#build-std) of `cargo`.
