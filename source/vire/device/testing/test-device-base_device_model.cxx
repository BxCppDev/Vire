//! \file device/testing/test-device-base_device_model.cxx
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>

// This project:
#include <vire/device/device_types.h>
#include <vire/device/base_device_model.h>
#include <vire/device/base_port_model.h>
#include <vire/device/device_utils.h>
#include <vire/device/single_instance.h>
#include <vire/device/regular_1d_instance.h>
#include <vire/device/multiple_instance.h>
#include <vire/device/base_datapoint_model.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

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

    vire::device::regular_1d_instance many_ports_instance;
    vire::device::slot port_first_slot(vire::device::space::DIM_1, 3);
    many_ports_instance.set(port_first_slot, 10, 1);

    vire::device::physical_port & many_ports = dev_model.add_port("many_ports",
                                                                  port_model,
                                                                  many_ports_instance);
    dev_model.initialize_simple();
    dev_model.tree_dump(std::clog, "Virtual device model: ");
    std::clog << std::endl;

    // Setup model:
    vire::device::base_device_model setup_model;
    setup_model.set_name("MySetupModel");
    setup_model.set_display_name("My setup model");
    setup_model.set_terse_description("A dummy setup model");
    setup_model.set_type(vire::device::TYPE_SETUP);
    setup_model.set_model_key(1000);
    setup_model.grab_auxiliaries().store_string("model.label", "Dummy");
    setup_model.grab_auxiliaries().store_string("model.version", "1.0");
    setup_model.grab_auxiliaries().store_string("model.manufacturer", "ACME");

    // Add devices in the setup:
    vire::device::physical_device & devA = setup_model.add_embedded_device("devA", dev_model);
    vire::device::physical_device & devB = setup_model.add_embedded_device("devB", dev_model);
    vire::device::physical_device & devC = setup_model.add_embedded_device("devC", dev_model);

    vire::device::multiple_instance devs_instance;
    devs_instance.append_slot(vire::device::slot(vire::device::space::DIM_1, 7));
    devs_instance.append_slot(vire::device::slot(vire::device::space::DIM_1, 8));
    devs_instance.append_slot(vire::device::slot(vire::device::space::DIM_1, 12));
    devs_instance.append_slot(vire::device::slot(vire::device::space::DIM_1, 42));
    vire::device::physical_device & devs = setup_model.add_embedded_device("devs", dev_model, devs_instance);

    // Change our mind (remove the last inserted device):
    setup_model.remove_embedded_device("devB");
    setup_model.initialize_simple();
    setup_model.tree_dump(std::clog, "Virtual setup model: ");
    std::clog << std::endl;

    // Datapoint model:
    vire::device::base_datapoint_model dp_model;
    dp_model.tree_dump(std::clog, "Virtual datapoint model: ");
    std::clog << std::endl;

    
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
