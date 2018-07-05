// test-cmsserver-uc_composition_description.cxx

// Ourselves:
#include <vire/cmsserver/uc_composition_description.h>

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

void test_uc_composition_description_1();
void test_uc_composition_description_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::uc_composition_description' class."
              << std::endl;

    test_uc_composition_description_1();
    test_uc_composition_description_2();

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

void test_uc_composition_description_1()
{
  std::clog << "\ntest_uc_composition_description_1: basics" << std::endl;

  vire::cmsserver::uc_composition_description ucCompDesc;
  ucCompDesc.set_scheduling(vire::running::SCHED_PARALLEL);
  ucCompDesc.add_daughter("Monitor1", "MonitoringModel1", "Monitoring use case model #1");
  ucCompDesc.add_daughter("Monitor2", "MonitoringModel2", "Monitoring use case model #2");
  ucCompDesc.add_daughter("Controler1", "ControlModel1", "Control use case model #1");
  ucCompDesc.lock();
  ucCompDesc.tree_dump(std::clog, "Use case composition description: ");
  std::clog  << std::endl;
  try {
    ucCompDesc.add_daughter("Controler2", "ControlModelX", "Another use case model");
  } catch (std::exception & x) {
    std::clog << "info: As expected: " << x.what() << std::endl;
  }
  std::clog  << std::endl;

  datatools::properties ucCompDescCfg;
  ucCompDesc.export_to_config(ucCompDescCfg);
  datatools::properties::config cfgWriter;
  cfgWriter.write(std::clog, ucCompDescCfg);

  std::clog  << std::endl;
  return;
}

void test_uc_composition_description_2()
{
  std::clog << "\ntest_uc_composition_description_2: basics" << std::endl;

  {
    vire::cmsserver::uc_composition_description ucCompDesc;
    ucCompDesc.set_scheduling(vire::running::SCHED_SERIAL);
    ucCompDesc.add_daughter("Monitor1", "MonitoringModel1", "Monitoring use case model #1");
    ucCompDesc.add_daughter("Monitor2", "MonitoringModel2", "Monitoring use case model #2");
    ucCompDesc.add_daughter("Controler1", "ControlModel1", "Control use case model #1");
    ucCompDesc.lock();
    ucCompDesc.tree_dump(std::clog, "Use case composition description: ");
    std::clog  << std::endl;
    
    {
      datatools::data_writer writer("test-cmsserver-ucd.xml", datatools::using_multi_archives);
      writer.store(ucCompDesc);
    }
  }
  
  {
    vire::cmsserver::uc_composition_description ucCompDesc;
    datatools::data_reader reader("test-cmsserver-ucd.xml", datatools::using_multi_archives);
    reader.load(ucCompDesc);
    ucCompDesc.tree_dump(std::clog, "Use case composition description (loaded): ");
    std::clog  << std::endl;
  }
  
  std::clog  << std::endl;
  return;
}

