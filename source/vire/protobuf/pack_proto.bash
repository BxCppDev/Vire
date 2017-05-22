#!/usr/bin/env bash

opwd=$(pwd)

function my_exit()
{
    local error_code=$1
    shift 1
    local error_message="$@"
    if [ ${error_code} -ne 0 -a "x${error_message}" != "x" ]; then
        echo "$@" >&2
    fi
    cd ${opwd}
    exit ${error_code}
}

debug=0
out_dir=

while [ -n "$1" ]; do
    token="$1"
    if [ "x${token:0:1}" = "x-" ]; then
	option="${token}"
        if [  ${option} = "--debug" ]; then
            debug=1
	elif [  ${option} = "-d" -o ${option} = "--out-dir" ]; then
            shift 1
            out_dir="$1"
        fi
    else
        argument="$token"
        echo 1>&2 "WARNING: Ignoring command line argument '${argument}' !"
    fi
    shift 1
done
if [ "x${out_dir}" = "x" ]; then
    out_dir="/tmp/${USER}/_vire.d/VireProtobuf"
fi

echo 1>&2 "NOTICE: out_dir = '${out_dir}'"

if [ -d ${out_dir} ]; then
    rm -fr ${out_dir}
fi
mkdir -p ${out_dir}

all_vire_protos=$(find . -name "*.proto")

packed_vire_protos=`echo ${all_vire_protos} | tr ' ' '\n' | grep -e /vire/utility/ -e /vire/message/ -e /vire/cms/`

for fproto in ${packed_vire_protos} ; do
    echo 1>&2 "Proto : '${fproto}'"
    thisprotodir=$(dirname ${fproto})
    thisprotobase=$(basename ${fproto})
    thistargetdir=${out_dir}/${thisprotodir}
    thisfproto=${out_dir}/_tmp.proto
    echo 1>&2 "NOTICE: Building proto file '${thisfproto}'..."
    echo "option java_package = \"VireProtobuf\"" > ${thisfproto}
    cat ${fproto} >> ${thisfproto}
    cat ${thisfproto}
    if [ ! -d ${thistargetdir} ]; then
	mkdir -p ${thistargetdir}
    fi
    echo 1>&2 "NOTICE: Installing proto file '${thisfproto}' in '${thistargetdir}'..."
    install ${thisfproto} ${thistargetdir}/${thisprotobase}
    rm -f ${thisfproto}
done

cp -f README.rst ${out_dir}/

tree ${out_dir}/

cd ${out_dir}/

tar cvzf VireProtobuf.tar.gz vire
ls -l VireProtobuf.tar.gz
tar tvzf VireProtobuf.tar.gz
mv VireProtobuf.tar.gz ${opwd}/
cd ${opwd}

my_exit 0

# end
