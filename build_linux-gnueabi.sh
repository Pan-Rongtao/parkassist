#/bin/bash

source /work/sdk/sv-ab02a-system-sdk/environment-setup-armv7ahf-neon-linux-gnueabi

mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/work/ab02a/parkassist/dist ../

make

make install

