//! \file cms/testing/test-cms-image.cxx
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>

// This project:
#include <vire/cms/image.h>
#include <vire/device/logical_device.h>
#include <vire/utility/path.h>
#include <vire/resource/resource.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;
  try {

    {
      std::string rpath = "SuperNEMO:/Demonstrator/CMS/DAQ/start";
      std::string rname = vire::utility::path::to_address(rpath);
      vire::resource::resource r;
      r.set_name(rname);
      r.set_id(1000);
      r.set_path(rpath);
      r.set_access(vire::utility::RW_WRITABLE);
      r.set_max_number_of_tokens(1);
      r.set_display_name("DAQ start");
      r.initialize_simple();
      r.tree_dump(std::clog, "Resource '" + r.get_display_name() + "' : ");
      std::clog << std::endl;

      vire::cms::image img(r);
      img.set_timestamp(vire::time::now());
      img.set_missing(true);
      img.tree_dump(std::clog, "Image '"+ img.get_path() + "' : ");
      std::clog << std::endl;
    }

    {
      std::string dpath = "SuperNEMO:/Demonstrator/CMS/DAQ";
      std::string dname = vire::utility::path::to_address(dpath);
      vire::device::logical_device d;
      d.set_name(dname + ".log");
      d.initialize();
      d.tree_dump(std::clog, "Logical device '" + d.get_name() + "' : ");
      std::clog << std::endl;

      vire::cms::image img(dpath, d);
      img.set_timestamp(vire::time::now());
      img.set_disabled(true);
      img.tree_dump(std::clog, "Image '" + img.get_path() + "' : ");
      std::clog << std::endl;
    }

  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
