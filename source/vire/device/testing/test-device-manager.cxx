//! \file vire/device/testing/test-device-manager.cxx
//
// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of Vire.
//
// Vire is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Vire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/manager.h>

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>
#include <datatools/properties.h>
#include <datatools/utils.h>

// This project:
#include <vire/device/device_types.h>
#include <vire/device/device_utils.h>
#include <vire/device/base_electronics_module_model.h>
#include <vire/device/base_electronics_crate_model.h>
#include <vire/device/base_electronics_rack_model.h>

void test0();
void test1();

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {
    bool run_test0 = true;
    bool run_test1 = true;

    int iarg = 0;
    while (iarg < argc_) {
      std::string token(argv_[iarg]);
      if (token == "--no-test0") {
        run_test0 = false;
      } else if (token == "--no-test1") {
        run_test1 = false;
      }
      iarg++;
    }
    if (run_test0) {
       test0();
    }
    if (run_test1) {
      test1();
    }
  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}

void test0()
{
  std::clog << "\nTest 0: Entering..." << std::endl;

  // Device manager:
  vire::device::manager dev_mgr;
  dev_mgr.set_logging_priority(datatools::logger::PRIO_DEBUG);
  dev_mgr.set_name("devices");
  dev_mgr.set_display_name("Devices");
  dev_mgr.set_terse_description("The device manager of the experiment");
  dev_mgr.set_setup_label("vire::experiment");
  dev_mgr.set_setup_description("A mock experimental setup from the Vire API");
  dev_mgr.set_setup_version("0.1");
  dev_mgr.set_factory_preload_system_all(true);
  // dev_mgr.set_mapping_requested(false);
  dev_mgr.set_mapping_requested(true);
  dev_mgr.set_force_initialization_at_load(false);

  // Describe a Calorimeter Frontend board input port:
  datatools::properties calo_feb_input_port_config;
  {
    calo_feb_input_port_config.store("type", "generic");
    calo_feb_input_port_config.store("io_type", "in");
    calo_feb_input_port_config.store("format", "lemo-coaxial-00");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_port_model("calo_feb_input",
                          "vire::device::base_port_model",
                          calo_feb_input_port_config);

  // Describe a Calorimeter Frontend board output port:
  datatools::properties calo_feb_output_port_config;
  {
    calo_feb_output_port_config.store("type", "generic");
    calo_feb_output_port_config.store("io_type", "out");
    calo_feb_output_port_config.store("format", "backplane_bus");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_port_model("calo_feb_output",
                          "vire::device::base_port_model",
                          calo_feb_output_port_config);

  // Describe a Calorimeter Frontend board chip:
  datatools::properties calo_feb_chip_config;
  {
    calo_feb_chip_config.store("format", "220-pin FPGA");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_device_model("calo_feb_chip",
                            "vire::device::base_integrated_circuit_model",
                            calo_feb_chip_config);

  // Describe a Calorimeter Frontend board:
  datatools::properties calo_feb_config;
  {
    calo_feb_config.store("model_key", 10000);
    calo_feb_config.store("type", "module_board");
    calo_feb_config.store("allow_embedded_devices", true);
    calo_feb_config.store("format", "VME-6U-extended");
    calo_feb_config.store("slot_width", 1);
    calo_feb_config.store("max_number_of_mezzanine_modules", 0);
    calo_feb_config.store("aux.manufacturer", "LAL Orsay");
    calo_feb_config.store("aux.model", "16-channel WaveCatcher Board");
    calo_feb_config.store("aux.version", "1.0");

    // Ports:
    std::vector<std::string> calo_feb_port_labels;
    calo_feb_port_labels.push_back("inputs");
    calo_feb_port_labels.push_back("output");
    calo_feb_config.store("ports.labels", calo_feb_port_labels);
    calo_feb_config.store("ports.model.inputs", "calo_feb_input");
    calo_feb_config.store("ports.instance.inputs",
                          "vire::device::regular_1d_instance=[nslots=16;first_slot=(1:0);step=(1)]");
    calo_feb_config.store("ports.model.output",
                          "calo_feb_output");
    calo_feb_config.store("ports.instance.output",
                          "vire::device::single_instance=[slot=(0)]");

    // Embedded devices:
    std::vector<std::string> calo_feb_device_labels;
    calo_feb_device_labels.push_back("chips");
    calo_feb_config.store("embedded_devices.labels", calo_feb_device_labels);
    calo_feb_config.store("embedded_devices.model.chips",
                          "calo_feb_chip");
    calo_feb_config.store("embedded_devices.instance.chips",
                          "vire::device::regular_1d_instance=[nslots=3;first_slot=(1:0); step=(1)]");

    // Mapping rules:
    calo_feb_config.store("mapping.port.inputs",  "[calo_fr_input.dc:input+0]");
    calo_feb_config.store("mapping.device.chip0", "[calo_fe_fpga.dc:fe_fpga=0]");
    calo_feb_config.store("mapping.device.chip1", "[calo_fe_fpga.dc:fe_fpga=1]");
    calo_feb_config.store("mapping.device.chip2", "[calo_ctrl_fpga.dc:ctrl_fpga]");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_device_model("calo_feb",
                            "vire::device::base_electronics_module_model",
                            calo_feb_config);

  std::clog << "DEVEL: " << "TEST A" << std::endl;

  // Describe a Calorimeter Control board:
  datatools::properties calo_cb_config;
  {
    calo_cb_config.store("model_key", 12000);
    calo_cb_config.store("type", "module_board");
    calo_cb_config.store("allow_embedded_devices", true);
    calo_cb_config.store("format", "VME-6U-extended");
    calo_cb_config.store("slot_width", 1);
    calo_cb_config.store("max_number_of_mezzanine_modules", 0);
  }
  // Load the description of the model in the manager:
  dev_mgr.load_device_model("calo_cb",
                            "vire::device::base_electronics_module_model",
                            calo_cb_config);

  // Describe a Calorimeter Crate:
  datatools::properties calo_crate0_config;
  {
    calo_crate0_config.store("model_key", 5000);
    calo_crate0_config.store("type", "crate");
    calo_crate0_config.store("allow_embedded_devices", true);
    calo_crate0_config.store("format", "VME-6U-extended");
    calo_crate0_config.store("max_number_of_modules", 21);
    calo_crate0_config.store("modules.slot_0.model", "calo_feb");
    calo_crate0_config.store("modules.slot_0.label", "FEB0");
    calo_crate0_config.store("modules.slot_1.model", "calo_feb");
    calo_crate0_config.store("modules.slot_1.label", "FEB1");
    calo_crate0_config.store("modules.slot_2.model", "calo_feb");
    calo_crate0_config.store("modules.slot_2.label", "FEB2");
    calo_crate0_config.store("modules.slot_3.model", "calo_feb");
    calo_crate0_config.store("modules.slot_3.label", "FEB3");
    calo_crate0_config.store("modules.slot_4.model", "calo_feb");
    calo_crate0_config.store("modules.slot_4.label", "FEB4");
    calo_crate0_config.store("modules.slot_10.model", "calo_cb");
    calo_crate0_config.store("modules.slot_10.label", "CB");
    calo_crate0_config.store("modules.slot_11.model", "calo_feb");
    calo_crate0_config.store("modules.slot_11.label", "FEB10");
    calo_crate0_config.store("modules.slot_12.model", "calo_feb");
    calo_crate0_config.store("modules.slot_12.label", "FEB11");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_device_model("calo_crate0",
                            "vire::device::base_electronics_crate_model",
                            calo_crate0_config);

  // Describe a Calorimeter Rack:
  datatools::properties calo_rack0_config;
  {
    calo_rack0_config.store("format", "VME-6U-extended");
    calo_rack0_config.store("max_number_of_crates", 3);
    calo_rack0_config.store("crates.slot_0.label", "Crate0");
    calo_rack0_config.store("crates.slot_0.model", "calo_crate0");
    calo_rack0_config.store("crates.slot_1.label", "Crate1");
    calo_rack0_config.store("crates.slot_1.model", "calo_crate0");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_device_model("calo_rack0",
                            "vire::device::base_electronics_rack_model",
                            calo_rack0_config);


  // Describe the electronics:
  datatools::properties electronics_config;
  {
    electronics_config.store("type", "generic");
    std::vector<std::string> subdevlabels = { "CaloRack0", "CaloRack1" };
    electronics_config.store("embedded_devices.labels", subdevlabels);
    electronics_config.store("embedded_devices.model.CaloRack0", "calo_rack0");
    electronics_config.store("embedded_devices.model.CaloRack1", "calo_rack0");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_device_model("electronics",
                            "vire::device::base_device_model",
                            electronics_config);

  // Describe the detector:
  datatools::properties detector_config;
  {
    detector_config.store("type", "generic");
    // std::vector<std::string> subdevlabels = { "CaloSubmodule0", "CaloSubmodule1" };
    // detector_config.store("embedded_devices.labels", subdevlabels);
    // detector_config.store("embedded_devices.model.CaloSubmodule0", "calo_submodule");
    // detector_config.store("embedded_devices.model.CaloSubmodule0", "calo_submodule");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_device_model("detector",
                            "vire::device::base_device_model",
                            detector_config);

  // Describe the demonstrator experiment:
  datatools::properties demonstrator_config;
  {
    demonstrator_config.store("type", "generic");
    std::vector<std::string> subdevlabels = { "Electronics", "Detector" };
    demonstrator_config.store("embedded_devices.labels", subdevlabels);
    demonstrator_config.store("embedded_devices.model.Electronics", "electronics");
    demonstrator_config.store("embedded_devices.model.Detector",    "detector");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_device_model("demonstrator",
                            "vire::device::base_device_model",
                            demonstrator_config);

  // Describe the setup:
  datatools::properties setup_config;
  {
    setup_config.store("type", "setup");
    std::vector<std::string> subdevlabels = { "Demonstrator" };
    setup_config.store("embedded_devices.labels", subdevlabels);
    setup_config.store("embedded_devices.model.Demonstrator", "demonstrator");
  }
  // Load the description of the model in the manager:
  dev_mgr.load_device_model("setup",
                            "vire::device::base_device_model",
                            setup_config);

  std::clog << "DEVEL: " << "TEST B" << std::endl;

  dev_mgr.initialize_simple();
  dev_mgr.tree_dump(std::clog, "Virtual device manager: ");
  std::clog << std::endl;

  const vire::device::base_device_model & setup =
    dev_mgr.get_device_model_as<vire::device::base_device_model>("setup");
  setup.tree_dump(std::clog, "Setup: ");
  std::clog << std::endl;

  std::clog << "DEVEL: " << "TEST C" << std::endl;

  const vire::device::base_electronics_module_model & calo_feb =
    dev_mgr.get_device_model_as<vire::device::base_electronics_module_model>("calo_feb");
  calo_feb.tree_dump(std::clog, "Calorimeter frontend board: ");
  std::clog << std::endl;

  std::clog << "DEVEL: " << "TEST D" << std::endl;

  const vire::device::base_electronics_rack_model & calo_rack =
    dev_mgr.get_device_model_as<vire::device::base_electronics_rack_model>("calo_rack0");
  calo_rack.tree_dump(std::clog, "Calorimeter rack: ");
  std::clog << std::endl;

  std::clog << "DEVEL: " << "TEST E" << std::endl;

  dev_mgr.tree_dump(std::clog, "Virtual device manager: ");
  std::clog << std::endl;

  std::clog << "\nTest 0: Exiting." << std::endl;
  return;
}

void test1()
{
  std::clog << "\nTest 1: Entering..." << std::endl;

  // Device manager:
  vire::device::manager dev_mgr;

  datatools::properties dev_mgr_config;
  std::string dev_mgr_config_filename = "${VIRE_BASE_SOURCE_DIR}/device/testing/config/test-device-manager.conf";
  datatools::fetch_path_with_env(dev_mgr_config_filename);
  std::clog << "\nTest 1: Config file: " << dev_mgr_config_filename << std::endl;
  dev_mgr_config.read_configuration(dev_mgr_config_filename);
  dev_mgr.initialize_standalone(dev_mgr_config);
  dev_mgr.tree_dump(std::clog, "Virtual device manager: ");
  std::clog << std::endl;

  const vire::device::base_electronics_module_model & calo_feb =
    dev_mgr.get_device_model_as<vire::device::base_electronics_module_model>("calo_feb");
  calo_feb.tree_dump(std::clog, "Calorimeter frontend board: ");
  std::clog << std::endl;

  std::clog << "\nTest 1: Exiting." << std::endl;
  return;
}
