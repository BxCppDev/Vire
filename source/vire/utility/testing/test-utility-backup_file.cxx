//! \file utility/testing/test-utility-backup_file.cxx
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
#include <fstream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>

// This project:
#include <vire/utility/backup_file.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    std::string filename("/tmp/dummy.conf");
    {
      std::ofstream fout(filename.c_str(), std::ofstream::app);
      fout << "Test" << std::endl;
      fout.close();
    }

    vire::utility::backup_file backupFile(filename);
    backupFile.set_logging(datatools::logger::PRIO_INFORMATION);
    backupFile.print(std::clog, "Backuped file: ");
    backupFile.make_new_backup();
    backupFile.make_new_backup();
    backupFile.make_new_backup();
    backupFile.print(std::clog, "Backuped file: ");
    backupFile.purge(3); // Preserve the last 3 backup files
    std::clog << std::endl;

  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
