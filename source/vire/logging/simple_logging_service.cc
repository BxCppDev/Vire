//! \file vire/logging/simple_logging_service.cc
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/logging/simple_logging_service.h>

#define BOOST_LOG_DYN_LINK

// Standard library:
#include <unistd.h>

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include "boost/bimap.hpp"
#include "boost/bimap/set_of.hpp"
#include "boost/bimap/multiset_of.hpp"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>

// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/logger.h>
#include <datatools/utils.h>

namespace {

  //! Hide Boost bimap declaration in a typedef
  struct severity_type_lookup {
    typedef boost::bimap<
      boost::bimaps::set_of<std::string>,
      boost::bimaps::multiset_of<boost::log::trivial::severity_level>
      > lookup_table;
  };

  //! Construct the lookup table.
  const severity_type_lookup::lookup_table & severity_lookup_table()
  {
    static severity_type_lookup::lookup_table _lt;
    if (!_lt.size()) {
      boost::assign::insert(_lt)
        ("trace",   boost::log::trivial::trace)
        ("debug",   boost::log::trivial::debug)
        ("info",    boost::log::trivial::info)
        ("warning", boost::log::trivial::warning)
        ("error",   boost::log::trivial::error)
        ("fatal",   boost::log::trivial::fatal)
        ("default", boost::log::trivial::fatal)
        ;
    }
    return _lt;
  }

}

namespace vire {

  namespace logging {

    // No public interface for this function:
    boost::log::trivial::severity_level get_severity_level(const std::string & label_)
    {
      const severity_type_lookup::lookup_table & table = severity_lookup_table();
      // if (_table.empty()) _table = severity_lookup_table();
      severity_type_lookup::lookup_table::left_const_iterator sl = table.left.find(label_);
      return sl != table.left.end() ? sl->second : boost::log::trivial::fatal;
    }

    // No public interface for this function:
    bool severity_level_label_is_valid(const std::string & label_)
    {
      const severity_type_lookup::lookup_table & table = severity_lookup_table();
      // static severity_type_lookup::lookup_table _table;
      // if (_table.empty()) _table = construct_severity_lookup_table();
      severity_type_lookup::lookup_table::left_const_iterator sl = table.left.find(label_);
      return sl != table.left.end();
    }

    // No public interface for this function:
    std::string get_severity_level_label(boost::log::trivial::severity_level sl_)
    {
      const severity_type_lookup::lookup_table & table = severity_lookup_table();
      // static severity_type_lookup::lookup_table _table;
      // if (_table.empty()) _table = construct_severity_lookup_table();
      severity_type_lookup::lookup_table::right_const_iterator lbl = table.right.find(sl_);
      return lbl != table.right.end() ? lbl->second : "";
    }

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(simple_logging_service, "vire::logging::simple_logging_service");
    typedef boost::scoped_ptr<boost::log::sources::severity_logger< boost::log::trivial::severity_level > > scoped_logger_type;

    // PIMPL:
    struct simple_logging_service::work_type {
      boost::log::trivial::severity_level severity; //!< Current severity
      boost::shared_ptr< boost::log::sinks::sink > logconsole_sink;
      boost::shared_ptr< boost::log::sinks::sink > logfile_sink;
      // boost::log::sources::severity_logger< boost::log::trivial::severity_level > logger; //!< Private logger
      // scoped_logger_type glogger; //!< Scoped private logger
      work_type();
    };

    simple_logging_service::work_type::work_type()
    {
      severity = boost::log::trivial::fatal;
      return;
    }

    simple_logging_service::simple_logging_service()
    {
      _initialized_ = false;
      _set_defaults();
      return;
    }

    simple_logging_service::~simple_logging_service()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool simple_logging_service::is_initialized() const
    {
      return _initialized_;
    }

    int simple_logging_service::initialize(const datatools::properties & config_,
                                           datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Simple_Logging_Service is already initialized !");

      this->::datatools::base_service::common_initialize(config_);

      // Fetch configuration parameters:
      if (config_.has_key("logconsole")) {
        set_use_logconsole(config_.fetch_boolean("logconsole"));
      }

      if (config_.has_key("logfile")) {
        set_use_logfile(config_.fetch_boolean("logfile"));
      }

      if (_use_logfile_) {

        if (_logfile_dir_.empty()) {
          if (config_.has_key("logfile.dir")) {
            std::string dir = config_.fetch_string("logfile.dir");
            datatools::fetch_path_with_env(dir);
            set_logfile_dir(dir);
          }
        }

        if (_logfile_pattern_.empty()) {
          if (config_.has_key("logfile.pattern")) {
            std::string pattern = config_.fetch_string("logfile.pattern");
            set_logfile_pattern(pattern);
          }
        }

      if (_logfile_rotate_size_ == 0) {
        if (config_.has_key("logfile.rotate_size")) {
          unsigned int rotate_size = config_.fetch_positive_integer("logfile.rotate_size");
          set_logfile_rotate_size(rotate_size);
        }
      }
      if (_logfile_rotate_size_ == 0) {
        set_logfile_rotate_size(DEFAULT_ROTATE_SIZE);
      }

      }

      if (_severity_label_.empty()) {
        if (config_.has_key("severity")) {
          std::string label = config_.fetch_string("severity");
          set_severity_label(label);
        }
      }

      if (_severity_label_.empty()) {
        DT_LOG_DEBUG(get_logging_priority(), "Set default severity...");
        _severity_label_ = "default";
      }

      _init();
      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void simple_logging_service::_set_defaults()
    {
      _use_logconsole_ = false;
      _use_logfile_ = false;
      _logfile_rotate_size_ = 0;
      return;
    }

    int simple_logging_service::reset()
    {
      DT_THROW_IF(!_initialized_, std::logic_error, "Simple global logging is not initialized !");
      _reset();
      _initialized_ = false;
      return datatools::SUCCESS;
    }

    void simple_logging_service::set_use_logconsole(bool c_)
    {
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Simple global logging is already initialized !");
      _use_logconsole_ = c_;
      return;
    }

    void simple_logging_service::set_use_logfile(bool f_)
    {
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Simple global logging is already initialized !");
      _use_logfile_ = f_;
      return;
    }

    void simple_logging_service::set_logfile_dir(const std::string & dir_)
    {
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Simple_Logging_Service is already initialized !");
      _logfile_dir_ = dir_;
      return;
    }

    void simple_logging_service::set_logfile_pattern(const std::string & pattern_)
    {
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Simple global logging is already initialized !");
      _logfile_pattern_ = pattern_;
      return;
    }

    // datatools::command::returned_info
    // simple_logging_service::cmd_set_severity(const std::string & severity_)
    // {
    //   datatools::command::returned_info cri;
    //   try {
    //     set_severity_label(severity_);
    //   } catch (std::exception & error) {
    //     cri.set_error_code(datatools::command::CEC_PARAMETER_INVALID_VALUE);
    //   }
    //   return cri;
    // }

    // datatools::command::returned_info
    // simple_logging_service::cmd_get_severity()
    // {
    //   datatools::command::returned_info cri;
    //   cri.set_output(_severity_label_);
    //   return cri;
    // }

    const std::string & simple_logging_service::get_severity_label() const
    {
      return _severity_label_;
    }

    void simple_logging_service::set_severity_label(const std::string & severity_)
    {
      DT_THROW_IF(!severity_level_label_is_valid(severity_), std::logic_error, "Severity '" << severity_ << "' is not supported!");
      _severity_label_ = severity_;
      _update_filter();
      return;
    }

    void simple_logging_service::set_logfile_rotate_size(unsigned int sz_)
    {
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Simple global logging is already initialized !");
      _logfile_rotate_size_ = sz_;
      return;
    }

    void simple_logging_service::_update_filter()
    {
      if (_work_.get() != nullptr) {
        // Update the logging core severity filter:
        _work_->severity = get_severity_level(_severity_label_);
        boost::log::core::get()->set_filter(boost::log::trivial::severity >= _work_->severity);
      }
      return;
    }

    void simple_logging_service::set_enable_logging(bool enable_log_)
    {
      boost::log::core::get()->set_logging_enabled(enable_log_);
      return;
    }

    void simple_logging_service::_init()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      _work_.reset(new work_type);
      std::ostringstream logfilepath_oss;
      if (! _logfile_dir_.empty()) {
        logfilepath_oss << _logfile_dir_ << '/';
      }
      logfilepath_oss << _logfile_pattern_;

      // Default size for the log file :
      unsigned int logfile_rotate_size = 10 * 1024 * 1024;
      if (_logfile_rotate_size_ > 0) {
        logfile_rotate_size = _logfile_rotate_size_;
      }

      _update_filter();

      auto log_format = boost::log::expressions::stream
        << boost::log::expressions::attr< unsigned int >("LineID")
        << "|" << boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d_%H:%M:%S.%f")
        << "|" << boost::log::trivial::severity
        << "|" << boost::log::expressions::smessage
        ;
      // boost::log::keywords::format log_format = "[%TimeStamp%]: %Message%"

      if (_use_logconsole_) {
        DT_LOG_DEBUG(get_logging_priority(), "Activate console logging...");
        boost::shared_ptr< boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > > sink =
          boost::log::add_console_log(std::clog,
                                      boost::log::keywords::auto_flush = true,
                                      boost::log::keywords::format = log_format);
        _work_->logconsole_sink = sink;
      }

      if (_use_logfile_) {
        DT_LOG_DEBUG(get_logging_priority(), "Activate log file...");
        boost::shared_ptr< boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > > sink =
          boost::log::add_file_log(boost::log::keywords::file_name = logfilepath_oss.str(),
                                   boost::log::keywords::rotation_size = logfile_rotate_size,
                                   boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
                                   boost::log::keywords::format = log_format,
                                   boost::log::keywords::open_mode = std::ios_base::app);
        _work_->logfile_sink = sink;
      }

      DT_LOG_DEBUG(get_logging_priority(), "Add common attributes...");
      boost::log::add_common_attributes();

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void simple_logging_service::_reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (_work_->logconsole_sink.get() != 0) {
        DT_LOG_TRACE(get_logging_priority(), "Removing log console from the Boost/Log core...");
        boost::log::core::get()->remove_sink(_work_->logconsole_sink);
      }
      if (_work_->logfile_sink.get() != 0) {
        DT_LOG_TRACE(get_logging_priority(), "Removing log file from the Boost/Log core...");
        boost::log::core::get()->remove_sink(_work_->logfile_sink);
      }
      _work_.reset();
      _logfile_pattern_.clear();
      _logfile_dir_.clear();
      _severity_label_.clear();
      _set_defaults();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    bool simple_logging_service::is_trace() const
    {
      return _severity_label_ == get_severity_level_label(boost::log::trivial::trace);
    }

    bool simple_logging_service::is_debug() const
    {
      return _severity_label_ == get_severity_level_label(boost::log::trivial::debug);
    }

    bool simple_logging_service::is_info() const
    {
      return _severity_label_ == get_severity_level_label(boost::log::trivial::info);
    }

    bool simple_logging_service::is_warning() const
    {
      return _severity_label_ == get_severity_level_label(boost::log::trivial::warning);
    }

    bool simple_logging_service::is_error() const
    {
      return _severity_label_ == get_severity_level_label(boost::log::trivial::error);
    }

    bool simple_logging_service::is_fatal() const
    {
      return _severity_label_ == get_severity_level_label(boost::log::trivial::fatal);
    }

    void simple_logging_service::tree_dump(std::ostream & out_,
                                           const std::string & title_,
                                           const std::string & indent_,
                                           bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Use log console : "
           << _use_logconsole_
           << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Use log file : "
           << _use_logfile_
           << std::endl;

      if (_use_logfile_) {
        out_ << indent_ << i_tree_dumpable::tag
             << "Log file directory : '"
             << _logfile_dir_ << "'"
             << std::endl;
        out_ << indent_ << i_tree_dumpable::tag
             << "Log file pattern : '"
             << _logfile_pattern_ << "'"
             << std::endl;
        out_ << indent_ << i_tree_dumpable::tag
             << "Log file rotate size : ["
             << _logfile_rotate_size_ << "]"
             << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Severity : '"
           << _severity_label_ << "'"
           << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Initialized : "
           << _initialized_
           << std::endl;

      return;
    }

  } // end of namespace logging

} // end of namespace vire
