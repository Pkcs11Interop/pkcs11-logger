#!/bin/bash

set -e

make distclean

# cat Makefile | sed 's/^ARCH_FLAGS=.*/ARCH_FLAGS= -arch arm64/' | sed 's/^LIBNAME=.*/LIBNAME=libpkcs11-logger-arm64.dylib/' > Makefile.arm64
# make -f Makefile.arm64
# rm Makefile.arm64
# make clean

# cat Makefile | sed 's/^ARCH_FLAGS=.*/ARCH_FLAGS= -arch x86_64/' | sed 's/^LIBNAME=.*/LIBNAME=libpkcs11-logger-x64.dylib/' > Makefile.x64
# make -f Makefile.x64
# rm Makefile.x64
# make clean

cat Makefile | sed 's/^ARCH_FLAGS=.*/ARCH_FLAGS= -arch arm64 -arch x86_64/' | sed 's/^LIBNAME=.*/LIBNAME=libpkcs11-logger.dylib/' > Makefile.universal
make -f Makefile.universal
rm Makefile.universal
make clean

