//! \file  vire/message/testing/test-message-message_body.cxx
//! \brief Vire message body
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
#include <vire/message/message_body.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/io_factory.h>
#include <bayeux/datatools/archives_instantiation.h>
// - BxJsontools
#include <bayeux/jsontools/iofile.h>
// - BxProtobuftools
#include <bayeux/protobuftools/iofile.h>

// This project:
#include <vire/vire.h>
#include <vire/time/utils.h>
#include <vire/utility/base_error.h>

void test_message_body_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::message::message_body' class."
              << std::endl;

    test_message_body_1();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

void test_message_body_1()
{
  std::clog << "\ntest_message_body_1: basics" << std::endl;

  {
    // Create an error object payload:
    vire::utility::base_error my_error(3, "A basic error");

    // Create a message body with embeded error event payload:
    vire::message::message_body msgbody;
    msgbody.set_payload(my_error);
    msgbody.tree_dump(std::clog, "Created message body: ");
    std::clog << std::endl;

    const vire::utility::base_error & be_cref =
      msgbody.get_payload_as<vire::utility::base_error>();
    be_cref.tree_dump(std::clog, "The base error: ");

    {
      std::clog << "test_message_body_1: Boost-serializing the message body..." << std::endl;
      datatools::data_writer writer("test-message-message_body.xml");
      writer.store(msgbody);
    }

    {
      std::clog << "test_message_body_1: JSON-serializing the message body..." << std::endl;
      jsontools::store("test-message-message_body.json", msgbody);
    }

    {
      std::clog << "test_message_body_1: Protobuf-serializing the message body..." << std::endl;
      protobuftools::store("test-message-message_body.protobuf", msgbody, protobuftools::IO_DEBUG);
    }

    // vire::message::message_body msgbody;
    msgbody.reset();
    {
      std::clog << "test_message_body_1: JSON-deserializing the message body..." << std::endl;
      jsontools::load("test-message-message_body.json", msgbody);
    }
    msgbody.tree_dump(std::clog, "Loaded JSON message body: ");
    std::clog << std::endl;
  }

  return;
}
