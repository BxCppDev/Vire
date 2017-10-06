//! \file  vire/utility/backup_file.h
//! \brief Backup file
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

#ifndef VIRE_UTILITY_BACKUP_FILE_H
#define VIRE_UTILITY_BACKUP_FILE_H

// Standard library:
#include <string>
#include <iostream>
#include <vector>

// Third party:
// - Bayeux:
#include <bayeux/datatools/logger.h>

namespace vire {

  namespace utility {

    //! \brief Backup file
    class backup_file
    {
    public:

      /// Default constructor
      backup_file(const std::string & path_,
                  const datatools::logger::priority l_ = datatools::logger::PRIO_FATAL);

      /// Compute the list of backup files
      void list_of_backup_files(std::vector<std::string> & list_) const;

      /// Check if original file exist
      bool exists() const;

      /// Create a new backup copy of the file
      void make_new_backup();

      /// Purge backup
      void purge(std::size_t count_ = 0);

      /// Parse a filepath
      bool parse_backup_file(const std::string & filepath_,
                             std::string & dir_,
                             std::string & filename_,
                             int32_t & backup_copy_) const;

      /// Recompute the last backup copy number
      void sync();

      void set_logging(const datatools::logger::priority l_);

      datatools::logger::priority get_logging() const;

      std::string get_directory() const;

      std::string get_filename() const;

      int32_t get_last_copy() const;

      std::string get_full_filename() const;

      std::string get_full_backup_filename(const int32_t) const;

      /// Basic print
      void print(std::ostream & out_,
                 const std::string & title_ = "",
                 const std::string & indent_ = "") const;

    private:

      void _set_file_path_(const std::string & path_);

    private:

      datatools::logger::priority _logging_; //!< Logging priority
      std::string _directory_;               //!< Path of the hosting directory
      std::string _filename_;                //!< Filename
      int32_t     _last_copy_ = -1;          //!< Last backup copy number

    };

  } // namespace utility

} // namespace vire

#endif // VIRE_UTILITY_BACKUP_FILE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
