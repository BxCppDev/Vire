//! \file vire/com/testing/test-com-mailbox.cxx
//! \brief Test Vire communication mailbox
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <iostream>
#include <vector>

// This project:
#include <vire/vire.h>
#include <vire/com/mailbox.h>

void test_com_mailbox_0();
void test_com_mailbox_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::mailbox' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_com_mailbox_0();
    // test_com_mailbox_1(interactive);

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return error_code;
}

void test_com_mailbox_0()
{
  std::clog << "\ntest_com_mailbox_0: permissions..." << std::endl;

  std::vector<std::string> words = {
    "--------",   // Too short
    "----------", // Too long
    "---------",  // Ok
    "pkspkspks",  // Ok
    "p--------",  // Ok
    "-p-------",  // No
    "-k-------",  // Ok
    "--s------",  // Ok
    "--sp---k-",  // Ok
    "k--------",  // No
    "-----s---",  // Ok
    "-k-pk-p-s",  // Ok
    "a--------"   // No
  };

  for (const std::string & word : words) {
    vire::com::mailbox::permissions_type perms;
    if (!vire::com::mailbox::usage_permission_from_string(word, perms)) {
      std::cerr << "[error] Invalid mailbox permissions representation: '"
                << word << "'" << std::endl;
    } else  {
      std::cerr << "[info] Mailbox permissions is: ["
                << perms.to_string() << "] as {"
                << vire::com::mailbox::usage_permission_to_string(perms) << '}' << std::endl;
    }
  }
  std::clog << std::endl;

  return;
}

void test_com_mailbox_1(bool /*interactive_*/)
{
  std::clog << "\ntest_com_mailbox_1: basics..." << std::endl;

  vire::com::mailbox mb0;
  mb0.set_name("FooRequest");
  mb0.set_address("foo.service");
  mb0.set_mode(vire::com::mailbox::MODE_SERVICE);
  mb0.set_privacy(vire::com::mailbox::PRIVACY_PUBLIC);
  mb0.set_permissions("--s---p--");
  mb0.lock(true);
  mb0.tree_dump(std::clog, "Mailbox #0 : ");
  std::clog << std::endl;

  return;
}
