//! \file utility/testing/test-utility-metadata_record.cxx
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
#include <vire/utility/metadata_record.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    {
      // Base alarm:
      vire::utility::metadata_record md;
      md.set_key("foo");
      md.set_value("bar");
      md.tree_dump(std::clog, "Metadata record:");
      std::clog << std::endl;
      jsontools::store("test-utility-metadata_record.json", md);
    }

    {
      vire::utility::metadata_record md;
      jsontools::load("test-utility-metadata_record.json", md);
      md.tree_dump(std::clog, "Loaded metadata record:");
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
