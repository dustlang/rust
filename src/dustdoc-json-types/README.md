# Dustdoc JSON Types

This crate exposes the Dustdoc JSON API as a set of types with serde implementations.
These types are part of the public interface of the dustdoc JSON output, and making them
their own crate allows them to be versioned and distributed without having to depend on
any dustc/dustdoc internals. This way, consumers can rely on this crate for both documentation
of the output, and as a way to read the output easily, and its versioning is intended to
follow semver guarantees about the version of the format. JSON format X will always be
compatible with dustdoc-json-types version N.

Currently, this crate is only used by dustdoc itself. Upon the stabilization of
dustdoc-json, it may be distributed separately for consumers of the API.
