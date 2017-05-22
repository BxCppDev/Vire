//! \file vire/com/testing/test-com-actor.cxx
//! \brief Test Vire communication actor
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
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include <map>

// This project:
#include <vire/vire.h>
#include <vire/com/actor.h>

void test_com_actor_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::actor' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_com_actor_1(interactive);

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

void test_com_actor_1(bool interactive_)
{
  std::clog << "\ntest_com_actor_1: basics" << std::endl;

  // Actor #0:
  std::clog << "\ntest_com_actor_1: Actor #0" << std::endl;
  vire::com::actor n0;
  n0.set_name("snemo.vire.server");
  n0.set_category(vire::com::actor::CATEGORY_SERVER);
  n0.tree_dump(std::clog, "Actor 0: ");
  std::clog << std::endl;

  // Actor #1:
  std::clog << "\ntest_com_actor_1: Actor #1" << std::endl;
  vire::com::actor n1;
  n1.set_name("snemo.vire.client.DZAEDH83DZED");
  n1.set_category(vire::com::actor::CATEGORY_CLIENT);
  n1.grab_metadata().store("rabbitmq.login", "foo");
  n1.grab_metadata().store("rabbitmq.passwd", "toto");
  n1.tree_dump(std::clog, "Actor 1: ");
  std::clog << std::endl;

  // Actor #2:
  std::clog << "\ntest_com_actor_2: Actor #2" << std::endl;
  vire::com::actor n2;
  n2.set_name("snemo.vire.client.78DKZHFDZF");
  n2.set_category(vire::com::actor::CATEGORY_CLIENT);
  n2.grab_metadata().store("rabbitmq.login", "bar");
  n2.grab_metadata().store("rabbitmq.passwd", "baz");
  n2.tree_dump(std::clog, "Actor 2: ");
  std::clog << std::endl;

  // Actor #3:
  std::clog << "\ntest_com_actor_3: Actor #3" << std::endl;
  vire::com::actor n3;
  n3.set_name("snemo.vire.subcontractor.cmslapp");
  n3.set_category(vire::com::actor::CATEGORY_SUBCONTRACTOR);
  n3.tree_dump(std::clog, "Actor 3: ");
  std::clog << std::endl;

  return;
}
