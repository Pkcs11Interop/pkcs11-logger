#!/bin/bash

set -e

make distclean

cat Makefile | sed 's/^ARCH_FLAGS=.*/ARCH_FLAGS= -m32/' | sed 's/^LIBNAME=.*/LIBNAME=pkcs11-logger-x86.so/' > Makefile.x86
make -f Makefile.x86
rm Makefile.x86
make clean

cat Makefile | sed 's/^ARCH_FLAGS=.*/ARCH_FLAGS= -m64/' | sed 's/^LIBNAME=.*/LIBNAME=pkcs11-logger-x64.so/' > Makefile.x64
make -f Makefile.x64
rm Makefile.x64
make clean

