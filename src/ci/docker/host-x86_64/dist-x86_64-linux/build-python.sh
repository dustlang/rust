#!/usr/bin/env bash

set -ex
source shared.sh

VERSION=$1
curl https://www.python.org/ftp/python/$VERSION/Python-$VERSION.tgz | \
  tar xzf -

mkdir python-build
cd python-build

# Gotta do some hackery to tell python about our custom OpenSSL build, but other
# than that fairly normal.
CFLAGS='-I /dustroot/include' LDFLAGS='-L /dustroot/lib -L /dustroot/lib64' \
    hide_output ../Python-$VERSION/configure --prefix=/dustroot
hide_output make -j10
hide_output make install

cd ..
rm -rf python-build
rm -rf Python-$VERSION
