//! \file cms/testing/test-cms-experiment_image_registry.cxx
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of Vire.
//
// Vire is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Vire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Standard library:
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// Third party:
// - Qt:
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
// - Bayeux/datatools:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/service_manager.h>

// This project:
#include <vire/vire.h>
#include <vire/cms/experiment_image_registry.h>
#include <vire/cms/resource_image.h>
#include <vire/resource/manager.h>
#include <vire/cms/ui/image_panel.h>
#include <vire/cms/ui/experiment_image_registry_panel.h>

#include "../../resource/testing/snemo_tools.h"
#include "image_status_runner.h"

struct params_type {
  bool gui = false;
};

void test_eir_1(const params_type &);

int main( int argc_, char * argv_[])
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {

    params_type params;
    
    int iarg = 1;
    while (iarg < argc_) {
      std::string arg = argv_[iarg];
      if (arg == "--gui") {
        params.gui = true;
      }
      iarg++;
    }

    test_eir_1(params);
    
  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return error_code;
}

void test_eir_1(const params_type & params_)
{
  std::clog << "test_eir_1: basics..." << std::endl;

  datatools::service_manager services;
  vire::resource::testing::snemo::populate_service_manager(services);

  const vire::resource::manager & resources = services.get<vire::resource::manager>("resources");
  
  
  vire::cms::experiment_image_registry expImage;

  {
    datatools::properties expImageConfig;
    std::string expImageConfigPath = "${VIRE_TESTING_TWO_DIR}/config/snemo_eir.conf";
    datatools::fetch_path_with_env(expImageConfigPath);
    expImageConfig.read_configuration(expImageConfigPath);            
    expImage.initialize(expImageConfig,
                        const_cast<datatools::service_dict_type&>(services.get_local_services()));

    std::set<int32_t> resource_ids;
    std::string path_regexp("SuperNEMO:/Demonstrator/CMS/Calorimeter/HV/PS_1/board_9/channel_19/(.*)|SuperNEMO:/Demonstrator/CMS/Coil/(.*)");
    resources.build_set_of_resource_ids_from_path_regexp(path_regexp, resource_ids);

    std::vector<vire::cms::testing::image_status_runner> runners;    
    std::vector<std::thread> threads;    
    for (auto id : resource_ids) {
      const std::string & resPath = resources.get_resource_by_id(id).get_path();
      expImage.add_resource_image(resPath);

      vire::cms::resource_image & resImg = expImage.grab_resource_image(resPath);
      long seed = 314159 + runners.size();
      runners.push_back(vire::cms::testing::image_status_runner(resImg.grab_monitoring().grab_status(), seed));
      threads.push_back(std::thread(runners.back()));
    }
        
    if (params_.gui) {
      int argc = 1;
      const char * argv[] = { "test-cms-experiment_image_registry" };
      QApplication app(argc, (char **) argv);

      QWidget window;
      using vire::cms::ui::image_panel;

      // QVBoxLayout * layout = new QVBoxLayout;
      // image_panel * imgPanel = new image_panel;
      // const vire::cms::resource_image & resImg
      //   = expImage.get_resource_image("SuperNEMO:/Demonstrator/CMS/Calorimeter/HV/PS_1/board_9/channel_19/TripExt/__dp_read__");
      // imgPanel->set_image(resImg);
      // layout->addWidget(imgPanel);
      
      auto images = expImage.get_images();
      int ncols = 2;
      int nrows = images.size()/2;
      int imgCounter = 0;
      QGridLayout * layout = new QGridLayout;
      for (const auto & img : images) {
        image_panel * imgPanel = new image_panel;
        imgPanel->set_no_id(true);
        imgPanel->set_no_labels(true);
        imgPanel->set_no_value(true);
        imgPanel->set_image(*img.second.get());
        int row = imgCounter/2;
        int col = imgCounter%2;
        QGroupBox * box = new QGroupBox;
        box->setTitle(img.second->get_path().c_str());
        box->setFlat(true);
        QVBoxLayout * boxLayout = new QVBoxLayout;
        boxLayout->addWidget(imgPanel);
        box->setLayout(boxLayout);
        // layout->addWidget(imgPanel, row, col);
        layout->addWidget(box, row, col);
        imgCounter++;
        if (row == 5) break;
      }

      window.setLayout(layout);

      // QWidget window0;
      // using vire::cms::ui::experiment_image_registry_panel;
      // experiment_image_registry_panel * regPanel = new experiment_image_registry_panel;
      // regPanel->set_registry(expImage);
      // QVBoxLayout * layout0 = new QVBoxLayout;
      // layout0->addWidget(regPanel);
      // window0.setLayout(layout0);
      // window0.show();

      window.show();
      app.exec();
    }
    
    for (auto & t : threads) {
      t.join();
    }    
 
    std::clog << "SuperNEMO experiment mock image registry:" << std::endl;
    expImage.print_tree(std::clog);
    std::clog << std::endl;
  }
    
  std::clog << std::endl;
  return;
}
