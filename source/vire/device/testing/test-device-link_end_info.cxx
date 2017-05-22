//! \file vire/device/testing/test-device-link_end_info.cxx
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/device/link_end_info.h>

// Standard library:
#include <iostream>

// This project:
#include <vire/device/base_device_model.h>
#include <vire/device/regular_1d_instance.h>
#include <vire/device/regular_2d_instance.h>
#include <vire/device/base_port_model.h>
#include <vire/device/link_end_info.h>
#include <vire/device/device_link_info.h>

void test0();

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    test0();

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

  // Port model:
  vire::device::base_port_model subport_model;
  subport_model.set_name("MySubPortModel");
  subport_model.set_display_name("My sub-port model");
  subport_model.set_terse_description("A mock sub-port model");
  subport_model.set_type(vire::device::PORT_GENERIC);
  subport_model.set_io_type(vire::device::PORT_IO_IN);
  subport_model.initialize_simple();
  subport_model.tree_dump(std::clog, "Virtual sub-port model: ");
  std::clog << std::endl;

  // Device submodel:
  vire::device::base_device_model subdev_model;
  subdev_model.set_name("MySubDeviceModel");
  subdev_model.set_display_name("My sub-device model");
  subdev_model.set_terse_description("A mock sub-device model");
  subdev_model.set_type(vire::device::TYPE_GENERIC);
  subdev_model.set_model_key(1210);
  vire::device::physical_port & portX= subdev_model.add_port("portX", subport_model);
  vire::device::physical_port & portY= subdev_model.add_port("portY", subport_model);
  vire::device::regular_1d_instance portZ_instance;
  vire::device::slot portZ_first_slot(vire::device::space::DIM_1, 5);
  portZ_instance.set(portZ_first_slot, 10, 1);
  vire::device::physical_port & portZ = subdev_model.add_port("portZ",
                                                              subport_model,
                                                              portZ_instance);

  subdev_model.initialize_simple();
  subdev_model.tree_dump(std::clog, "Virtual sub-device model: ");

  // Port model:
  vire::device::base_port_model port_model;
  port_model.set_name("MyPortModel");
  port_model.set_display_name("My port model");
  port_model.set_terse_description("A mock port model");
  port_model.set_type(vire::device::PORT_GENERIC);
  port_model.set_io_type(vire::device::PORT_IO_IN);
  port_model.grab_auxiliaries().store_string("physical.type", "socket");
  port_model.grab_auxiliaries().store_string("physical.kind", "female");
  port_model.grab_auxiliaries().store_string("physical.color", "metal");
  port_model.initialize_simple();
  port_model.tree_dump(std::clog, "Virtual port model: ");
  std::clog << std::endl;

  // Device model:
  vire::device::base_device_model dev_model;
  dev_model.set_name("MyDeviceModel");
  dev_model.set_display_name("My device model");
  dev_model.set_terse_description("A mock device model");
  dev_model.set_type(vire::device::TYPE_GENERIC);
  dev_model.set_model_key(1220);
  vire::device::physical_port & portA = dev_model.add_port("portA", port_model);
  vire::device::physical_port & portB = dev_model.add_port("portB", port_model);
  vire::device::physical_port & portC = dev_model.add_port("portC", port_model);
  vire::device::regular_1d_instance portD_instance;
  vire::device::slot portD_first_slot(vire::device::space::DIM_1, 2);
  portD_instance.set(portD_first_slot, 4, 2);
  portD_instance.tree_dump(std::clog, "PortD instance: ");
  vire::device::physical_port & portD_ports = dev_model.add_port("portD",
                                                                 port_model,
                                                                 portD_instance);
  vire::device::physical_device & subdev1 = dev_model.add_embedded_device("SubDev1", subdev_model);
  vire::device::regular_2d_instance subdev2_instance;
  vire::device::slot subdev2_first_slot(vire::device::space::DIM_2, 0, 0);
  subdev2_instance.set(subdev2_first_slot, 3, 2, 1, 0, 0, 1);
  vire::device::physical_device & subdev2 = dev_model.add_embedded_device("SubDev2",
                                                                          subdev_model,
                                                                          subdev2_instance);
  dev_model.initialize_simple();
  dev_model.tree_dump(std::clog, "Virtual device model: ");
  std::clog << std::endl;

  /*
    MyDeviceModel
    |-- portA (singleton)
    |-- portB (singleton)
    |-- portC (singleton)
    `-- portD (multiple, x3)


   */

    vire::device::link_end_info lei1;
    std::string link_end_info_repr1 = "portA";
    if (!lei1.parse(link_end_info_repr1, dev_model)) {
      std::cerr << "error: "
                << "Test 0: "
                << "Cannot parse link end info!"
                << std::endl;
    } else {
      lei1.tree_dump(std::clog, "Link end info: ");
    }

    vire::device::link_end_info lei2;
    std::string link_end_info_repr2 = "SubDev1/portX";
    if (!lei2.parse(link_end_info_repr2, dev_model)) {
      std::cerr << "error: "
                << "Test 0: "
                << "Cannot parse link end info!"
                << std::endl;
    } else {
      lei2.tree_dump(std::clog, "Link end info: ");
    }

    vire::device::link_end_info lei3;
    std::string link_end_info_repr3 = "SubDev1/portZ[slots={(1:10);(1:12);(1:14)}]";
    if (!lei3.parse(link_end_info_repr3, dev_model)) {
      std::cerr << "error: "
                << "Test 0: "
                << "Cannot parse link end info!"
                << std::endl;
    } else {
      lei3.tree_dump(std::clog, "Link end info: ");
    }

    vire::device::link_end_info lei4;
    std::string link_end_info_repr4 = "SubDev2[slots={(2:1,0);(2:2,1)}]/portZ[nslots=4;first_slot=(1:10);step=(-1)]";
    if (!lei4.parse(link_end_info_repr4, dev_model)) {
      std::cerr << "error: "
                << "Test 0: "
                << "Cannot parse link end info!"
                << std::endl;
    } else {
      lei4.tree_dump(std::clog, "Link end info: ");
    }

  vire::device::device_link_info devLink;
  devLink.set_from(lei4);
  devLink.set_to(lei3);
  devLink.tree_dump(std::clog, "Link info: ");

  return;
}
