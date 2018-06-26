//! \file cms/testing/test-cms-monitoring.cxx
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <datatools/clhep_units.h>

// This project:
#include <vire/cms/monitoring.h>
#include <vire/cms/monitoring_record.h>
#include <vire/time/utils.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;
  try {

    {
      vire::cms::monitored_value_type value;
      value = 1.23 * CLHEP::volt;
      std::clog << "Value = " << boost::get<double>(value) / CLHEP::volt << " volt" << std::endl;
      std::clog << std::endl;
    }

    {
      vire::cms::monitoring_record record;
      record.grab_status().set_timestamp(vire::time::now());
      record.grab_status().set_missing(false);
      record.grab_status().set_disabled(false);
      record.grab_status().set_pending(false);
      record.grab_status().set_failed(false);
      record.set_real_value(1.23 * CLHEP::volt);
      record.tree_dump(std::clog, "Record:", "1 => ");

      record.grab_status().set_timestamp(vire::time::now());
      record.grab_status().set_failed(true);
      record.set_integer_value(234);
      record.tree_dump(std::clog, "Record:", "2 => ");

      record.grab_status().set_timestamp(vire::time::now());
      record.grab_status().set_failed(false);
      record.set_string_value("Foo");
      record.tree_dump(std::clog, "Record:", "3 => ");
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
