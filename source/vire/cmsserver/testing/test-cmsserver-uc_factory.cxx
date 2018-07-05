// test-cmsserver-uc_factory.cxx

// Ourselves:
#include <vire/cmsserver/uc_factory.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Boost:
// #include <boost/filesystem.hpp>

// This project:
#include <vire/vire.h>

void test_uc_factory_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::uc_factory' class."
              << std::endl;

    test_uc_factory_1();

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

void test_uc_factory_1()
{
  std::clog << "\ntest_uc_factory_1: basics" << std::endl;

  std::string ucFactoryCfgPath = "${VIRE_TESTING_TWO_DIR}/config/uc_factory.conf";
  datatools::fetch_path_with_env(ucFactoryCfgPath);
  datatools::properties ucFactoryCfg;
  ucFactoryCfg.read_configuration(ucFactoryCfgPath);

  datatools::service_dict_type services;
  
  vire::cmsserver::uc_factory ucFactory;
  ucFactory.set_logging_priority(datatools::logger::PRIO_DEBUG);
  ucFactory.initialize(ucFactoryCfg, services);
  ucFactory.print_tree(std::clog,
                       datatools::i_tree_dumpable::make_base_print_options("Use case factory: "));
  std::clog  << std::endl;


  std::shared_ptr<vire::cmsserver::base_use_case> uc = ucFactory.top_create_dry("Bot", "Process");
  uc->print_tree(std::clog,
                 datatools::i_tree_dumpable::make_base_print_options("Use case: "));

  ucFactory.print_tree(std::clog,
                       datatools::i_tree_dumpable::make_base_print_options("Use case factory: "));
  std::clog  << std::endl;
 
  ucFactory.reset();
  
  std::clog  << std::endl;
  return;
}
