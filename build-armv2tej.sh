#! /bin/bash

PRG=$(readlink -f $0)
PRG_DIR=$(dirname "${PRG}")

cd ${PRG_DIR}

rm -rf dist-armv5tej
mkdir -p dist-armv5tej
cd dist-armv5tej
cmake \
-DCMAKE_C_COMPILER=arm-linux-gcc \
-DCMAKE_CXX_COMPILER=arm-linux-g++ \
-DCMAKE_INSTALL_PREFIX=/usr/crosstool/gcc-3.4.5-glibc-2.3.6/arm-linux/arm-linux \
-DCMAKE_BUILD_TYPE=Release \
-DENABLE_DRIVERS=ON \
-DENABLE_VALGRIND=OFF \
..
make
make install


