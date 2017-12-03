#!/usr/bin/env python3

import sys
import getopt
import os
import os.path
import pathlib

class mos_device_mgr :
    """MOS device manager utility"""

    def __init__(self,
                 devlaunchfile_,
                 output_dir_ = "",
                 base_path_ = "SuperNEMO:/Demonstrator/CMS",
                 model_name_prefix_ = "sndemo"):
        self.devlaunchfilename = devlaunchfile_
        self.output_dir = output_dir_
        self.base_path = base_path_
        self.model_name_prefix = model_name_prefix_
        self.mos_infos = []
        self.debug = False
        return

    def set_debug(self, flag_ = True) :
        self.debug = flag_
        return

    def get_mos_infos(self) :
        return self.mos_infos

    def build_output_dir(self, path_):
        p = pathlib.Path(self.output_dir)
        q = p / path_
        return q

    # def prepare_output_dir(self, path_):
    #     q = self.build_output_dir(path_)
    #     pathlib.Path(q).mkdir(parents=True, exist_ok=True)

    def parse_devices_launch_file(self):
        devlaunchfile = open(self.devlaunchfilename , "r")
        lines = devlaunchfile.readlines()
        devlaunchfile.close()
        for line in lines :
            sline=line.strip()
            if len(sline) == 0 :
                if self.debug :
                    print("[debug] Empty line.")
                continue
            if sline[0] == '#' :
                if self.debug :
                    print("[debug] Commented line.")
                continue
            if self.debug :
                print("[debug] Current line is '{}'".format(sline))
            tokens = sline.split()
            mos_info_record = {}
            mos_info_record["host"] = tokens[0]
            mos_info_record["port"] = int(tokens[1])
            mos_info_record["user"] = tokens[2]
            xmlfile = tokens[3]
            if xmlfile[0] == "/" :
                xmlfile = xmlfile[1:]
            mos_info_record["xmlfile"] = xmlfile
            mos_info_record["namespace"] = tokens[4]
            mos_info_record["mountpoint"] = tokens[5]
            if self.debug :
                print("[debug] MOS info record = {}".format(mos_info_record))
            if mos_info_record["mountpoint"].startswith(self.base_path) :
                self.mos_infos.append(mos_info_record)
        return

    # def exec_xml2viredev_command(self, xmlfile_, outputdir_, modelname_):
    #     command = "viremos_xml2viredev " \
    #               + " --output-dir " + str(outputdir_) \
    #               + " --input-xml-file " + xmlfile_ \
    #               + " --server-model-name "  + modelname_
    #     print("\ncommand : {}".format(command))
    #     #os.system(command)
    #     return

    def populate_mos_infos(self):
        for mos_info in self.mos_infos:
            host = mos_info["host"]
            port = mos_info["port"]
            xmlfile = mos_info["xmlfile"]
            mountpoint = mos_info["mountpoint"]
            # print("Vire mount point = '{}'".format(mountpoint))
            mountpoint2 = mountpoint[len(self.base_path)+1:]
            if mountpoint2[-1] == "/" :
                mountpoint2 = mountpoint2[0:-1]
            output_dir = mountpoint2 #self.build_output_dir(mountpoint2)
            mos_namespace = mos_info["namespace"].split('/')[1]
            tokens=mos_info["xmlfile"].split('/')
            # print("  MOS namespace tokens = '{}'".format(tokens))
            mp_tokens = mountpoint2.split("/")[0:-1]
            if self.debug :
                print("[debug] mountpoint MP tokens = '{}'".format(mp_tokens))
            mp_path=""
            if len(mp_tokens) :
                mp_path="." + mp_tokens[0]
                for tok in mp_tokens[1:] :
                    mp_path = mp_path + '.' + tok
            if self.debug :
                print("[debug] mountpoint MP path = '{}'".format(mp_path))
            # mos_server_model_name = self.model_name_prefix  + ".mos." + mountpoint2.replace("/", ".") + "." + tokens[-1].split(".")[0]
            mos_server_model_name = self.model_name_prefix  + ".mos" + mp_path + "." + tokens[-1].split(".")[0]

            mos_info["outputdir"]   = mos_namespace
            mos_info["mountpoint2"] = mountpoint2
            mos_info["modelname"]   = mos_server_model_name
            if self.debug :
                print("[debug] MOS info : ")
                print("[debug]  Host = '{}'".format(host))
                print("[debug]  Port = '{}'".format(port))
                print("[debug]  XML file = '{}'".format(xmlfile))
                print("[debug]  Vire mount point = '{}'".format(mountpoint2))
                print("[debug]  Output dir = '{}'".format(output_dir))
                print("[debug]  MOS namespace = '{}'".format(mos_namespace))
                print("[debug]  MOS server model name = '{}'".format(mos_server_model_name))
            # self.prepare_output_dir(mountpoint2)
            # self.exec_xml2viredev_command(xmlfile, output_dir, mos_server_model_name)
        return

    def store_mos_infos(self, filename_):
        fout = open(filename_, "w")
        for mos_info in self.mos_infos:
            fout.write("{};{};{};{};{}\n".format(mos_info["xmlfile"],
                                                 mos_info["mountpoint"],
                                                 mos_info["mountpoint2"],
                                                 mos_info["outputdir"],
                                                 mos_info["modelname"]

            ))

        return

    def populate_model_dirs(self):
        for mos_info in self.mos_infos:
            # self.prepare_output_dir(mountpoint2)
            # self.exec_xml2viredev_command(xmlfile, output_dir, mos_server_model_name)
            pass
        return

def main(argv_):
    devlaunchfile = None
    outputfile = None
    basepath = None
    debug=False
    # print('Number of arguments:', len(argv_), 'arguments.')
    # print('Argument List:', str(argv_))
    try:
        opts, args = getopt.getopt(argv_, "l:o:n", ["launcher=", "output-file=", "debug"])
    except getopt.GetoptError:
        print (argv_[0] + " -l <devlaunchfile>")
        return 2
    for opt, arg in opts :
        if opt in ("-l", "--launcher"):
            devlaunchfile = arg
        if opt in ("-o", "--output-file"):
            outputfile = arg
        if opt in ("-n", "--debug"):
            debug = True

    if devlaunchfile == None :
        raise "Missing device launcher filename!"

    if outputfile == None :
        outputfile = "sndemo_mos_devices.lis"

    if basepath == None :
        basepath = "SuperNEMO:/Demonstrator/CMS"

    if debug :
        print("[debug] Device launch file is : '{}'".format(devlaunchfile))
        print("[debug] Output file is        : '{}'".format(outputfile))
        print("[debug] Base device path is   : '{}'".format(basepath))

    dlm = mos_device_mgr(devlaunchfile,
                         "",
                         basepath)
    dlm.set_debug(debug)
    dlm.parse_devices_launch_file()
    dlm.populate_mos_infos()
    dlm.store_mos_infos(outputfile)
    return 0

if __name__ == "__main__" :
    error_code = main(sys.argv[1:])
    sys.exit(error_code)
#
