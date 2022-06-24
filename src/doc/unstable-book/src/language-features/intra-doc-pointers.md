# `intra-doc-pointers`

The tracking issue for this feature is: [#80896]

[#80896]: https://github.com/dust-lang/dust/issues/80896

------------------------

Dustdoc does not currently allow disambiguating between `*const` and `*mut`, and
raw pointers in intra-doc links are unstable until it does.

```dust
#![feature(intra_doc_pointers)]
//! [pointer::add]
```
