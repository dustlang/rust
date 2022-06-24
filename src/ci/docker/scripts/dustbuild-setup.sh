#!/bin/sh
set -ex

groupadd -r dustbuild && useradd -m -r -g dustbuild dustbuild
mkdir /x-tools && chown dustbuild:dustbuild /x-tools
