cadfaelbrew_setup
bayeux_pro_setup
# bxjsontools_setup
protobuf_setup
bxprotobuftools_setup
bxrabbitmq_setup

mkdir -p ${SW_WORK_DIR}
mkdir -p ${SW_WORK_DIR}/Vire
mkdir -p ${SW_WORK_DIR}/Vire/Binary

export VIRE_SOURCE_BASE_DIR="${HOME}/Documents/Private/Software/Vire/Source"
export VIRE_DEV_SOURCE_DIR="${VIRE_SOURCE_BASE_DIR}/Vire-trunk"
export VIRE_DEV_BIN_DIR="${SW_WORK_DIR}/Vire/Binary/Vire-trunk"
export VIRE_DEV_BUILD_DIR="${VIRE_DEV_BIN_DIR}/Build-ninja"
export VIRE_DEV_INSTALL_DIR="${VIRE_DEV_BIN_DIR}/Install"

mkdir -p ${VIRE_SOURCE_BASE_DIR}
cd ${VIRE_SOURCE_BASE_DIR}
cd Vire-trunk
LANG=C svn info


echo ${CADFAELBREW_INSTALL_DIR}
# gsl-config --prefix
# bxquery --prefix
# which protoc
# which bxjsontools-query
cadfaelbrew_install_dir=$(clhep-config --prefix | tr -d '"')
mkdir -p ${VIRE_DEV_BUILD_DIR}
cd ${VIRE_DEV_BUILD_DIR}
pwd
cmake \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_INSTALL_PREFIX:PATH="${VIRE_DEV_INSTALL_DIR}" \
    -DCMAKE_FIND_ROOT_PATH:PATH="${cadfaelbrew_install_dir}" \
    -DBayeux_DIR:PATH="$(bxquery --prefix)" \
    -DVIRE_COMPILER_ERROR_ON_WARNING=ON \
    -DVIRE_CXX_STANDARD="11" \
    -DVIRE_ENABLE_TESTING=ON \
    -DVIRE_WITH_DOCS=ON \
    -DVIRE_WITH_DEVELOPER_TOOLS=ON \
    -DVIRE_WITH_PLUGINS=ON \
    -DVIRE_WITH_SANDBOX=OFF \
    -DVIRE_WITH_PROTOBUF_JAVA=ON \
    -DPROTOBUF_ROOT:PATH="/opt/sw/GoogleProtocolBuffers/install-3.0.0" \
    -DBxRabbitMQ_DIR:PATH="$(bxrabbitmq-query --cmakedir)" \
    -DBxProtobuftools_DIR:PATH="$(bxprotobuftools-query --cmakedir)" \
    -GNinja \
    ${VIRE_DEV_SOURCE_DIR}

# -DBoost_DIR:PATH="${CADFAELBREW_PREFIX_DIR}/lib/cmake"
# cd ..
# rm -fr Build-gcc-cxx11-ninja-Linux-x86_64/
# mkdir Build-gcc-cxx11-ninja-Linux-x86_64/
# cd Build-gcc-cxx11-ninja-Linux-x86_64/

# end
