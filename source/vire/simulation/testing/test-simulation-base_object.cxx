// test-simulation-base_object.cxx

// Ourselves:
#include <vire/simulation/base_object.h>

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
#include <vire/simulation/door.h>
#include <vire/simulation/switchable_device.h>
#include <vire/simulation/thermometer.h>

void test_base_object_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::simulation::base_object' class."
              << std::endl;

    test_base_object_1();

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

struct room_atmosphere_automaton
{
  room_atmosphere_automaton(vire::simulation::room & room_)
    : _room_(room_)
  {
  }
  void operator()()
  {
    double omega = 0.3 / CLHEP::second;
    while (_time_ < 20.0 * CLHEP::second) {
      int nmillis = 500;
      std::this_thread::sleep_for(std::chrono::milliseconds(nmillis));
      double new_room_temperature = _room_.get_temperature() + 3.0 * CLHEP::kelvin * std::cos(omega * _time_);
      double new_room_pressure = _room_.get_pressure() + 0.001 * CLHEP::bar * std::cos(omega * _time_);
      double new_room_humidity = _room_.get_humidity() + 0.5 * CLHEP::perCent * std::cos(omega * _time_);
      _room_.set_temperature(new_room_temperature);
      _room_.set_pressure(new_room_pressure);
      _room_.set_humidity(new_room_humidity);
      _time_ += nmillis * CLHEP::ms;
    }
  }
  double _time_ = 0.0;
  vire::simulation::room & _room_;
};

struct thermometer_automaton
{
  thermometer_automaton(vire::simulation::thermometer & thermometer_)
    : _thermometer_(thermometer_)
  {
  }
  void operator()()
  {
    double time = 0.0;
    while (time < 20.0 * CLHEP::second) {
      int nmillis = 1000;
      std::this_thread::sleep_for(std::chrono::milliseconds(nmillis));
      time += nmillis * CLHEP::ms;
      _thermometer_.update_measurement();
      double temperature = _thermometer_.get_last_measured_temperature();
      std::clog << "T = " << temperature / CLHEP::kelvin << " K" << std::endl;
    }
  }
  vire::simulation::thermometer & _thermometer_;
};

void test_base_object_1()
{
  std::clog << "\ntest_base_object_1: basics" << std::endl;

  using vire::simulation::base_object;
  using vire::simulation::room;
  using vire::simulation::door;
  using vire::simulation::switchable_device;
  using vire::simulation::thermometer;
  
  std::shared_ptr<base_object> house = std::make_shared<base_object>();
  house->set_name("house");
  house->set_display_name("House");
  house->set_terse_description("A dummy house");

  std::shared_ptr<room> hall = std::make_shared<room>();
  hall->set_name("hall");

  std::shared_ptr<switchable_device> hall_lighting = std::make_shared<switchable_device>();
  hall_lighting->set_name("hall_lighting");
  hall->add_child("lighting", hall_lighting);

  std::shared_ptr<room> kitchen = std::make_shared<room>();
  kitchen->set_name("kitchen");
   
  std::shared_ptr<switchable_device> kitchen_lighting = std::make_shared<switchable_device>();
  kitchen_lighting->set_name("kitchen_lighting");
  kitchen->add_child("lighting", kitchen_lighting);

  std::shared_ptr<room> bathroom = std::make_shared<room>();
  bathroom->set_name("bathroom");
  bathroom->set_floor_surface(10.0 * CLHEP::m2);
  bathroom->set_volume(25.0 * CLHEP::m3);
  bathroom->set_temperature(200.0 * CLHEP::kelvin);
  bathroom->set_pressure(1.0 * CLHEP::bar);
  bathroom->set_humidity(73.0 * CLHEP::perCent);

  std::shared_ptr<base_object> bathroom_bathtub = std::make_shared<base_object>();
  bathroom_bathtub->set_name("bathroom_bathtub");
  bathroom->add_child("bathtub", bathroom_bathtub);

  std::shared_ptr<base_object> bathroom_washbasin = std::make_shared<base_object>();
  bathroom_washbasin->set_name("bathroom_washbasin");
  bathroom->add_child("washbasin", bathroom_washbasin);

  std::shared_ptr<switchable_device> bathroom_lighting = std::make_shared<switchable_device>();
  bathroom_lighting->set_name("bathroom_lighting");
  bathroom->add_child("lighting", bathroom_lighting);

  std::shared_ptr<thermometer> bathroom_thermometer = std::make_shared<thermometer>();
  bathroom_thermometer->set_name("bathroom_thermometer");
  bathroom_thermometer->set_seed(12345);
  bathroom_thermometer->set(250.0 * CLHEP::kelvin, 340.0 * CLHEP::kelvin, 0.2 * CLHEP::kelvin);
  bathroom_thermometer->set_measured_object(*bathroom);
  bathroom->add_child("thermometer", bathroom_thermometer);

  std::shared_ptr<room> living_room = std::make_shared<room>();
  
  std::shared_ptr<switchable_device> living_tvset = std::make_shared<switchable_device>();
  living_tvset->set_name("living_tvset");
  living_room->add_child("tvset", living_tvset);
 
  std::shared_ptr<switchable_device> living_lighting = std::make_shared<switchable_device>();
  living_lighting->set_name("living_lighting");
  living_room->add_child("lighting", living_lighting);

  std::shared_ptr<room> bedroom1 = std::make_shared<room>();
   
  std::shared_ptr<switchable_device> bedroom1_ceiling_light = std::make_shared<switchable_device>();
  bedroom1_ceiling_light->set_name("bedroom1_ceiling_light");
  bedroom1->add_child("ceiling_light", bedroom1_ceiling_light);
  
  std::shared_ptr<switchable_device> bedroom1_bedside_lamp = std::make_shared<switchable_device>();
  bedroom1_bedside_lamp->set_name("bedroom1_bedside_lamp");
  bedroom1->add_child("bedside_lamp", bedroom1_bedside_lamp);

  std::shared_ptr<room> bedroom2 = std::make_shared<room>();
  
  std::shared_ptr<switchable_device> bedroom2_ceiling_light = std::make_shared<switchable_device>();
  bedroom2_ceiling_light->set_name("bedroom2_ceiling_light");
  bedroom2->add_child("ceiling_light", bedroom2_ceiling_light);

  std::shared_ptr<room> toilet = std::make_shared<room>();

  std::shared_ptr<room> corridor = std::make_shared<room>();

  house->add_child("hall", hall);
  house->add_child("kitchen", kitchen);
  house->add_child("bathroom", bathroom);
  house->add_child("bedroom1", bedroom1);
  house->add_child("bedroom2", bedroom2);
  house->add_child("living_room", living_room);
  house->add_child("toilet", toilet);
  house->add_child("corridor", corridor);

  std::shared_ptr<door> main_door = std::make_shared<door>();
  main_door->set_room_in(*hall);

  std::shared_ptr<door> corridor_door = std::make_shared<door>();
  corridor_door->set_room_in(*corridor);
  corridor_door->set_room_out(*hall);

  std::shared_ptr<door> toilet_door = std::make_shared<door>();
  toilet_door->set_room_in(*toilet);
  toilet_door->set_room_out(*corridor);

  std::shared_ptr<door> bathroom_door = std::make_shared<door>();
  bathroom_door->set_room_in(*bathroom);
  bathroom_door->set_room_out(*corridor);
 
  std::shared_ptr<door> kitchen_door = std::make_shared<door>();
  kitchen_door->set_room_in(*kitchen);
  kitchen_door->set_room_out(*hall);

  std::shared_ptr<door> bedroom1_door = std::make_shared<door>();
  bedroom1_door->set_room_in(*bedroom1);
  bedroom1_door->set_room_out(*corridor);

  std::shared_ptr<door> bedroom2_door = std::make_shared<door>();
  bedroom2_door->set_room_in(*bedroom2);
  bedroom2_door->set_room_out(*corridor);

  house->add_child("kitchen_door", kitchen_door);
  house->add_child("main_door", main_door);
  house->add_child("toilet_door", toilet_door);
  house->add_child("corridor_door", corridor_door);
  house->add_child("bedroom1_door", bedroom1_door);
  house->add_child("bedroom2_door", bedroom2_door);
  house->add_child("bathroom_door", bathroom_door);

  {
    boost::property_tree::ptree popts;
    popts.put("title", house->get_name() + " : ");
    house->print_tree(std::clog, popts);
  }

  {
    boost::property_tree::ptree popts;
    popts.put("title", bathroom->get_name() + " : ");
    bathroom->print_tree(std::clog, popts);
  }

  {
    boost::property_tree::ptree popts;
    popts.put("title", bathroom_thermometer->get_name() + " : ");
    bathroom_thermometer->print_tree(std::clog, popts);
  }

  bathroom->set_temperature(300 * CLHEP::kelvin);
  bathroom->set_pressure(1.013 * CLHEP::bar);
  bathroom->set_humidity(85.0 * CLHEP::perCent);

  room_atmosphere_automaton bathroomAtmBot(*bathroom);
  thermometer_automaton bathroomAtmSensorBot(*bathroom_thermometer);
  std::thread t1(bathroomAtmBot);
  std::thread t2(bathroomAtmSensorBot);
  t1.join();
  t2.join();
  // for (int i = 0; i < 10; i++) {
  //   bathroom_thermometer->update_measurement();
  //   double temperature = bathroom_thermometer->get_last_measured_temperature();
  //   std::clog << "T = " << temperature / CLHEP::kelvin << " K" << std::endl;
  // }
  
  std::clog  << std::endl;
  return;
}

