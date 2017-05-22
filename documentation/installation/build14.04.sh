brewsh
do_bayeux_dev11_setup

VIRE_DEV_BIN_DIR="${SW_WORK_DIR}/Vire/Binary/Vire-trunk"
mkdir -p ${VIRE_DEV_BIN_DIR}/Build-gcc-cxx11-ninja-Linux-x86_64
cd ${VIRE_DEV_BIN_DIR}/Build-gcc-cxx11-ninja-Linux-x86_64
VIRE_DEV11_INSTALL_DIR="${VIRE_DEV_BIN_DIR}/Install-gcc-cxx11-Linux-x86_64"
cmake \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_INSTALL_PREFIX:PATH="${VIRE_DEV11_INSTALL_DIR}" \
    -DCMAKE_FIND_ROOT_PATH:PATH="$(bxquery --prefix);$(gsl-config --prefix)/../.." \
    -DVIRE_COMPILER_ERROR_ON_WARNING=ON \
    -DVIRE_CXX_STANDARD="11" \
    -DVIRE_ENABLE_TESTING=ON \
    -DVIRE_WITH_DOCS=ON \
    -DVIRE_WITH_DEVELOPER_TOOLS=ON \
    -DVIRE_WITH_PLUGINS=OFF \
    -DVIRE_WITH_SANDBOX=OFF \
    -DVIRE_WITH_PROTOBUF_JAVA=ON \
    -DPROTOBUF_ROOT:PATH="/data3/sw/GoogleProtocolBuffers/install-3.0.0-beta-3.1" \
    -GNinja \
    /home/mauger/Private/Work/lpc_nemo_svn/snsw/Vire/Vire/Vire-trunk

cd ..
rm -fr Build-gcc-cxx11-ninja-Linux-x86_64/
mkdir Build-gcc-cxx11-ninja-Linux-x86_64/
cd Build-gcc-cxx11-ninja-Linux-x86_64/

# end
