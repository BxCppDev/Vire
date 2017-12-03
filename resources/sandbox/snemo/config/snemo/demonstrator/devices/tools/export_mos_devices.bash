#!/usr/bin/env bash

opwd=$(pwd)

function my_exit()
{
    local error_code=$1
    shift
    local error_msg="$@"
    if [ -n "${error_msg}" ]; then
	echo >&2 "[error] ${error_msg}"
    fi
    cd ${opwd}
    exit ${error_code}
}

if [ ! -f .tools ]; then
    my_exit 1 "Not in the SuperNEMO device tools directory!"
fi

tools_dir=$(pwd)
work_dir=""
snemo_cms_mos_version=""
snemo_devices_version=""
action=
debug=false
install=false
mos_devices_db=
build_device_model_xmlfile=
build_device_model_list=
dry_run=false

which virequery > /dev/null 2>&1
if [ $? -ne 0 ]; then
    my_exit 1 "Cannot find virequery!"
fi

mos_to_vire_bin=$(which viremos_xml2viredev)
if [ -z "${mos_to_vire_bin}" ]; then
    my_exit 1 "Cannot find viremos_xml2viredev!"
fi

function app_usage()
{
    cat<<EOF

export_mos_devices - Export MOS CMS devices to Vire model definition files

Usage:

  export_mos_devices [OPTIONS...]

Options:

  --help | -h
  --debug
  --print-status | -s
  --work-dir | -W
  --mos-version | -V
  --devices-version | -D
  --list-of-devices | -L
  --build-device-model | -b
  --build-all-device-models | -B
  --install | -I
  --mos-devices-db | -M
  --dry-run


Example:

  export_mos_devices --build-all-device-models --install

EOF
    return
}

function parse_args()
{
    while [ -n "$1" ]; do
	arg="$1"
	if [ "${arg}" == "--help" -o "${arg}" == "-h" ] ; then
	    app_usage
	    my_exit 2
	elif [ "${arg}" == "--dry-run" ] ; then
	    dry_run=true
	elif [ "${arg}" == "--print-status" -o "${arg}" == "-s" ] ; then
	    action="print-status"
	elif [ "${arg}" == "--work-dir" -o "${arg}" == "-W" ] ; then
	    shift 1
	    work_dir="$1"
	elif [ "${arg}" == "--mos-version" -o "${arg}" == "-V" ] ; then
	    shift 1
	    snemo_cms_mos_version="$1"
	elif [ "${arg}" == "--devices-version" -o "${arg}" == "-D" ] ; then
	    shift 1
	    snemo_devices_version="$1"
	elif [ "${arg}" == "--list-of-devices" -o "${arg}" == "-L" ] ; then
	    action="list-of-devices"
	elif [ "${arg}" == "--build-device-model" -o "${arg}" == "-b" ] ; then
	    action="build-device-model"
	    shift 1
	    build_device_model_xmlfile="$1"
	elif [ "${arg}" == "--build-all-device-models" -o "${arg}" == "-B" ] ; then
	    action="build-all-device-models"
	elif [ "${arg}" == "--debug" ] ; then
	    debug=true
	elif [ "${arg}" == "--install" ] ; then
	    install=true
	elif [ "${arg}" == "--mos-devices-db" -o "${arg}" == "-M" ] ; then
	    shift 1
	    mos_devices_db="$1"
	fi
	shift 1
    done
    return 0
}

parse_args $@

if [ -z "${work_dir}" ]; then
    work_dir="${tools_dir}/_work.d"
fi

mkdir -p ${work_dir}
cd ${work_dir}
full_work_dir=$(pwd)
cd ${opwd}

if [ -z "${snemo_cms_mos_version}" ]; then
    snemo_cms_mos_version="0.9.2"
fi

if [ -z "${snemo_devices_version}" ]; then
    snemo_devices_version="0.1"
fi

if [ -z "${mos_devices_db}" ]; then
    echo >&2 "[notice] Setting default MOS device DB file path..."
    mos_devices_db="${work_dir}/sndemo_mos_devices_db-${snemo_cms_mos_version}.lis"
fi

if [ ${debug} = true ]; then
    echo >&2 "[debug] Tools dir       = '${tools_dir}'"
    echo >&2 "[debug] Work dir        = '${work_dir}'"
    echo >&2 "[debug] CMS MOS version = '${snemo_cms_mos_version}'"
    echo >&2 "[debug] Devices version = '${snemo_devices_version}'"
    echo >&2 "[debug] MOS devices DB  = '${mos_devices_db}'"
    if [ -d ${work_dir} ]; then
	echo >&2 "[debug] Work directory '${work_dir}' already exists!"
    fi
fi

vire_version=$(virequery --version)
vire_mos_xsd_version=2.0.3
vire_mos_xsd_dir="$(virequery --resourcedir)/plugins/Vire_MOS/data/mos/xml/${vire_mos_xsd_version}"

snemo_cms_mos_xml_dir="../../cms/mos_xml/${snemo_cms_mos_version}"
snemo_cms_mos_dev_launch="${snemo_cms_mos_xml_dir}/SNEMO/devices_launch.conf"

snemo_devices_dir="../../devices/${snemo_devices_version}"
snemo_devices_cms_models_dir="${snemo_devices_dir}/models/demonstrator/cms"
snemo_devices_cms_models_lis="${snemo_devices_cms_models_dir}/models.lis"
snemo_path_prefix="@snemo:config/snemo/demonstrator/devices/${snemo_devices_version}/models/demonstrator/cms/MOS"

if [ ${debug} = true ]; then
    echo >&2 "[debug] vire_version                 = ${vire_version}"
    echo >&2 "[debug] vire_mos_xsd_version         = ${vire_mos_xsd_version}"
    echo >&2 "[debug] vire_mos_xsd_dir             = ${vire_mos_xsd_dir}"
    echo >&2 "[debug] snemo_cms_mos_version        = ${snemo_cms_mos_version}"
    echo >&2 "[debug] snemo_cms_mos_xml_dir        = ${snemo_cms_mos_xml_dir}"
    echo >&2 "[debug] snemo_cms_mos_dev_launch     = ${snemo_cms_mos_dev_launch}"
    echo >&2 "[debug] snemo_devices_version        = ${snemo_devices_version}"
    echo >&2 "[debug] snemo_devices_dir            = ${snemo_devices_dir}"
    echo >&2 "[debug] snemo_devices_cms_models_dir = ${snemo_devices_cms_models_dir}"
    echo >&2 "[debug] snemo_devices_cms_models_lis = ${snemo_devices_cms_models_lis}"
    echo >&2 "[debug] snemo_path_prefix            = ${snemo_path_prefix}"
    echo >&2 "[debug] mos_devices_db               = ${mos_devices_db}"
fi

if [ ! -d ${vire_mos_xsd_dir} ]; then
    my_exit 1 "Directory '${vire_mos_xsd_dir}' does not exist!"
fi

if [ ! -d ${snemo_cms_mos_xml_dir} ]; then
    my_exit 1 "Directory '${snemo_cms_mos_xml_dir}' does not exist!"
fi

if [ ! -f ${snemo_cms_mos_dev_launch} ]; then
    my_exit 1 "File '${snemo_cms_mos_dev_launch}' does not exist!"
fi

if [ ! -d ${snemo_devices_dir} ]; then
    my_exit 1 "Directory '${snemo_devices_dir}' does not exist!"
fi

if [ ! -d ${snemo_devices_cms_models_dir} ]; then
    my_exit 1 "Directory '${snemo_devices_cms_models_dir}' does not exist!"
fi

# if [ ! -f ${snemo_devices_cms_models_lis} ]; then
#     my_exit 1 "File '${snemo_devices_cms_models_lis}' does not exist!"
# fi

function build_mos_devices_db()
{
    local debug_opt=""
    if [ ${debug} = true ]; then
	debug_opt="--debug"
    fi
    python3 ${tools_dir}/mos_device_mgr.py \
	  --launcher="${tools_dir}/../../cms/mos_xml/0.9.2/SNEMO/devices_launch.conf" \
	  --output-file="${mos_devices_db}" \
	  ${debug_opt}
    if [ $? -ne 0 ]; then
	return 1
    fi
    return 0
}

if [ ! -f ${mos_devices_db} ]; then
    echo >&2 "[notice] Generating MOS device DB file..."
    build_mos_devices_db
    if [ $? -ne 0 ]; then
	my_exit 1 "Cannot create the '${mos_devices_db}' file!"
    fi
    echo >&2 "[notice] MOS device DB file is generated."
fi

function build_cms_mos_xml_dir_list()
{
    cd ${snemo_cms_mos_xml_dir}
    local list=$(find . -type f -a -name "*.xml" | grep -v "Model.xml" | sed -e "s@\./@@g")
    echo ${list}
    cd ${opwd}
    return 0
}

snemo_xml_list=$(build_cms_mos_xml_dir_list)
if [ ${debug} = true ]; then
    echo >&2 "[debug] snemo_xml_list=${snemo_xml_list}"
fi

function print_status()
{
    echo "vire_version                 = ${vire_version}"
    echo "vire_mos_xsd_version         = ${vire_mos_xsd_version}"
    echo "vire_mos_xsd_dir             = ${vire_mos_xsd_dir}"
    echo "snemo_cms_mos_version        = ${snemo_cms_mos_version}"
    echo "snemo_cms_mos_xml_dir        = ${snemo_cms_mos_xml_dir}"
    echo "snemo_cms_mos_dev_launch     = ${snemo_cms_mos_dev_launch}"
    echo "snemo_devices_version        = ${snemo_devices_version}"
    echo "snemo_devices_dir            = ${snemo_devices_dir}"
    echo "snemo_devices_cms_models_dir = ${snemo_devices_cms_models_dir}"
    echo "snemo_devices_cms_models_lis = ${snemo_devices_cms_models_lis}"
    echo "mos_devices_db               = ${mos_devices_db}"
    return
}

declare -A snemo_mosdev_xmlfiles
declare -A snemo_mosdev_mountpoints
declare -A snemo_mosdev_mountpoints2
declare -A snemo_mosdev_outputdirs
declare -A snemo_mosdev_modelnames

function load_list_of_launched_devices()
{
    echo >&2 "[notice] Build the list of launched devices..."
    local old_IFS=$IFS
    IFS=$'\n'
    let count=1
    for line in $(cat ${mos_devices_db})
    do
	#echo "[debug] Line is '${line}'"
	xmlfile=`echo ${line} | cut -d';' -f 1`
	mountpoint=`echo ${line} | cut -d';' -f 2`
	mountpoint2=`echo ${line} | cut -d';' -f 3`
	outputdir=`echo ${line} | cut -d';' -f 4`
	modelname=`echo ${line} | cut -d';' -f 5`
	# echo "[debug] key is '${count}' : "
	# echo "[debug]   xmlfile     is '${xmlfile}'"
	# echo "[debug]   mountpoint  is '${mountpoint}'"
	# echo "[debug]   mountpoint2 is '${mountpoint2}'"
	# echo "[debug]   outputdir   is '${outputdir}'"
	# echo "[debug]   modelname   is '${modelname}'"
	snemo_mosdev_xmlfiles[${count}]="${xmlfile}"
	snemo_mosdev_mountpoints[${count}]="${mountpoint}"
	snemo_mosdev_mountpoints2[${count}]="${mountpoint2}"
	snemo_mosdev_outputdirs[${count}]="${outputdir}"
 	snemo_mosdev_modelnames[${count}]="${modelname}"
	let count=count+1
    done
    IFS=$old_IFS
    return 0
}

function print_list_of_launched_devices()
{
    echo "============= List of launched devices =============="
    for key in ${!snemo_mosdev_mountpoints[@]}; do
	echo ${snemo_mosdev_xmlfiles[${key}]}
    done
    echo "====================================================="
}

function exists_in_list_of_launched_devices()
{
    local xmlfile="$1"
    for key in "${!snemo_mosdev_mountpoints[@]}"; do
	xmlkey=${snemo_mosdev_xmlfiles[${key}]}
	if [ ${debug} = true ]; then
	    echo >&2 "[debug] Scanning '${xmlkey}' at key=${key} ..."
	fi
	if [ "${xmlfile}" = "${xmlkey}" ]; then
	    if [ ${debug} = true ]; then
		echo >&2 "[debug] Found '${xmlfile}' at key=${key}."
	    fi
	    echo ${key}
	    return 0
	fi
    done
    if [ ${debug} = true ]; then
	echo >&2 "[debug] Not found '${xmlfile}'."
    fi
    echo 0
    return 1
}

function print_list_of_mos_devices()
{
    for fxml in ${snemo_xml_list} ; do
	# echo >&2 "[debug] Checking '${fxml}'..."
	# echo >&2 "[debug] index = '${index}'..."
	declare -i index=`exists_in_list_of_launched_devices ${fxml}`
	if [ -n "$index" ]; then
	    echo "${fxml}"
	fi
    done
    return
}

function build_device_model()
{
    local logging_label="debug"
    local xmlmodel="$1"
    echo >&2 "[debug] Building device model from XML file '${xmlmodel}'..."
    declare -i index=`exists_in_list_of_launched_devices ${xmlmodel}`
    echo >&2 "[debug] index = '${index}'!"
    if [ $index -eq 0 ]; then
	echo >&2 "[error] Cannot find XML device model = '${xmlmodel}'!"
	return 1
    fi
    full_xml_file="${snemo_cms_mos_xml_dir}/${xmlmodel}"
    output_dir="${work_dir}/${snemo_mosdev_mountpoints2[$index]}"
    server_model_name=${snemo_mosdev_modelnames[$index]}
    removable_dir="${output_dir}/${server_model_name}"
    echo >&2 "[debug] full_xml_file     = '${full_xml_file}'!"
    echo >&2 "[debug] output_dir        = '${output_dir}'!"
    echo >&2 "[debug] server_model_name = '${server_model_name}'!"
    echo >&2 "[debug] output_dir        = '${output_dir}'!"
    viremos_xml2viredev \
	-L ${logging_label} \
    	--input-xml-file ${full_xml_file} \
    	--output-dir ${output_dir} \
    	--path-prefix "${snemo_path_prefix}" \
    	--server-model-name ${server_model_name}
    if [ -n ${removable_dir} -a -d ${removable_dir} ]; then
	echo >&2 "[notice] Removing '${removable_dir}'..."
	rm -fr ${removable_dir}/
    fi
    tree ${output_dir}
    if [ -f ${mos_devices_db} ]; then
	rm -f ${mos_devices_db}
    fi
    return 0
}

function build_all_device_models()
{
    for key in "${!snemo_mosdev_mountpoints[@]}"; do
	xmlmodelfile=${snemo_mosdev_xmlfiles[${key}]}
	build_device_model ${xmlmodelfile}
    done

    cd ${work_dir}/
    mos_model_files=$(find ./ -name "mos_models.vdm")
    cd ${opwd}
    echo >&2 "[notice] MOS models files = ${mos_model_files}"
    if [ -f ${work_dir}/mos_models.lis ]; then
	rm -f ${work_dir}/mos_models.lis
    fi
    touch ${work_dir}/mos_models.lis
    for filepath in ${mos_model_files}; do
    	echo >&2 "[notice] Recording MOS models file '${filepath}'..."
    	relfilepath=$(echo ${filepath} | sed -e "s@\./@@g")
    	echo >&2 "[notice]   path = '${relfilepath}'..."
    	echo "${snemo_path_prefix}/${relfilepath}" >> ${work_dir}/mos_models.lis
    done
    if [ -f ${mos_devices_db} ]; then
	rm -f ${mos_devices_db}
    fi

    if [ ${install} == true ]; then
	mkdir -p ${snemo_devices_cms_models_dir}/MOS/
	cp -f ${work_dir}/mos_models.lis ${snemo_devices_cms_models_dir}/MOS/
	cd ${work_dir}/
	dir_list=$(find . -mindepth 1 -maxdepth 1 -a -type d)
	cd ${opwd}
	for dir in ${dir_list}; do
	   cp -fa ${work_dir}/${dir} ${snemo_devices_cms_models_dir}/MOS/
	done
	tree ${snemo_devices_cms_models_dir}/MOS/
    fi

    return 0
}

if [ -z "${action}" ]; then
    my_exit 1 "Missing action!"
fi

if [ ${dry_run} == false -a "${action}" == "print-status" ]; then
    print_status
fi

if [ ${dry_run} == false -a "${action}" == "list-of-devices" ]; then
    echo >&2 "[notice] Printing the list of MOS managed devices..."
    load_list_of_launched_devices
    # print_list_of_launched_devices
    print_list_of_mos_devices
fi

if [ ${dry_run} == false -a "${action}" == "build-device-model" ]; then
    if [ -z "${build_device_model_xmlfile}" ]; then
	my_exit 1 "Missing device model"
    fi
    echo >&2 "[notice] Trying to build device model from '${build_device_model_xmlfile}'..."
    load_list_of_launched_devices
    build_device_model ${build_device_model_xmlfile}
    if [ $? -ne 0 ]; then
	my_exit 1 "Unsupported device model '${build_device_model_xmlfile}'!"
    else
	echo >&2 "[debug] Device model = '${build_device_model_xmlfile}' is built."
    fi
fi

if [ ${dry_run} == false -a "${action}" == "build-all-device-models" ]; then
    echo >&2 "[notice] Trying to build all device models..."
    load_list_of_launched_devices
    build_all_device_models
fi

my_exit 0

# end
