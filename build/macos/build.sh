#!/bin/bash

set -e

make distclean

cat Makefile | sed 's/^ARCH_FLAGS=.*/ARCH_FLAGS= -target arm64-apple-macos11/' | sed 's/^LIBNAME=.*/LIBNAME=pkcs11-logger-arm64.dylib/' > Makefile.arm64
make -f Makefile.arm64
rm Makefile.arm64
make clean

cat Makefile | sed 's/^ARCH_FLAGS=.*/ARCH_FLAGS= -target x86_64-apple-macos10.12/' | sed 's/^LIBNAME=.*/LIBNAME=pkcs11-logger-x86_64.dylib/' > Makefile.x86_64
make -f Makefile.x86_64
rm Makefile.x86_64
make clean

lipo -create -output pkcs11-logger.dylib pkcs11-logger-arm64.dylib pkcs11-logger-x86_64.dylib
rm pkcs11-logger-arm64.dylib pkcs11-logger-x86_64.dylib
