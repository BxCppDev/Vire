#!/usr/bin/env bash

echo >&2 "[trace] Entering 'gen_pb_source.sh'..."

opwd=$(pwd)

function my_exit()
{
    local error_code=$1
    shift 1
    local error_message="$@"
    if [ ${error_code} -ne 0 -a "x${error_message}" != "x" ]; then
	echo >&2 "[error] $@"
    fi
    cd ${opwd}
    echo >&2 "[trace] Exiting 'gen_pb_source.sh'..."
    exit ${error_code}
}

debug=0
protoc_bin=$(which protoc)
out_dir=.
proto_src_dir=
proto_inc_dirs=
gen_cpp=0
gen_java=0
in_list=
out_list=0
list_proto=
while [ -n "$1" ]; do
    token="$1"
    # if [ ${debug} -ne 0 ]; then
    # 	echo "[debug] token = '${token}' !" >&2
    # fi
    proto_inc_dir=
    if [ "x${token:0:1}" = "x-" ]; then
        option="$token"
        if [  ${option} = "--debug" ]; then
	    debug=1
        elif [  ${option} = "-c" -o ${option} = "--protoc" ]; then
	    shift 1
            protoc_bin="$1"
        elif [  ${option} = "-d" -o ${option} = "--out-dir" ]; then
	    shift 1
            out_dir="$1"
        elif [  ${option} = "-s" -o ${option} = "--proto-src-dir" ]; then
	    shift 1
            proto_src_dir="$1"
	    if [ ${debug} -ne 0 ]; then
		echo >&2 "[debug] Parsing proto_src_dir = '${proto_src_dir}' !"
	    fi
        elif [  ${option} = "-I" -o ${option} = "--proto-inc-dir" ]; then
	    shift 1
            proto_inc_dir="$1"
	    if [ ${debug} -ne 0 ]; then
		echo >&2 "[debug] Parsing proto_inc_dir = '${proto_inc_dir}' !"
	    fi
        elif [ ${option} = "--cpp" ]; then
            gen_cpp=1
        elif [ ${option} = "--java" ]; then
            gen_java=1
	elif [ ${option} = "-l" -o ${option} = "--list" ]; then
            out_list=1
	elif [ ${option} = "-i" -o ${option} = "--input" ]; then
	    shift 1
            in_list="$1"
	fi
    else
        argument="$token"
        echo >&2 "[warning] Ignoring command line argument '${argument}' !"
    fi
    if [ "x${proto_inc_dir}" != "x" ]; then
	if [ ${debug} -ne 0 ]; then
            echo >&2 "[debug] Appending proto_inc_dir = '${proto_inc_dir}' !"
	fi
	proto_inc_dirs="${proto_inc_dirs} ${proto_inc_dir}"
    fi
    shift 1
done

if [ "x${out_dir}" = "x" ]; then
    out_dir="."
fi
if [ "x${proto_src_dir}" = "x" ]; then
    proto_src_dir="."
fi
if [ "x${proto_inc_dirs}" = "x" ]; then
    proto_inc_dirs="."
fi
if [ "x${protoc_bin}" = "x" ]; then
    echo >&2 "[error] Missing protoc binary!"
    my_exit 1 "Missing protoc binary!"
fi

if [ ${debug} -ne 0 ]; then
    echo >&2 "[debug] protoc_bin     = '${protoc_bin}'"
    echo >&2 "[debug] out_dir        = '${out_dir}'"
    echo >&2 "[debug] proto_src_dir  = '${proto_src_dir}'"
    echo >&2 "[debug] proto_inc_dirs = '${proto_inc_dirs}'"
fi
if [ "x${in_list}" != "x" ]; then
    list_proto=$(echo "${in_list}" | tr ';' '\n')
else
    list_proto=$(find ${proto_src_dir} -name "*.proto" | sed -e "s@^${proto_src_dir}/@@g")
fi

if [ ${debug} -ne 0 ]; then
    echo >&2 "[debug] list_proto='${list_proto}'"
fi

if [ ${gen_cpp} -ne 0 ]; then
    mkdir -p ${out_dir}/cpp
fi
if [ ${gen_java} -ne 0 ]; then
    mkdir -p ${out_dir}/java
fi

pp_opt=
for ppdir in ${proto_inc_dirs}
do
    pp_opt="${pp_opt} --proto_path=${ppdir}"
done
echo >&2 "[debug] pp_opt='${pp_opt}'"

for protofile in ${list_proto}
do
    if [ ${gen_cpp} -ne 0 ]; then
	if [ ${debug} -ne 0 ]; then
	    echo >&2 "[debug] Generate C++ source from '${protofile}'"
	fi
	protofile_base=$(basename ${protofile})
	echo >&2 "[debug] protofile_base = '${protofile_base}'"
	COMMAND="${protoc_bin} ${pp_opt} --cpp_out=${out_dir}/cpp ${proto_src_dir}/${protofile}"
	echo >&2 "[debug] COMMAND = '${COMMAND}'"
	eval ${COMMAND}
	if [ $? -ne 0 ]; then
	    my_exit 1 "Cannot generate C++ source from '${protofile}'!"
	fi
    fi


    if [ ${gen_java} -ne 0 ]; then
	if [ ${debug} -ne 0 ]; then
	    echo >&2 "[debug] Generate Java source from '${protofile}'"
	fi
	protofile_base=$(basename ${protofile})
	echo >&2 "[debug] protofile_base = '${protofile_base}'"
	COMMAND="${protoc_bin} ${pp_opt} --java_out=${out_dir}/java ${proto_src_dir}/${protofile}"
	echo >&2 "[debug] COMMAND = '${COMMAND}'"
	eval ${COMMAND}
	if [ $? -ne 0 ]; then
	    my_exit 1 "Cannot generate Java source from '${protofile}'!"
	fi
    fi

    if [ ${out_list} -ne 0 ]; then
	if [ ${debug} -ne 0 ]; then
	    echo >&2 "[debug] protofile='${protofile}'"
	fi
	echo ${protofile} | sed -e 's@.proto$@@g'
    fi

done
# if [ ${debug} -ne 0 ]; then
#     # tree ${out_dir} >&2
# fi

my_exit 0

# end
