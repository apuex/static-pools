#! /bin/bash

PRG=$(readlink -f $0)
PRG_DIR=$(dirname "${PRG}")

cd ${PRG_DIR}

rm -rf dist-mipsel
mkdir -p dist-mipsel
cd dist-mipsel
cmake \
-DCMAKE_C_COMPILER=mipsel-linux-musl-gcc \
-DCMAKE_CXX_COMPILER=mipsel-linux-musl-g++ \
-DCMAKE_INSTALL_PREFIX=/opt/cross/mipsel-linux-musl \
-DCMAKE_BUILD_TYPE=Release \
-DENABLE_DRIVERS=ON \
-DENABLE_VALGRIND=OFF \
..
make
make install


