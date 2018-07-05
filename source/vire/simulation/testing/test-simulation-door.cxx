// test-simulation-door.cxx

// Ourselves:
#include <vire/simulation/door.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <thread>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/vire.h>
#include <vire/simulation/room.h>

void test_door_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::simulation::door' class."
              << std::endl;

    test_door_1();

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

void test_door_1()
{
  std::clog << "\ntest_door_1: basics" << std::endl;

  using vire::simulation::door;
  using vire::simulation::room;

  room room1;
  room1.set_name("room1");
  room1.set_display_name("Room #1");
  room1.set_volume(30 * CLHEP::m3);
  room1.print_tree(std::clog, datatools::i_tree_dumpable::make_base_print_options(room1.get_display_name()));
  
  room room2;
  room2.set_name("room2");
  room2.set_display_name("Room #2");
  room2.set_volume(40 * CLHEP::m3);
  room2.print_tree(std::clog, datatools::i_tree_dumpable::make_base_print_options(room2.get_display_name()));
   
  door d;
  d.set_name("myDoor");
  d.set_display_name("My door");
  d.set_terse_description("A dummy door");
  d.set_room_in(room1);
  d.set_room_out(room2);
  {
    boost::property_tree::ptree popts;
    popts.put("title", "Door: ");
    d.print_tree(std::clog, popts);
  }
  
  std::clog  << std::endl;
  return;
}

