//! \file utility/testing/test-utility-properties.cxx
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <datatools/properties.h>

// This project:
#include <vire/utility/properties.h>
#include <vire/utility/model_identifier.h>
#include <vire/utility/instance_identifier.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    // Instance identifier:
    vire::utility::instance_identifier configId("supernemo");
    // std::clog << "Config ID : " << configId.to_string() << std::endl;

    // Model identifier:
    vire::utility::model_identifier topicId("vire::setup::configuration");
    // std::clog << "Topic ID : " << topicId.to_string() << std::endl;

    vire::utility::properties config;
    config.set_id(configId);
    config.set_model(topicId);
    config.add_property("test", "1", "bool");
    config.add_property("counter", "42", "integer");
    config.add_property("vire.api.version", "1.0.0", "string");
    config.add_property("com.default_encoding", "protobuf", "string");
    config.add_property("com.default_transport", "rabbitmq", "string");
    config.add_property("length", "3.0", "real", "unit=cm");
    config.add_property("logfile", "/var/log/vire.log", "string", "path");
    config.tree_dump(std::clog, "Properties: ");
    config.remove_property("com.default_transport");

    // Export to a Bayeux properties container:
    datatools::properties bxconfig;
    config.export_to(bxconfig);
    bxconfig.tree_dump(std::clog, "Bayeux properties: ");

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
