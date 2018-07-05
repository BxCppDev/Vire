// test-cmsserver-uc_mounting_table.cxx

// Ourselves:
#include <vire/cmsserver/uc_mounting_table.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/vire.h>

void test_mt_1();
void test_mt_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::uc_mounting_table' class."
              << std::endl;

    test_mt_1();
    test_mt_2();

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

void test_mt_1()
{
  std::clog << "\ntest_mt_1: basics" << std::endl;
  vire::cmsserver::uc_resource_mounting_table rmt1;
  rmt1.add("CaloHVMonitor1", "HVPS", "Calorimeter",  "HV/PS_0");
  rmt1.add("CaloHVMonitor1", "AtmCond", "Atm", "Monitor/Measurements");
  rmt1.add("CaloHVMonitor2", "HVPS", "Calorimeter",  "HV/PS_1");

  {
    boost::property_tree::ptree options;
    options.put(datatools::i_tree_dumpable::base_print_options::title_key(), "Mounting table: ");
    options.put(datatools::i_tree_dumpable::base_print_options::indent_key(), ">>> ");
    rmt1.print_tree(std::clog, options);
  }
  std::clog << std::endl;

  return;
}

void test_mt_2()
{
  std::clog << "\ntest_mt_2: config" << std::endl;

  std::string rmtConfPath = "${VIRE_TESTING_TWO_DIR}/config/uc_mtab.conf";
  datatools::fetch_path_with_env(rmtConfPath);
  datatools::properties rmtConf;
  rmtConf.read_configuration(rmtConfPath);

  vire::cmsserver::uc_resource_mounting_table rmt;
  rmt.configure(rmtConf);
  {
    boost::property_tree::ptree options;
    options.put(datatools::i_tree_dumpable::base_print_options::title_key(), "Mounting table: ");
    options.put(datatools::i_tree_dumpable::base_print_options::indent_key(), ">>> ");
    rmt.print_tree(std::clog, options);
  }
  std::clog << std::endl;

  return;
}
