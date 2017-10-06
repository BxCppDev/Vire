// vire/utility/backup_file.cc - Implementation of Vire backup file utility
//
// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Ourselves:
#include <vire/utility/backup_file.h>

// Standard library:
#include <sstream>
#include <map>

// Third party:
// - Boost:
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
// - Bayeux:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace utility {

    backup_file::backup_file(const std::string & path_,
                  const datatools::logger::priority logging_)
      : _logging_(logging_)
    {
      _set_file_path_(path_);
      sync();
      return;
    }

    void backup_file::purge(std::size_t count_)
    {
      std::vector<std::string> backup_ls;
      list_of_backup_files(backup_ls);
      std::size_t removed_counter = 0;
      std::size_t to_be_removed_counter = backup_ls.size();
      if (count_ <= backup_ls.size()) to_be_removed_counter -= count_;
      DT_LOG_INFORMATION(_logging_, "To be removed: [" << to_be_removed_counter << "]");
      for (auto backup_file : backup_ls) {
        boost::filesystem::path removed(backup_file);
        if (removed_counter >= to_be_removed_counter) {
          break;
        }
        DT_LOG_INFORMATION(_logging_, "Removing '" << removed.string() << "'...");
        boost::filesystem::remove(removed);
        removed_counter++;
      }
      sync();
      return;
    }

    bool backup_file::parse_backup_file(const std::string & full_backup_filename_,
                                        std::string & dir_,
                                        std::string & filename_,
                                        int32_t & backup_copy_) const
    {
      // Init:
      dir_.clear();
      filename_.clear();
      backup_copy_ = -1;
      boost::filesystem::path p(full_backup_filename_);
      boost::filesystem::path dir = p.parent_path();
      if (dir.string() != _directory_) return false;
      boost::filesystem::path backup_filename = p.filename();
      std::vector<std::string> strs;
      boost::split(strs, backup_filename.string(), boost::is_any_of("~"));
      if (strs.size() != 2) return false;
      std::string filename = strs[0];
      if (filename != _filename_) return false;
      int backup_copy = -1;
      {
        std::istringstream bciss(strs[1]);
        bciss >> backup_copy;
        if (backup_copy < 0) {
          return false;
        }
      }
      // Finally:
      dir_ = dir.string();
      filename_ = filename;
      backup_copy_ = backup_copy;
      return true;
    }

    void backup_file::list_of_backup_files(std::vector<std::string> & list_) const
    {
      list_.clear();
      namespace fs = boost::filesystem;
      std::map<int32_t, std::string> ls;
      fs::path dir(_directory_);
      fs::directory_iterator dir_iter;
      if (fs::exists(dir) && fs::is_directory(dir)) {
        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_iter(dir) ; dir_iter != end_iter ; ++dir_iter) {
          if (fs::is_regular_file(dir_iter->status())) {
            fs::path file = *dir_iter;
            std::string dir;
            std::string filename;
            int32_t backup_copy(-1);
            if (parse_backup_file(file.string(), dir, filename, backup_copy)) {
              ls[backup_copy] = file.string();
            }
          }
        }
      }
      for (const auto & p : ls) {
        list_.push_back(p.second);
      }
      return;
    }

    void backup_file::sync()
    {
      std::vector<std::string> backup_ls;
      list_of_backup_files(backup_ls);
      if (backup_ls.size()) {
        const std::string & last_backup = backup_ls.back();
        std::string dir;
        std::string filename;
        int32_t cpnum = -1;
        if (parse_backup_file(last_backup, dir, filename, cpnum)) {
          _last_copy_ = cpnum;
        }
      } else {
        _last_copy_ = -1;
      }
      return;
    }

    void backup_file::set_logging(const datatools::logger::priority l_)
    {
      _logging_ = l_;
      return;
    }

    datatools::logger::priority backup_file::get_logging() const
    {
      return _logging_;
    }

    std::string backup_file::get_directory() const
    {
      return _directory_;
    }

    std::string backup_file::get_filename() const
    {
      return _filename_;
    }

    int32_t backup_file::get_last_copy() const
    {
      return _last_copy_;
    }

    std::string backup_file::get_full_backup_filename(const int32_t num_) const
    {
      DT_THROW_IF(num_ < 0, std::range_error, "Invalid copy number!");
      std::ostringstream out;
      out << get_full_filename() << '~' << num_;
      return out.str();
    }

    std::string backup_file::get_full_filename() const
    {
      boost::filesystem::path backup_path = boost::filesystem::path(_directory_) / boost::filesystem::path(_filename_);
      return backup_path.string();
    }

    bool backup_file::exists() const
    {
      return boost::filesystem::is_regular_file(get_full_filename());
    }

    void backup_file::make_new_backup()
    {
      DT_THROW_IF(!exists(), std::logic_error, "Original file '" << get_full_filename() << "' does not exist!");
      int32_t next_cpnum = 0;
      if (_last_copy_ >= 0) {
        next_cpnum = _last_copy_ + 1;
      }
      std::string backup_filename = get_full_backup_filename(next_cpnum);
      boost::filesystem::path path(get_full_filename());
      boost::filesystem::path backup_path(backup_filename);
      boost::filesystem::copy_file(path, backup_path);
      sync();
      return;
    }

    void backup_file::print(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_ ) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }
      out_ << indent_ << "|-- Logging   : [" << _logging_ << "]" << std::endl;
      out_ << indent_ << "|-- Directory : '" << _directory_ << "'" << std::endl;
      out_ << indent_ << "|-- Filename  : '" << _filename_ << "'" << std::endl;
      out_ << indent_ << "|-- Full path : '" << get_full_filename() << "'" << std::endl;
      out_ << indent_ << "|-- Exists    : " << std::boolalpha << exists() << std::endl;
      out_ << indent_ << "`-- Last copy number : [" << get_last_copy() << "]" << std::endl;
      return;
    }

    void backup_file::_set_file_path_(const std::string & full_filename_)
    {
      boost::filesystem::path p(full_filename_);
      boost::filesystem::path dir = p.parent_path();
      if (dir.empty()) {
        dir = ".";
      }
      boost::filesystem::path leaf = p.filename();
      std::vector<std::string> strs;
      boost::split(strs, leaf.string(), boost::is_any_of("~"));
      DT_THROW_IF(strs.size() > 1, std::logic_error,
                  "Invalid file name '" << full_filename_ << "'!");
      _directory_ = dir.string();
      _filename_ = leaf.string();
      return;
    }

  } // namespace utility

} // namespace vire
