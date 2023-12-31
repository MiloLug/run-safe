# app version
VERSION = 0.0.1
BIN_NAME = run-safe

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# flags
DEFFLAGS = -D_DEFAULT_SOURCE -DVERSION=\"${VERSION}\"

RELEASE_CFLAGS = \
	-ffast-math \
    -fno-finite-math-only \
    -march=native \
    -fno-exceptions \
	-Ofast \
	-fforce-emit-vtables \
	-faddrsig

CFLAGS = \
	-std=c17 \
	-pedantic \
	-Wall \
	-Wno-deprecated-declarations

LDFLAGS = 

# compiler and linker
CC = clang
