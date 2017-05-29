//! \file  vire/message/testing/test-message-message.cxx
//! \brief Vire message
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
#include <vire/message/message.h>

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
#include <jsontools/iofile.h>

// This project:
#include <vire/vire.h>
#include <vire/time/utils.h>
#include <vire/message/body_layout.h>
#include <vire/utility/base_error.h>
/// #include "error_event.h"

void test_message_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::message::message' class."
              << std::endl;

    test_message_1();

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

void test_message_1()
{
  std::clog << "\ntest_message_1: basics" << std::endl;

  {
    // Create an error event payload:
    vire::utility::base_error my_error(3, "A basic error");
    my_error.tree_dump(std::clog, "Error event: ");
    std::clog << std::endl;

    // Create a message:
    vire::message::message msg;

    vire::message::message_header & h = msg.grab_header();
    vire::message::message_identifier msg_id("vire.server", 42);
    vire::utility::model_identifier body_layout_id;
    body_layout_id.set_name(vire::message::body_layout::name());
    body_layout_id.set_version(vire::message::body_layout::current_version());
    h.set_message_id(msg_id);
    h.set_timestamp(vire::time::now());
    h.set_category(vire::message::MESSAGE_EVENT);
    h.set_in_reply_to(vire::message::message_identifier("vire.client.0", 23));
    h.set_asynchronous(false);
    h.set_body_layout_id(body_layout_id);
    h.add_metadata("key1", "foo");
    h.add_metadata("key2", 42);
    h.add_metadata("key3", 3.14159);
    h.add_metadata("key4", true);
    vire::message::message_body & b = msg.grab_body();
    b.set_payload(my_error);

    msg.tree_dump(std::clog, "Message: ");

    {
      std::clog << "test_message_1: Serializing the message..." << std::endl;
      datatools::data_writer writer("test-message-message.xml");
      writer.store(msg);
      std::clog << "test_message_1: Serializing the JSON message..." << std::endl;
      jsontools::store("test-message-message.json", msg);
    }
    std::clog << std::endl;
  }

  {
    vire::message::message msg;
    {
      std::clog << "test_message_1: Deserializing the message..." << std::endl;
      datatools::data_reader reader("test-message-message.xml");
      reader.load(msg);
    }
    msg.tree_dump(std::clog, "Loaded message (from XML Boost/archive): ");
    std::clog << std::endl;
  }

  {
    vire::message::message msg;
    {
      std::clog << "test_message_1: Deserializing the JSON message..." << std::endl;
      jsontools::load("test-message-message.json", msg);
    }
    msg.tree_dump(std::clog, "Loaded message (from JSON format): ");
    std::clog << std::endl;
  }

  return;
}
