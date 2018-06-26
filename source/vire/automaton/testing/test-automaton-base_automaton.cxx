// test-automaton-base_automaton.cxx

// Ourselves:
#include <vire/automaton/base_automaton.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <thread>

// Third party:
// - Boost:
// #include <boost/filesystem.hpp>

// This project:
#include <vire/vire.h>

void test_base_automaton_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::automaton::base_automaton' class."
              << std::endl;

    test_base_automaton_1();

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

void test_base_automaton_1()
{
  std::clog << "\ntest_base_automaton_1: basics" << std::endl;

  using vire::automaton::base_automaton;
  using vire::time::system_time_point;

  system_time_point now = std::chrono::system_clock::now();
  datatools::logger::priority logging = datatools::logger::PRIO_FATAL;
  // logging = datatools::logger::PRIO_DEBUG;
  
  base_automaton homeBot;
  homeBot.set_name("Home");
  homeBot.set_display_name("CMS/Orleans home");
  homeBot.set_terse_description("The Orleans CMS home");
  homeBot.set_logging_priority(logging);
  homeBot.set_run_tick(std::chrono::milliseconds(500));
  homeBot.set_run_duration(std::chrono::seconds(12));

  std::shared_ptr<base_automaton> hallBot = std::make_shared<base_automaton>();
  hallBot->set_name("Hall");
  hallBot->initialize_simple();
  std::clog << hallBot->get_name() << ":\n";
  hallBot->print_tree(std::clog);

  std::shared_ptr<base_automaton> kitchenBot = std::make_shared<base_automaton>();
  kitchenBot->set_name("Kitchen");
  kitchenBot->initialize_simple();
  std::clog << kitchenBot->get_name() << ":\n";
  kitchenBot->print_tree(std::clog);
  
  std::shared_ptr<base_automaton> bathroomBot = std::make_shared<base_automaton>();
  bathroomBot->set_name("Bathroom");
  bathroomBot->initialize_simple();
  std::clog << bathroomBot->get_name() << ":\n";
  bathroomBot->print_tree(std::clog);
  
  homeBot.add_child(hallBot);
  homeBot.add_child(kitchenBot);
  homeBot.add_child(bathroomBot);
  homeBot.initialize_simple();
  std::clog << homeBot.get_name() << ":\n";
  homeBot.print_tree(std::clog);
  std::clog << "\n";

  base_automaton homeBot2;
  homeBot2.set_name("Home2");
  homeBot2.set_display_name("CMS/Orleans home 2");
  homeBot2.set_terse_description("The Orleans CMS home 2");
  homeBot2.set_logging_priority(logging);
  homeBot2.set_run_tick(std::chrono::milliseconds(300));
  homeBot2.set_run_start_time(now + std::chrono::seconds(2));
  homeBot2.set_run_duration(std::chrono::seconds(5));
  homeBot2.initialize_simple();
  std::clog << homeBot2.get_name() << ":\n";
  homeBot2.print_tree(std::clog);
  std::clog << "\n";
 
  std::thread worker(&base_automaton::run, std::ref(homeBot));
  std::thread worker2(&base_automaton::run, std::ref(homeBot2));
  worker.join();
  worker2.join();
  std::clog << "Home bot status :\n";
  homeBot.get_control().print();
  std::clog << "\n";
  
  std::clog << "Home bot 2 status :\n";
  homeBot2.get_control().print();
  std::clog << "\n";
  
  std::clog  << std::endl;
  return;
}

