//! \file utility/testing/test-utility-base_alarm.cxx
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
// - Boost
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
// - Bayeux/datatools
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/exception.h>
// - BxJsontools
#include <jsontools/iofile.h>

// This project:
#include <vire/utility/base_alarm.h>
#include <vire/time/utils.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    {
      // Base alarm:
      vire::utility::base_alarm alarm;
      alarm.set_timestamp(vire::time::now());
      alarm.set_severity("very_serious");
      alarm.set_message("This is a very serious alarm!");
      alarm.tree_dump(std::clog, "Alarm:");
      std::clog << std::endl;
      jsontools::store("test-utility-base_alarm.json", alarm);
    }

    {
      vire::utility::base_alarm alarm;
      jsontools::load("test-utility-base_alarm.json", alarm);
      alarm.tree_dump(std::clog, "Loaded alarm:");
      std::clog << std::endl;
    }

  } catch (std::exception & error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
