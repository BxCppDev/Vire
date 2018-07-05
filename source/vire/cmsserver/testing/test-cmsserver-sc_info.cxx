// test-cmsserver-sc_info.cxx

// Ourselves:
#include <vire/cmsserver/sc_info.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <thread>
#include <random>
#include <chrono>

// Third party:
// - Qt:
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>

// This project:
#include <vire/vire.h>
#include <vire/cmsserver/ui/sc_info_panel.h>

#include "tools.h"
#include "sc_info_runner.h"
#include "../../cms/testing/image_status_runner.h"

struct params
{
  bool gui = false;
};

void test_sc_info_1(params);

int main(int argc_, char ** argv_)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::sc_info' class."
              << std::endl;
    params the_params;
    if (argc_ > 1 && std::string(argv_[1]) == "--gui") {
      the_params.gui = true;
    }

    test_sc_info_1(the_params);

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

void test_sc_info_1(params params_)
{
    bool gui = params_.gui;

  std::clog << "\ntest_sc_info_1: basics" << std::endl;

  vire::device::manager deviceMgr;
  vire::sandbox::init_devices(deviceMgr);

  vire::resource::manager resourceMgr;
  vire::sandbox::init_resources(resourceMgr, deviceMgr);
   
  vire::cmsserver::sc_info scInfo;
  scInfo.set_name("orleans");
  scInfo.set_display_name("Orleans");
  scInfo.set_terse_description("Orleans CMS subcontractor interface");
  scInfo.set_device_manager(deviceMgr);
  scInfo.set_resource_manager(resourceMgr);
  scInfo.add_mounted_device("SuperNEMO:/Demonstrator/CMS/Coil/PS");
  scInfo.add_mounted_device("SuperNEMO:/Demonstrator/CMS/Calorimeter/HV/PS_0");
  scInfo.add_mounted_device("SuperNEMO:/Demonstrator/CMS/Calorimeter/HV/PS_1");
  scInfo.initialize_simple();
  scInfo.print_tree(std::clog, datatools::i_tree_dumpable::make_base_print_options("Subcontractor:"));
  std::clog << std::endl;

  vire::cmsserver::testing::sc_info_runner scRunner(scInfo);
  
  vire::cms::image_status & sc1
    = scInfo.grab_mounted_device_info("SuperNEMO:/Demonstrator/CMS/Coil/PS").status;
  vire::cms::testing::image_status_runner stRunner1(sc1, 123);

  vire::cms::image_status & sc2
    = scInfo.grab_mounted_device_info("SuperNEMO:/Demonstrator/CMS/Calorimeter/HV/PS_0").status;
  vire::cms::testing::image_status_runner stRunner2(sc2, 456);

  vire::cms::image_status & sc3
    = scInfo.grab_mounted_device_info("SuperNEMO:/Demonstrator/CMS/Calorimeter/HV/PS_1").status;
  vire::cms::testing::image_status_runner stRunner3(sc3, 789);

  std::thread t1(scRunner);
  std::thread t2(stRunner1);
  std::thread t3(stRunner2);
  std::thread t4(stRunner3);

  if (gui) {
    int argc = 1;
    const char * argv[] = { "test-cmsserver-sc_info" };
    QApplication app(argc, (char **) argv);

    using vire::cmsserver::ui::sc_info_panel;
    sc_info_panel * scInfoPanel = new sc_info_panel;
    scInfoPanel->set_subcontractor_info(scInfo);

    QWidget window;
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(scInfoPanel);
    window.setLayout(layout);
    window.show();
    app.exec();
  }
  t1.join();
  t2.join();
  t3.join();
  t4.join();

  std::clog << std::endl;
  return;
}
