//! \file vire/com/testing/test-com-boost_serialization_encoding_driver.cxx
//! \brief Test Vire communication Boost/Serialization encoding driver
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

// Standard library:
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include <map>

// This project:
#include <vire/vire.h>
#include <vire/com/boost_serialization_encoding_driver.h>
#include <vire/message/message.h>
#include <vire/utility/base_alarm.h>
#include <vire/message/body_layout.h>

void test_com_bsed_1(bool interactive_ = false);
void test_com_bsed_2(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::boost_serialization_encoding_driver' class."
              << std::endl;

    bool interactive = false;
    interactive = true;

    test_com_bsed_1(interactive);
    test_com_bsed_2(interactive);

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

void test_com_bsed_1(bool interactive_)
{
  std::clog << "\ntest_com_bsed_1: basics" << std::endl;

  vire::com::boost_serialization_encoding_driver::archive_format_type ar_format
    = vire::com::boost_serialization_encoding_driver::AR_XML;
  ar_format = vire::com::boost_serialization_encoding_driver::AR_TEXT;
  vire::com::boost_serialization_encoding_driver bsEncodingDriver;
  bsEncodingDriver.set_archive_format(ar_format);
  bsEncodingDriver.initialize_simple();

  vire::com::raw_message_type raw_msg;
  {
    // Create an error event payload:
    vire::utility::base_alarm my_alarm("warning", "A simple warning");
    my_alarm.tree_dump(std::clog, "Alarm event: ");
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
    h.set_in_reply_to(vire::message::message_identifier("vire.client.0.BubeiB2T", 23));
    // h.set_asynchronous(true);
    h.set_async_address("snemo.vire.cms.alarm");
    h.set_body_layout_id(body_layout_id);

    vire::message::message_body & b = msg.grab_body();
    b.set_payload(my_alarm);
    msg.tree_dump(std::clog, "Message: ");

    bsEncodingDriver.encode(msg, raw_msg);

    std::clog << "===========================================" << std::endl;
    std::clog << "Buffer: " << std::endl;
    std::clog << "===========================================" << std::endl;
    for (auto c : raw_msg.buffer) {
      std::clog << c;
    }
    std::clog << "===========================================" << std::endl;

  }
  {
    vire::message::message msg;
    bsEncodingDriver.decode(raw_msg, msg);
    msg.tree_dump(std::clog, "Loaded message: ");
  }
  return;
}

void test_com_bsed_2(bool interactive_)
{
  std::clog << "\ntest_com_bsed_2: basics" << std::endl;

  vire::com::boost_serialization_encoding_driver::archive_format_type ar_format
    = vire::com::boost_serialization_encoding_driver::AR_XML;
  vire::com::boost_serialization_encoding_driver bsEncodingDriver;
  bsEncodingDriver.set_archive_format(ar_format);
  bsEncodingDriver.initialize_simple();

  vire::com::raw_message_type raw_msg;
  {
    // Create an error event payload:
    vire::utility::base_alarm my_alarm("warning", "A simple warning");
    my_alarm.tree_dump(std::clog, "Alarm event: ");
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
    // h.set_asynchronous(true);
    h.set_async_address("snemo.vire.cms.alarm");
    h.set_body_layout_id(body_layout_id);

    vire::message::message_body & b = msg.grab_body();
    b.set_payload(my_alarm);
    msg.tree_dump(std::clog, "Message: ");

    bsEncodingDriver.encode(msg, raw_msg);

    std::clog << "===========================================" << std::endl;
    std::clog << "Buffer: " << std::endl;
    std::clog << "===========================================" << std::endl;
    for (auto c : raw_msg.buffer) {
      std::clog << c;
    }
    std::clog << "===========================================" << std::endl;

  }
  {
    vire::message::message msg;
    bsEncodingDriver.decode(raw_msg, msg);
    msg.tree_dump(std::clog, "Decoded message: ");
  }
  return;
}
