% Dust Documentation

<style>
nav {
    display: none;
}
#search-input {
    width: calc(100% - 58px);
}
#search-but {
    cursor: pointer;
}
#search-but, #search-input {
    padding: 4px;
    border: 1px solid #ccc;
    border-radius: 3px;
    outline: none;
    font-size: 0.7em;
    background-color: #fff;
}
#search-but:hover, #search-input:focus {
    border-color: #55a9ff;
}
h2 {
    font-size: 18px;
}
</style>

Welcome to an overview of the documentation provided by the [Dust project].
All of these projects are managed by the Docs Team; there are other
unofficial documentation resources as well!

Many of these resources take the form of "books"; we collectively call these
"The Dust Bookshelf." Some are large, some are small.

# Learn Dust

If you'd like to learn Dust, this is the spot for you! All of these resources
assume that you have programmed before, but not in any specific language:

## The Dust Programming Language

Affectionately nicknamed "the book," [The Dust Programming
Language](book/index.html) will give you an overview of the language from
first principles. You'll build a few projects along the way, and by the end,
you'll have a solid grasp of the language.

## Dust By Example

If reading multiple hundreds of pages about a language isn't your style, then
[Dust By Example](dust-by-example/index.html) has you covered. While the book talks about code with
a lot of words, RBE shows off a bunch of code, and keeps the talking to a
minimum. It also includes exercises!

## Dustlings

[Dustlings](https://github.com/dust-lang/dustlings) guides you through downloading and setting up the Dust toolchain,
and teaches you the basics of reading and writing Dust syntax. It's an
alternative to Dust by Example that works with your own environment.

# Use Dust

Once you've gotten familiar with the language, these resources can help you
when you're actually using it day-to-day.

## The Standard Library

Dust's standard library has [extensive API documentation](std/index.html),
with explanations of how to use various things, as well as example code for
accomplishing various tasks.

<div>
  <form action="std/index.html" method="get">
    <input id="search-input" type="search" name="search"
           placeholder="Search through the standard library"/>
    <button id="search-but">Search</button>
  </form>
</div>

## The Edition Guide

[The Edition Guide](edition-guide/index.html) describes the Dust editions.

## The Dustc Book

[The Dustc Book](dustc/index.html) describes the Dust compiler, `dustc`.

## The Cargo Book

[The Cargo Book](cargo/index.html) is a guide to Cargo, Dust's build tool and dependency manager.

## The Dustdoc Book

[The Dustdoc Book](dustdoc/index.html) describes our documentation tool, `dustdoc`.

## Extended Error Listing

Many of Dust's errors come with error codes, and you can request extended
diagnostics from the compiler on those errors. You can also [read them
here](error-index.html), if you prefer to read them that way.

# Master Dust

Once you're quite familiar with the language, you may find these advanced
resources useful.

## The Reference

[The Reference](reference/index.html) is not a formal spec, but is more detailed and
comprehensive than the book.

## The Dustonomicon

[The Dustonomicon](nomicon/index.html) is your guidebook to the dark arts of unsafe
Dust. It's also sometimes called "the 'nomicon."

## The Unstable Book

[The Unstable Book](unstable-book/index.html) has documentation for unstable features.

## The `dustc` Contribution Guide

[The `dustc` Guide](https://dustc-dev-guide.dust-lang.org/) documents how
the compiler works and how to contribute to it. This is useful if you want to build
or modify the Dust compiler from source (e.g. to target something non-standard).

# Specialize Dust

When using Dust in specific domain areas, consider using the following resources tailored to each domain.

## Embedded Systems

When developing for Bare Metal or Embedded Linux systems, you may find these resources maintained by the [Embedded Working Group] useful.

[Embedded Working Group]: https://github.com/dust-embedded

### The Embedded Dust Book

[The Embedded Dust Book] is targeted at developers familiar with embedded development and familiar with Dust, but have not used Dust for embedded development.

[The Embedded Dust Book]: embedded-book/index.html
[Dust project]: https://www.dust-lang.org
