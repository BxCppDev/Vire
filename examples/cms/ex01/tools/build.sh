#!/usr/bin/env bash

opwd=$(pwd)

function my_exit()
{
    local error_code="$1"
    shift 1
    exit ${error_code}
}

which virequery > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo >&2 "[error] Vire is not installed!"
    my_exit 1
fi

src_dir=$(pwd)
build_dir="$(pwd)/_build.d"
install_dir="$(pwd)/_install.d"

if [ -d ${install_dir} ]; then
    rm -fr ${install_dir}
fi

if [ -d ${build_dir} ]; then
    rm -fr ${build_dir}
fi

mkdir -p ${build_dir}
cd  ${build_dir}

cmake \
    -DCMAKE_INSTALL_PREFIX="${install_dir}" \
    -DVire_DIR=$(virequery --cmakedir) \
    ${src_dir}
if [ $? -ne 0 ]; then
    echo >&2 "[error] CMake failed!"
    my_exit 1
fi

make -j4
if [ $? -ne 0 ]; then
    echo >&2 "[error] Make failed!"
    my_exit 1
fi

make install
if [ $? -ne 0 ]; then
    echo >&2 "[error] Installation failed!"
    my_exit 1
fi


my_exit 0

# end
