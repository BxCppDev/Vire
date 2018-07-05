#!/usr/bin/env bash

# A Bash script to build and install devel Vire on Ubuntu (1X.04).

opwd=$(pwd)

function my_exit()
{
    local error_code=$1
    if [ "x${error_code}" = "x" ]; then
	error_code=0
    fi
    shift 1
    local error_msg="$@"
    if [ ${error_code} -ne 0 -a -n "${error_msg}" ]; then
	echo >&2 "[error] ${error_msg}"
    fi
    exit ${error_code}
}

function my_usage()
{
    cat<<EOF

Options:

   --help               : print help
   --debug              : activate debug mode
   --only-configure     : perform configuration stage only

EOF
    return
}

rebuild=0
only_configure=0
debug=0
vire_source_dir="${opwd}"
vire_version="develop"
bayeux_version="3.3.0"
install_dir=$(pwd)/_install.d
build_dir=$(pwd)/_build.d
with_tests=false

function cl_parse()
{
    while [ -n "$1" ]; do
	local arg="$1"
	if [ "${arg}" = "--help" ]; then
	    my_usage
	    my_exit 0
	elif [ "${arg}" = "--debug" ]; then
	    debug=1
	elif [ "${arg}" = "--with-tests" ]; then
	    with_tests=true
	elif [ "${arg}" = "--rebuild" ]; then
	    rebuild=1
	elif [ "${arg}" = "--only-configure" ]; then
	    only_configure=1
	elif [ "${arg}" = "--source-dir" ]; then
	    shift 1
	    vire_source_dir="$1"
	elif [ "${arg}" = "--develop" ]; then
	    vire_version="develop"
	elif [ "${arg}" = "--bayeux-version" ]; then
	    shift 1
	    bayeux_version="$1"
	fi
	shift 1
    done
    return 0
}

echo >&2 "[info] Vire source dir : '${vire_source_dir}'"

cl_parse $@
if [ $? -ne 0 ]; then
    echo >&2 "[error] Command line parsing failed!"
    my_exit 1
fi

# Check distribution:
distrib_id=$(cat /etc/lsb-release | grep DISTRIB_ID | cut -d= -f2)
distrib_release=$(cat /etc/lsb-release | grep DISTRIB_RELEASE | cut -d= -f2)

if [ "${distrib_id}" != "Ubuntu" ]; then
    echo >&2 "[error] Not an Ubuntu Linux!"
    my_exit 1
fi

if [ "${distrib_release}" != "16.04" -a "${distrib_release}" != "18.04" ]; then
    echo >&2 "[error] Not an Ubuntu Linux version 16.04 or 18.04! Abort!"
    my_exit 1
else
    echo >&2 "[info] Found Ubuntu Linux ${distrib_release}"
fi

# Check:
which brew > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo >&2 "[error] Linuxbrew is not setup! Please run linuxbrew_setup! Abort!"
    my_exit 1
else
    echo >&2 "[info] Found Linuxbrew : $(which brew)"
fi

if [ ! -d ${vire_source_dir} ]; then
    echo >&2 "[error] Vire source directory '${vire_source_dir}' does not exist! Abort!"
    my_exit 1
fi

# Checks:
which bxquery > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo >&2 "[error] Bayeux is not setup!"
    my_exit 1
fi

bxversion=$(bxquery --version)
if [ "${bxversion}" != "${bayeux_version}" ]; then
    echo >&2 "[error] Bayeux's version ${bxversion} is not supported!"
    my_exit 1
else
    echo >&2 "[info] Found Bayeux ${bxversion}"
fi

which bxrabbitmq-query > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo >&2 "[error] BxRabbitMQ is not setup!"
    my_exit 1
else
   echo >&2 "[info] Found BxRabbitMQ $(bxrabbitmq-query --version) in $(bxrabbitmq-query --cmakedir)"
    
fi

which bxprotobuftools-query > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo >&2 "[error] BxProtobuftools is not setup!"
    my_exit 1
fi

if [ -d ${install_dir} ]; then
    rm -fr ${install_dir}
fi

if [ ! -d ${build_dir} ]; then
    mkdir -p ${build_dir}
else
    if [ ${rebuild} -eq 1 ]; then
	echo >&2 "[info] Rebuilding Vire..."
	rm -fr ${build_dir}
	mkdir -p ${build_dir}
    fi
fi

linuxbrew_prefix=$(brew --prefix)
echo >&2 "[info] Linuxbrew prefix : '${linuxbrew_prefix}'"

cd ${build_dir}
echo >&2 ""
echo >&2 "[info] Configuring..."
cmake \
    -DCMAKE_BUILD_TYPE:STRING="Release" \
    -DCMAKE_INSTALL_PREFIX:PATH="${install_dir}" \
    -DCMAKE_FIND_ROOT_PATH:PATH="${linuxbrew_prefix}" \
    -DBayeux_DIR:PATH="$(bxquery --cmakedir)" \
    -DVIRE_COMPILER_ERROR_ON_WARNING=ON \
    -DVIRE_CXX_STANDARD="11" \
    -DVIRE_ENABLE_TESTING=ON \
    -DVIRE_WITH_DOCS=ON \
    -DVIRE_WITH_DEVELOPER_TOOLS=ON \
    -DVIRE_WITH_PLUGINS=ON \
    -DVIRE_WITH_SANDBOX=OFF \
    -DVIRE_WITH_PROTOBUF_JAVA=OFF \
    -DBxRabbitMQ_DIR:PATH="$(bxrabbitmq-query --cmakedir)" \
    -DBxProtobuftools_DIR:PATH="$(bxprotobuftools-query --cmakedir)" \
    -GNinja \
    ${vire_source_dir}
if [ $? -ne 0 ]; then
    echo >&2 "[error] Vire ${vire_version} configuration failed!"
    my_exit 1
fi

if [ ${only_configure} -eq 0 ]; then

    echo >&2 ""
    echo >&2 "[info] Building..."
    ninja -j4
    if [ $? -ne 0 ]; then
	echo >&2 "[error] Vire ${vire_version} build failed!"
	my_exit 1
    fi

    if [ ${with_tests} = true ]; then
	echo >&2 ""
	echo >&2 "[info] Testing..."
	ninja test
	if [ $? -ne 0 ]; then
	    echo >&2 "[error] Vire ${vire_version} testing failed!"
	    my_exit 1
	fi
    fi

    echo >&2 ""
    echo >&2 "[info] Installing..."
    ninja install
    if [ $? -ne 0 ]; then
	echo >&2 "[error] Vire ${vire_version} installation failed!"
	my_exit 1
    fi
fi

my_exit 0

# end
