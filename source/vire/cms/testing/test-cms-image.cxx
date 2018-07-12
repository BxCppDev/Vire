//! \file cms/testing/test-cms-image.cxx
//
// Copyright (c) 2016-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <QVBoxLayout>
// - Bayeux/datatools
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/datatools/introspection/method.h>
#include <bayeux/datatools/introspection/data_description.h>

// This project:
#include <vire/cms/base_image.h>
#include <vire/cms/resource_image.h>
#include <vire/cms/device_image.h>
#include <vire/device/logical_device.h>
#include <vire/device/base_method_port_model.h>
#include <vire/utility/path.h>
#include <vire/resource/resource.h>
#include <vire/resource/method_resource_instance.h>
#include <vire/device/base_datapoint_model.h>
#include <vire/resource/datapoint_resource_instance.h>
#include <vire/cms/ui/image_panel.h>

#include "image_status_runner.h"

struct params_type {
  bool gui = false;
};

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

    {
      vire::cms::base_image img;
      img.set_metadata("vire.cms.subcontractor", "cmslapp");
      img.set_metadata("value.type", "real");
      img.set_metadata("value.real.resolution","5 ms");
      img.set_metadata("value.unit_dimension", "time");
      img.set_metadata("value.preferred_unit", "millisecond");
      img.set_metadata("foo", "bar");
      img.grab_status().set_timestamp(vire::time::now_utc());
      img.grab_status().set_disabled(true);
      img.grab_monitoring().set_real_value(3.14 * CLHEP::second);
      img.print_tree(std::clog,
                     datatools::i_tree_dumpable::make_base_print_options("Dummy image:"));
      std::clog << std::endl;
    }
    
    {
      // Description of a method:
      std::shared_ptr<datatools::introspection::method> daqStartMethodPtr
        = std::make_shared<datatools::introspection::method>();
      daqStartMethodPtr->set_name("start_daq");
      daqStartMethodPtr->set_display_name("Start DAQ");
      daqStartMethodPtr->set_constness(false);
      daqStartMethodPtr->initialize();
      daqStartMethodPtr->tree_dump(std::clog, "Description of a method : ");
      std::clog << std::endl;

      // Model of the method:
      vire::device::base_method_port_model daqStartMethodModel;
      daqStartMethodModel.set_name("daq.start.method.model");
      daqStartMethodModel.set_type(vire::device::PORT_METHOD);
      daqStartMethodModel.set_io_type(vire::device::PORT_IO_GENERIC);
      daqStartMethodModel.set_rw_access(vire::utility::RW_WRITABLE);
      daqStartMethodModel.set_method(daqStartMethodPtr);
      daqStartMethodModel.tree_dump(std::clog, "Method port model: ");
      std::clog << std::endl;
      
      // Method resource instance:
      std::shared_ptr<vire::resource::method_resource_instance> methResInstPtr
        = std::make_shared<vire::resource::method_resource_instance>();
      methResInstPtr->set_name("daqStart");
      methResInstPtr->set_model(daqStartMethodModel);
      methResInstPtr->tree_dump(std::clog, "Method resource instance: ");
      std::clog << std::endl;

      // Registered unique resource:
      vire::resource::resource startDaqRes;
      std::string rpath = "SuperNEMO:/Demonstrator/CMS/DAQ/start";
      std::string rname = vire::utility::path::to_address(rpath);
      startDaqRes.set_name(rname);
      startDaqRes.set_display_name("SuperNEMO DAQ start");
      startDaqRes.set_id(1000);
      startDaqRes.set_path(rpath);
      startDaqRes.set_access(vire::utility::RW_WRITABLE);
      startDaqRes.set_max_number_of_tokens(1);
      startDaqRes.set_resource_instance(methResInstPtr);
      startDaqRes.initialize_simple();
      startDaqRes.tree_dump(std::clog, "Resource '" + startDaqRes.get_display_name() + "' : ");
      std::clog << std::endl;
      
      vire::cms::resource_image startDaqResImg(startDaqRes);
      startDaqResImg.set_metadata("vire.cms.subcontractor", "cmslapp");
      startDaqResImg.grab_status().set_timestamp(vire::time::now_utc());
      startDaqResImg.grab_status().set_missing(false);
      startDaqResImg.grab_status().set_disabled(true);
      startDaqResImg.grab_status().set_pending(false);
      startDaqResImg.grab_status().set_failed(false);
      startDaqResImg.print_tree(std::clog,
                                datatools::i_tree_dumpable::make_base_print_options("Start DAQ resource image:"));
      std::clog << std::endl;

      // Registered uniqued device:
      vire::device::logical_device daqDev;
      std::string devPath = "SuperNEMO:/Demonstrator/CMS/DAQ";
      std::string devName = vire::utility::path::to_address(devPath);
      daqDev.set_name(devName);
      daqDev.set_display_name("SuperNEMO DAQ");
      daqDev.initialize();
      daqDev.print_tree(std::clog,
                        datatools::i_tree_dumpable::make_base_print_options("Device '" + daqDev.get_display_name() + "' : "));
      std::clog << std::endl;
  
      vire::cms::device_image daqDevImg(devPath, daqDev);
      daqDevImg.set_metadata("vire.cms.subcontractor", "cmslapp");
      // Forbidden: daqDevImg.grab_monitoring().set_real_value(1234 * CLHEP::volt);
      daqDevImg.grab_status().set_timestamp(vire::time::now_utc());
      daqDevImg.grab_status().set_missing(false);
      daqDevImg.grab_status().set_disabled(false);
      daqDevImg.grab_status().set_pending(false);
      daqDevImg.grab_status().set_failed(false);
      daqDevImg.print_tree(std::clog,
                           datatools::i_tree_dumpable::make_base_print_options("DAQ device image:"));
      std::clog << std::endl;
    }

    {

      datatools::introspection::data_description daqEventRateDataDesc;
      daqEventRateDataDesc.set_type(datatools::introspection::DATA_TYPE_DOUBLE);
      daqEventRateDataDesc.set_layout(datatools::introspection::DATA_LAYOUT_SCALAR);
      datatools::introspection::unit_info daqEventRateUnitInfo(datatools::introspection::UNIT_SUPPORT_IMPLICIT_UNIT,
                                                               "Hz");
      daqEventRateDataDesc.set_unit_info(daqEventRateUnitInfo);
      daqEventRateDataDesc.initialize();
      daqEventRateDataDesc.tree_dump(std::clog, "DAQ event rate data description:");
      std::clog << std::endl;

      // Model of the datapoint:
      vire::device::base_datapoint_model daqEventRateDpModel;
      daqEventRateDpModel.set_name("daq.event_rate.dp.model");
      daqEventRateDpModel.set_model_key(42);
      daqEventRateDpModel.set_virtual(false);
      daqEventRateDpModel.set_allow_embedded_devices(false);
      daqEventRateDpModel.set_rw_access(vire::utility::RW_READABLE);
      daqEventRateDpModel.set_data_description(daqEventRateDataDesc);
      daqEventRateDpModel.set_standard_interface(true);
      daqEventRateDpModel.initialize_simple();
      daqEventRateDpModel.tree_dump(std::clog, "DAQ event rate datapoint model: ");
      std::clog << std::endl;

      // Datapoint resource instance:
      std::shared_ptr<vire::resource::datapoint_resource_instance> daqEventRateDpInstPtr
        = std::make_shared<vire::resource::datapoint_resource_instance>();
      daqEventRateDpInstPtr->set_name("daqEventRate");
      daqEventRateDpInstPtr->set_model(daqEventRateDpModel);
      daqEventRateDpInstPtr->print_tree(std::clog,
                                        datatools::i_tree_dumpable::make_base_print_options("Datapoint resource instance:"));
      std::clog << std::endl;

      // Registered uniqued device:
      vire::device::logical_device daqEventRateDev;
      std::string daqEventRateDpDevPath = "SuperNEMO:/Demonstrator/CMS/DAQ/EventRate";
      std::string daqEventRateDpDevName = vire::utility::path::to_address(daqEventRateDpDevPath) + ".log";
      daqEventRateDev.set_name(daqEventRateDpDevName);
      daqEventRateDev.set_display_name("SuperNEMO DAQ Event Rate Datapoint");
      daqEventRateDev.initialize();
      daqEventRateDev.print_tree(std::clog,
                                 datatools::i_tree_dumpable::make_base_print_options("Logical datapoint device '" + daqEventRateDev.get_display_name() + "' : "));
      std::clog << std::endl;
  
      vire::cms::device_image daqEventRateDpDevImg(daqEventRateDpDevPath, daqEventRateDev);
      daqEventRateDpDevImg.print_tree(std::clog,
                                      datatools::i_tree_dumpable::make_base_print_options("DAQ event rate datapoint device image:"));
      std::clog << std::endl;

      
      // Method resource instance:
      std::shared_ptr<vire::resource::method_resource_instance> methReadDaqEventRateResInstPtr
        = std::make_shared<vire::resource::method_resource_instance>();
      methReadDaqEventRateResInstPtr->set_name("daqEventRate");
      methReadDaqEventRateResInstPtr->set_model(*(daqEventRateDpModel.get_standard_interface_method_port_models().find("__dp_read__.model")->second));
      methReadDaqEventRateResInstPtr->tree_dump(std::clog, "Method resource instance: ");
      std::clog << std::endl;

      // Registered unique resource:
      vire::resource::resource readDaqEventRateRes;
      std::string readDaqEventRateResPath = daqEventRateDpDevPath + "/__dp_read__";
      std::string readDaqEventRateResName = vire::utility::path::to_address(readDaqEventRateResPath);
      readDaqEventRateRes.set_name(readDaqEventRateResName);
      readDaqEventRateRes.set_display_name("SuperNEMO DAQ event rate");
      readDaqEventRateRes.set_id(1100);
      readDaqEventRateRes.set_path(readDaqEventRateResPath);
      readDaqEventRateRes.set_access(vire::utility::RW_READABLE);
      readDaqEventRateRes.set_resource_instance(methReadDaqEventRateResInstPtr);
      readDaqEventRateRes.initialize_simple();
      readDaqEventRateRes.tree_dump(std::clog, "Resource '" + readDaqEventRateRes.get_display_name() + "' : ");
      std::clog << std::endl;
    
      vire::cms::resource_image readDaqEventRateResImg(readDaqEventRateRes);
      readDaqEventRateResImg.set_metadata("vire.cms.subcontractor", "cmslapp");
      readDaqEventRateResImg.grab_status().set_timestamp(vire::time::now_utc());
      readDaqEventRateResImg.grab_status().set_missing(false);
      readDaqEventRateResImg.grab_status().set_disabled(false);
      readDaqEventRateResImg.grab_status().set_pending(false);
      readDaqEventRateResImg.grab_status().set_failed(false);
      readDaqEventRateResImg.set_metadata("value.type", "real");
      readDaqEventRateResImg.set_metadata("value.unit_dimension", "frequency");
      readDaqEventRateResImg.set_metadata("value.preferred_unit", "Hz");
      readDaqEventRateResImg.set_metadata("value.real.resolution", "0.01 Hz");
      readDaqEventRateResImg.grab_monitoring().set_real_value(234.5 * CLHEP::hertz);
      readDaqEventRateResImg.print_tree(std::clog,
                                        datatools::i_tree_dumpable::make_base_print_options("DAQ event rate read resource image:"));
      std::clog << std::endl;
   
      vire::cms::testing::image_status_runner stRunner(readDaqEventRateResImg.grab_monitoring().grab_status());
      std::thread t1(stRunner);
       
      if (params.gui) {
        int argc = 1;
        const char * argv[] = { "test-cms-image_status" };
        QApplication app(argc, (char **) argv);

        using vire::cms::ui::image_panel;
        image_panel * imgPanel = new image_panel;
        // imgPanel->set_no_labels(true);
        // imgPanel->set_no_id(true);
        // imgPanel->set_no_value(true);
        imgPanel->set_image(readDaqEventRateResImg);

        QWidget window;
        QVBoxLayout * layout = new QVBoxLayout;
        layout->addWidget(imgPanel);
        window.setLayout(layout);
        window.show();
        app.exec();

      }
      t1.join();

    }
    
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
