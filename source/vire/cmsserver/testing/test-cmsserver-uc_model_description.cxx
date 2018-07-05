// test-cmsserver-uc_model_description.cxx

// Ourselves:
#include <vire/cmsserver/uc_model_description.h>

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
#include <bayeux/datatools/io_factory.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/vire.h>

void test_uc_model_description_1();
void test_uc_model_description_2();
void test_uc_model_description_3();
void test_uc_model_description_4();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::uc_model_description' class."
              << std::endl;

    test_uc_model_description_1();
    test_uc_model_description_2();
    test_uc_model_description_3();
    test_uc_model_description_4();

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

void test_uc_model_description_1()
{
  std::clog << "\ntest_uc_model_description_1: basics" << std::endl;

  vire::cmsserver::uc_model_description ucDesc;
  ucDesc.set_name("FooLock");
  ucDesc.set_description("An expert session info record");
  ucDesc.set_use_case_type_id("vire::cmsserver::lock_use_case");
  ucDesc.set_use_case_config_path("@snemo:config/cms/server/use_cases/foo/foo_1.0.conf");
  ucDesc.configure();
  ucDesc.tree_dump(std::clog, "Use case model description: ");

  std::clog  << std::endl;
  return;
}

void test_uc_model_description_2()
{
  std::clog << "\ntest_uc_model_description_2: configuration" << std::endl;

  {
    datatools::properties ucCfg;
    ucCfg.store("name", "FooBot");
    ucCfg.store("description", "Foo use case");
    ucCfg.store("type_id", "snemo::cms::foo_use_case");
    ucCfg.store("config_path", "@snemo:config/cms/server/use_cases/foo/foo_1.0.conf");
    ucCfg.tree_dump(std::clog, "Use case model description configuration: ");
    std::clog  << std::endl;
    
    datatools::properties::config cfgWriter;
    cfgWriter.write(std::clog, ucCfg);
    std::clog  << std::endl;
   
    vire::cmsserver::uc_model_description ucDesc;
    ucDesc.configure(ucCfg);
    ucDesc.tree_dump(std::clog, "Use case model description: ");
    std::clog  << std::endl;
    
    {
      datatools::data_writer writer("test-cmsserver-umd.xml", datatools::using_multi_archives);
      writer.store(ucDesc);
    }
  }
  
  {
    vire::cmsserver::uc_model_description ucDesc;
    datatools::data_reader reader("test-cmsserver-umd.xml", datatools::using_multi_archives);
    reader.load(ucDesc);
    ucDesc.tree_dump(std::clog, "Use case model description (loaded): ");
    std::clog  << std::endl;
    
    datatools::properties ucDescCfg;
    ucDesc.export_to_config(ucDescCfg);
    datatools::properties::config cfgWriter;
    cfgWriter.write(std::clog, ucDescCfg);
  }
 
  std::clog << std::endl;
  return;
}

void test_uc_model_description_3()
{
  std::clog << "\ntest_uc_model_description_3: basics" << std::endl;

  datatools::properties ucModCfg;
  // Add fake configuration parameters:
  ucModCfg.store("name", "UniverseLock");
  ucModCfg.store("description", "Lock the full experiment");
  ucModCfg.store("type_id", "vire::cmsserver::lock_use_case");
  ucModCfg.store("config.mode", "expert");
  ucModCfg.store("config.log", "system");
    
  std::vector<std::string> daughter_names{{"M1","M2","M3","M4","S0", "S1"}};
  ucModCfg.store("composition.scheduling", "parallel");
  ucModCfg.store("composition.names", daughter_names);
  ucModCfg.store("composition.M1.model_id",    "snemo.monitoring.slow");
  ucModCfg.store("composition.M1.description", "Monitoring automaton #0");
  ucModCfg.store("composition.M2.model_id",    "snemo.monitoring.slow");
  ucModCfg.store("composition.M2.description", "Monitoring automaton #1");
  ucModCfg.store("composition.M3.model_id",    "snemo.monitoring.fast");
  ucModCfg.store("composition.M3.description", "Monitoring automaton #2");
  ucModCfg.store("composition.M4.model_id",    "snemo.monitoring.lazy");
  ucModCfg.store("composition.M4.description", "Monitoring automaton #2");
  ucModCfg.store("composition.S0.model_id",    "snemo.stat");
  ucModCfg.store("composition.S0.description", "Statistician automaton #0");
  ucModCfg.store("composition.S1.model_id",    "snemo.stat2");
  ucModCfg.store("composition.S1.description", "Statistician automaton #1");
  ucModCfg.store_flag("composition.locked");
  
  vire::cmsserver::uc_model_description ucModDesc;
  ucModDesc.configure(ucModCfg);
  ucModDesc.tree_dump(std::clog, "Use case model description: ");
    
  datatools::properties ucModDescCfg;
  ucModDesc.export_to_config(ucModDescCfg);
  datatools::properties::config cfgWriter;
  cfgWriter.write(std::clog, ucModDescCfg);

  std::clog  << std::endl;
  return;
}

void test_uc_model_description_4()
{
  std::clog << "\ntest_uc_model_description_4: basics" << std::endl;

  vire::cmsserver::uc_model_description ucDesc;
  ucDesc.set_name("FooLock");
  ucDesc.set_description("An expert session info record");
  ucDesc.set_use_case_type_id("vire::cmsserver::lock_use_case");
  ucDesc.set_use_case_config_path("@snemo:config/cms/server/use_cases/foo/foo_1.0.conf");
  ucDesc.grab_composition_description().set_scheduling(vire::running::SCHED_PARALLEL);
  ucDesc.grab_composition_description().add_daughter("M1", "snemo.monitoring.slow");
  ucDesc.grab_composition_description().add_daughter("M2", "snemo.monitoring.slow");
  ucDesc.grab_composition_description().add_daughter("M3", "snemo.monitoring.fast");
  ucDesc.grab_composition_description().add_daughter("M4", "snemo.monitoring.lazy");
  ucDesc.grab_composition_description().add_daughter("S0", "snemo.stat");
  ucDesc.configure();
  ucDesc.grab_composition_description().lock();
  ucDesc.tree_dump(std::clog, "Use case model description: ");

  std::clog  << std::endl;
  return;
}
