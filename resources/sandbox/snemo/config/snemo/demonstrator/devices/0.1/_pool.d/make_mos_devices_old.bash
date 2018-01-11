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

tmp_dir="./_tmp.d"
output_dir="./_tmp.d"
input_xml_file=""
model_name=""

function parse_args()
{
    while [ -n "$1" ]; do
	arg="$1"
	if [ "${arg}" == "--output-dir" -o "${arg}" == "-D" ] ; then
	    shift 1
	    output_dir="$1"
	elif [ "${arg}" == "--input-xml-file" -o "${arg}" == "-X" ] ; then
	    shift 1
	    input_xml_file="$1"
	elif [ "${arg}" == "--model-name" -o "${arg}" == "-N" ] ; then
	    shift 1
	    model_name="$1"
	fi
	shift 1
    done
    return 0
}

echo >&2 "[info] Output dir     = '${output_dir}'"
echo >&2 "[info] Input XML file = '${input_xml_file}'"
echo >&2 "[info] Model name     = '${model_name}'"

which virequery > /dev/null 2>&1
if [ $? -ne 0 ]; then
    my_exit 1 "Cannot find virequery!"
fi

mos_to_vire_bin=$(which viremos_xml2viredev)
if [ -z "${mos_to_vire_bin}" ]; then
    my_exit 1 "Cannot find viremos_xml2viredev!"
fi

if [ -d ${tmp_dir} ]; then
    echo >&2 "[info] Remove temporary directory '${tmp_dir}'..."
    # rm -fr ${tmp_dir}
fi

mkdir -p ${tmp_dir}
mkdir -p ${tmp_dir}/mosdev
cd ${tmp_dir}

mkdir -p ${output_dir}
cd ${output_dir}
pwd

vire_version=$(virequery --version)
vire_mos_xsd_version=2.0.3
vire_mos_xsd_dir="$(virequery --resourcedir)/plugins/Vire_MOS/data/mos/xml/${vire_mos_xsd_version}"

snemo_cms_mos_version=0.9.2
snemo_cms_mos_xml_dir="$(virequery --resourcedir)/sandbox/snemo/config/snemo/demonstrator/cms/mos_xml/${snemo_cms_mos_version}"
snemo_cms_mos_dev_launch="${snemo_cms_mos_xml_dir}/SNEMO/devices_launch.conf"

snemo_devices_version=0.1
snemo_devices_dir="$(virequery --resourcedir)/sandbox/snemo/config/snemo/demonstrator/devices/${snemo_devices_version}"
snemo_devices_cms_models_dir="${snemo_devices_dir}/models/demonstrator/cms"
snemo_devices_cms_models_lis="${snemo_devices_cms_models_dir}/models.lis"

echo >&2 "[info] vire_version          = ${vire_version}"
echo >&2 "[info] vire_mos_xsd_version  = ${vire_mos_xsd_version}"
echo >&2 "[info] vire_mos_xsd_dir      = ${vire_mos_xsd_dir}"
echo >&2 "[info] snemo_cms_mos_version = ${snemo_cms_mos_version}"
echo >&2 "[info] snemo_cms_mos_xml_dir = ${snemo_cms_mos_xml_dir}"
echo >&2 "[info] snemo_devices_version = ${snemo_devices_version}"
echo >&2 "[info] snemo_devices_dir     = ${snemo_devices_dir}"
echo >&2 "[info] snemo_devices_cms_models_dir = ${snemo_devices_cms_models_dir}"
echo >&2 "[info] snemo_devices_cms_models_lis = ${snemo_devices_cms_models_lis}"

my_exit 2

# tree ${vire_mos_xsd_dir}
# tree ${snemo_cms_mos_xml_dir}
# tree ${snemo_devices_dir}

function build_cms_mos_xml_dir_list()
{
    cd ${snemo_cms_mos_xml_dir}
    local list=$(find . -type f -a -name "*.xml" | grep -v "Model.xml")
    echo ${list}
    cd ${opwd}
    return 0
}

xml_list=$(build_cms_mos_xml_dir_list)
echo >&2 "[info] xml_list=${xml_list}"

# xml_list="SNEMO/COIL/COIL_PS_1.xml"

# my_exit 2

for xml_file in ${xml_list}; do
    xmlfile=$(echo ${xml_file} | sed -s 's@^\./@@')
    echo >&2 "[info] XML file = '${xmlfile}'"
    fullxmlfile=${snemo_cms_mos_xml_dir}/${xmlfile}
    if [ -f ${fullxmlfile} ]; then
	echo >&2 "[info] Located at = '${fullxmlfile}'"
    fi
    # mos_svr_model_name="Coil_PS"

    mos_svr_model_name=$(cat ${fullxmlfile} | grep "<Name>" | head -2 | tail -1 | sed -e "s@<Name>@@" -e "s@</Name>@@" | tr -d "[[:space:]]")
    echo >&2 "[info] MOS server model name = '${mos_svr_model_name}'"

    continue
    # ${mos_to_vire_bin} \
    # 	--input-xml-file "${fullxmlfile}" \
    # 	--output-dir "${tmp_dir}/mosdev/SuperNEMO/Demonstrator/CMS" \
    # 	--server-model-name "${mos_svr_model_name}"
done

echo "TEST"
pwd
cd ${opwd}

tree ${tmp_dir}

my_exit 0

# end
