#!/bin/bash

GWT_VERSION=2.8.2

if [ $# -lt 1 ]; then
	printf "Usage:"
	printf " $0 <PATH_TO_BUILDROOT>\n"
	exit
fi

BASE_DIR=$(readlink -f  $1)
TARGET_DIR=${BASE_DIR}/output/target
BUILD_DIR=${BASE_DIR}/output/build/playground-HEAD
STAGING_DIR=${BASE_DIR}/output/staging
HOST_DIR=${BASE_DIR}/output/host

GWT_COMPILER_DIR=${HOST_DIR}/gwt-${GWT_VERSION}

mkdir -p build
cd build
cmake	-DCMAKE_TOOLCHAIN_FILE="${HOST_DIR}/share/buildroot/toolchainfile.cmake" \
	-DCMAKE_COLOR_MAKEFILE=OFF \
	-DBUILD_DOC=OFF \
	-DBUILD_DOCS=OFF \
	-DBUILD_EXAMPLE=OFF \
	-DBUILD_EXAMPLES=OFF \
	-DBUILD_TEST=OFF \
	-DBUILD_TESTS=OFF \
	-DBUILD_TESTING=OFF \
	-DBUILD_SHARED_LIBS=ON \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX="${TARGET_DIR}/nonlinear/playground" \
	-DJARSDIR="${GWT_COMPILER_DIR}" \
	.. && \
make -j8 && \
make install