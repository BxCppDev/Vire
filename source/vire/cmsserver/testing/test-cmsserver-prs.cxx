// test-cmsserver-prs.cxx

// Ourselves:
#include <vire/cmsserver/parametrized_resource_specifications.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Boost:
#include <boost/filesystem.hpp>

// This project:
#include <vire/vire.h>

void test_prs_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::parametrized_resource_specifications' class."
              << std::endl;

    test_prs_1();

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

void test_prs_1()
{
  std::clog << "\ntest_prs_1: basics" << std::endl;
  vire::cmsserver::parametrized_resource_specifications prs;
  prs.add_device("bob", "HV.PowerSupply.Mod2817A");
  prs.add_device("riri", "Probe42");
  prs.add_resource("t1", "unit_dimension=temperature");
  prs.add_resource("p1", "unit_dimension=pressure");
  prs.add_resource("p2", "unit_dimension=pressure");
  
  boost::property_tree::ptree options;
  options.put(datatools::i_tree_dumpable::base_print_options::title_key(), "Specifications: ");
  options.put(datatools::i_tree_dumpable::base_print_options::indent_key(), ">>> ");

  prs.print_tree(std::clog, options);
  std::clog << std::endl;
  return;
}
