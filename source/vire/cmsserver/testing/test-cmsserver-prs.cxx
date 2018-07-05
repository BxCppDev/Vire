// test-cmsserver-prs.cxx

// Ourselves:
#include <vire/cmsserver/uc_parametrised_resource_specifications.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Boost:
#include <boost/filesystem.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/vire.h>

void test_prs_1();
void test_prs_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::uc_parametrised_resource_specifications' class."
              << std::endl;

    test_prs_1();
    test_prs_2();

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
  vire::cmsserver::uc_parametrised_resource_specifications prs;
  prs.add_device("PS",
                 "HV.PowerSupply.Mod2817A",
                 vire::device::TYPE_CRATE);
  prs.add_device("Probe",
                 "Probe42-3Ze");
  prs.add_device("Switch",
                 "Sw342");
  prs.add_resource("T1",
                   vire::resource::RESOURCE_CATEGORY_DATAPOINT,
                   vire::resource::ACCESS_READ_ONLY,
                   datatools::introspection::DATA_TYPE_DOUBLE,
                   "temperature");
  prs.add_resource("P1",
                   vire::resource::RESOURCE_CATEGORY_DATAPOINT,
                   vire::resource::ACCESS_READ_ONLY,
                   datatools::introspection::DATA_TYPE_DOUBLE,
                   "pressure");
  prs.add_resource("P2",
                   vire::resource::RESOURCE_CATEGORY_DATAPOINT,
                   vire::resource::ACCESS_READ_ONLY,
                   datatools::introspection::DATA_TYPE_DOUBLE,
                   "pressure");
  prs.lock();
  
  boost::property_tree::ptree options;
  options.put(datatools::i_tree_dumpable::base_print_options::title_key(), "Specifications: ");
  options.put(datatools::i_tree_dumpable::base_print_options::indent_key(), ">>> ");
  prs.print_tree(std::clog, options);
  std::clog << std::endl;

  vire::cmsserver::uc_parametrised_resource_specifications prs2;
  prs2.print_tree(std::clog, options);
  std::clog << std::endl;

  return;
}

void test_prs_2()
{
  std::clog << "\ntest_prs_2: configuration" << std::endl;
  vire::cmsserver::uc_parametrised_resource_specifications prs;

  std::string prsCfgPath = "${VIRE_TESTING_TWO_DIR}/config/prs.conf";
  datatools::fetch_path_with_env(prsCfgPath);
  datatools::properties prsCfg;
  prsCfg.read_configuration(prsCfgPath);
  {
    // boost::property_tree::ptree options;
    // options.put(datatools::i_tree_dumpable::base_print_options::title_key(), "Configuration: ");
    // prsCfg.print_tree(std::clog, options);
    prsCfg.tree_dump(std::clog, "Configuration:");
  }
  prs.load(prsCfg);
  {
    boost::property_tree::ptree options;
    options.put(datatools::i_tree_dumpable::base_print_options::title_key(), "Specifications: ");
    options.put(datatools::i_tree_dumpable::base_print_options::indent_key(), ">>> ");
    prs.print_tree(std::clog, options);
  }
  std::clog << std::endl;
  
  return;
}
