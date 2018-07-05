// test-cmsserver-uc_model_db.cxx

// Ourselves:
#include <vire/cmsserver/uc_model_db.h>

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

void test_uc_model_db_1();
void test_uc_model_db_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::uc_model_db' class."
              << std::endl;

    test_uc_model_db_1();
    test_uc_model_db_2();

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

void test_uc_model_db_1()
{
  std::clog << "\ntest_uc_model_db_1: basics" << std::endl;

  vire::cmsserver::uc_model_db ucModelDb;
  ucModelDb.add_supported_type_id("vire::cmsserver::temperature_monitoring_use_case");
  ucModelDb.add_supported_type_id("vire::cmsserver::base_use_case");
  ucModelDb.add_supported_type_id("vire::cmsserver::lock_use_case");

  std::string model_defs_path = "${VIRE_TESTING_TWO_DIR}/config/uc_models.defs";
  datatools::fetch_path_with_env(model_defs_path);
  datatools::multi_properties model_defs;
  model_defs.read(model_defs_path);
  ucModelDb.load(model_defs);
  ucModelDb.print_tree(std::clog);

  if (ucModelDb.has_cyclic_dependencies()) {
    std::clog << "WARNING ! Cyclic dependencies has been detected!" << std::endl;
    for (auto modelName : ucModelDb.get_cyclic_dependencies()) {
      std::clog << "\tModel '" << modelName << "' belongs to a dependency cycle." << std::endl; 
    }
  }
  
  if (ucModelDb.has_unsatisfied_dependencies()) {
    std::clog << "WARNING ! Unsatisfied dependencies has been detected!" << std::endl;
    for (auto modelName : ucModelDb.get_unsatisfied_dependencies()) {
      std::clog << "\tModel '" << modelName << "' is missing." << std::endl; 
    }
  }

  std::clog  << std::endl;
  return;
}

void test_uc_model_db_2()
{
  std::clog << "\ntest_uc_model_db_2: config..." << std::endl;
  vire::cmsserver::uc_model_db ucModelDb;

  std::string ucModelDbCfgPath = "${VIRE_TESTING_TWO_DIR}/config/uc_models_db.conf";
  datatools::fetch_path_with_env(ucModelDbCfgPath);
  datatools::properties ucModelDbCfg;
  ucModelDbCfg.read_configuration(ucModelDbCfgPath);

  ucModelDb.configure(ucModelDbCfg);
  ucModelDb.print_tree(std::clog);
  
  std::clog  << std::endl;
  return;
}
