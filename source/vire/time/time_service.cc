//! \file vire/time/time_service.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/time/time_service.h>

// Standard library:
#include <sstream>
#include <fstream>

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/tz_db_base.hpp>
#include <boost/filesystem.hpp>

// Links:
//
// * https://www.boost.org/doc/libs/1_67_0/doc/html/date_time.html
// * https://time.is/CT
// * https://www.iana.org/time-zones

namespace {
  
  boost::posix_time::time_duration get_utc_offset_now()
  {
    using namespace boost::posix_time;
    
    // boost::date_time::c_local_adjustor uses the C-API to adjust a
    // moment given in utc to the same moment in the local time zone.
    typedef boost::date_time::c_local_adjustor<ptime> local_adj;
    
    const ptime utc_now = second_clock::universal_time();
    const ptime now     = local_adj::utc_to_local(utc_now);
    
    return now - utc_now;
  }
}

namespace vire {

  namespace time {

    // PIMPL:
    struct time_service::work_type {
      work_type();
      void reset();
      void load_time_zone_db(const std::string & path_ = "");

      std::unique_ptr<boost::local_time::tz_database> tz_db_ptr; //!< Time zone database pointer
      boost::local_time::time_zone_ptr                tz_ptr = nullptr; //!< Time zone pointer
    };

    time_service::work_type::work_type()
    {
      return;
    }

    void time_service::work_type::load_time_zone_db(const std::string & path_)
    {
      this->tz_db_ptr.reset(new boost::local_time::tz_database);
      std::string path = path_;
      if (path.empty()) {
        path = "@vire:data/time/date_time_zonespec.csv";
      }
      datatools::fetch_path_with_env(path);
      this->tz_db_ptr->load_from_file(path);
      return;
    }

    void time_service::work_type::reset()
    {
      if (this->tz_db_ptr.get() != nullptr) {
        this->tz_db_ptr.reset();
      }
      if (this->tz_ptr) {
        this->tz_ptr = nullptr;
      }
      return;
    }
    
    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(time_service, "vire::time::time_service");

    time_service::time_service()
    {
      _initialized_ = false;
      _set_defaults();
      return;
    }

    time_service::~time_service()
    {
      if (is_initialized()) {
        reset();
      }
      _work_.reset();
      return;
    }

    bool time_service::has_time_zone_database_path() const
    {
      return not _time_zone_db_path_.empty();
    }

    void time_service::set_time_zone_database_path(const std::string & time_zone_db_filename_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Time service is already initialized!");
      _time_zone_db_path_ = time_zone_db_filename_;
      return;
    }
   
    const std::string & time_service::get_time_zone_database_path() const
    {
      return _time_zone_db_path_;
    }

    bool time_service::has_time_zone_id() const
    {
      return not _time_zone_id_.empty();
    }

    void time_service::set_time_zone_id(const std::string & time_zone_id_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Time service is already initialized!");
        
      _time_zone_id_ = time_zone_id_;
      return;
    }
      
    const std::string & time_service::get_time_zone_id() const
    {
      return _time_zone_id_;
    }

    boost::local_time::time_zone_ptr time_service::get_time_zone() const
    {
      DT_THROW_IF(!is_initialized(),
                  std::logic_error,
                  "Time service is not initialized!");
      return _work_->tz_ptr;
    }

    boost::posix_time::ptime time_service::local_to_utc(const boost::posix_time::ptime & local_,
                                                        const bool dst_) const
    {
      if (local_.is_not_a_date_time()) {
        // std::clog << "[DEBUG] Invalid local time!" << std::endl;
        return boost::posix_time::ptime(boost::posix_time::not_a_date_time);
      } else if (local_.is_neg_infinity()) {
        // std::clog << "[DEBUG] Infinite local time!" << std::endl;
        return boost::posix_time::ptime(boost::posix_time::neg_infin);
      } else if (local_.is_pos_infinity()) {
        // std::clog << "[DEBUG] Infinite local time!" << std::endl;
        return boost::posix_time::ptime(boost::posix_time::pos_infin);
      }
      unsigned int year = local_.date().year();
      unsigned int month = local_.date().month();
      unsigned int day = local_.date().day();
      unsigned int hours = local_.time_of_day().hours();
      unsigned int minutes = local_.time_of_day().minutes();
      unsigned int seconds = local_.time_of_day().seconds();
      unsigned int microseconds = local_.time_of_day().total_microseconds() - local_.time_of_day().total_seconds() * 1000000;
      // std::clog << "[DEBUG] microseconds = " << microseconds << std::endl;
      boost::local_time::local_date_time ldt(boost::posix_time::not_a_date_time);
      
      std::tuple<bool, std::string> result = make_local_time(ldt, year, month, day, hours, minutes, seconds, microseconds, dst_);
      if (!std::get<0>(result)) {
        // std::clog << "[DEBUG] Cannot make a valid local time : " << std::get<1>(result) << "!" << std::endl;
        return boost::posix_time::ptime(boost::posix_time::not_a_date_time);
      }
      // std::clog << "[DEBUG] Computed UTC time = " << ldt.utc_time() << std::endl;
      return ldt.utc_time();
      /*
      // Brute force:
      std::ostringstream local_repr_oss;
      local_repr_oss << boost::posix_time::to_iso_string(local_);
      std::string local_repr = local_repr_oss.str();
      std::clog << "[DEBUG] local time         = " << local_repr << std::endl;
      boost::posix_time::ptime utc_time(boost::posix_time::not_a_date_time);
      boost::posix_time::time_duration time_shift = boost::posix_time::hours(-12);
      boost::posix_time::ptime max_utc_time = local_ + boost::posix_time::hours(12);
      while (true) {
        boost::posix_time::ptime attempt_utc_time = local_ - time_shift;
        boost::local_time::local_date_time tz_time(attempt_utc_time, get_time_zone());
        //std::clog << "[DEBUG] attempt local time = " << tz_time.local_time() << std::endl;
        boost::posix_time::ptime attempt_local = tz_time.local_time();
        std::ostringstream attempt_local_repr_oss;
        attempt_local_repr_oss << boost::posix_time::to_iso_string(attempt_local);
        std::string attempt_local_repr = attempt_local_repr_oss.str();
        std::clog << "[DEBUG] attempt local time = " << attempt_local_repr << std::endl;
        if (attempt_local_repr == local_repr) {
          std::clog << "[DEBUG] Found UTC time = " << attempt_utc_time << std::endl;
          utc_time = attempt_utc_time;
          break;
        }
        if (attempt_utc_time > max_utc_time) {
          break;
        }
        time_shift += boost::posix_time::minutes(30);
      }
      return utc_time;
      */
    }

    void time_service::invalidate_local_time(boost::local_time::local_date_time & local_time_) const
    {
      local_time_ = boost::local_time::local_date_time(boost::posix_time::not_a_date_time, get_time_zone());
      return;
    }
    
    void time_service::local_time_infos(const boost::posix_time::ptime & local_,
                                        bool & valid_,
                                        boost::logic::tribool & dst_) const
    {
      dst_ = boost::indeterminate;
      bool has_dst = get_time_zone()->has_dst();
      if (!has_dst) {
        dst_ = false;
      }
      unsigned int year = local_.date().year();
      unsigned int month = local_.date().month();
      unsigned int day = local_.date().day();
      unsigned int hours = local_.time_of_day().hours();
      unsigned int minutes = local_.time_of_day().minutes();
      unsigned int seconds = local_.time_of_day().seconds();
      unsigned int microseconds = local_.time_of_day().total_microseconds() - local_.time_of_day().total_seconds() * 1000000;
      bool ok_nodst = check_local_time(year,month,day,hours,minutes,seconds,microseconds,false);
      if (has_dst) {
        bool ok_dst = check_local_time(year,month,day,hours,minutes,seconds,microseconds,true);
        if (ok_dst && !ok_nodst) {
          valid_ = true;
          dst_ = true;
        } else if (!ok_dst && ok_nodst) {
          valid_ = true;
          dst_ = false;
        } else if (ok_dst && ok_nodst) {
          valid_ = true;
          dst_ = boost::indeterminate;
        }
      } else {
        if (ok_nodst) {
          valid_ = true;
        }
      }
      valid_ = false;
      return;
    }

    bool time_service::check_local_time(const unsigned int year_,
                                        const unsigned int month_,
                                        const unsigned int day_,
                                        const unsigned int hours_,
                                        const unsigned int minutes_,
                                        const unsigned int seconds_,
                                        const unsigned int microseconds_,
                                        const bool dst_) const
    {
      boost::local_time::local_date_time test_local_time(boost::posix_time::not_a_date_time, get_time_zone());
      return std::get<0>(make_local_time(test_local_time, year_, month_, day_, hours_, minutes_, seconds_, microseconds_, dst_));
    }
    
    std::tuple<bool, std::string> time_service::make_local_time(boost::local_time::local_date_time & local_time_,
                                                                const unsigned int year_,
                                                                const unsigned int month_,
                                                                const unsigned int day_,
                                                                const unsigned int hours_,
                                                                const unsigned int minutes_,
                                                                const unsigned int seconds_,
                                                                const unsigned int microseconds_,
                                                                const bool dst_) const
    {
      boost::gregorian::date dt(year_, month_, day_);
      boost::posix_time::time_duration td(hours_, minutes_, seconds_);
      td += boost::posix_time::microseconds(microseconds_);
      try {
        boost::local_time::local_date_time ldt(dt, td, get_time_zone(), dst_);
        local_time_ = ldt;
        return std::make_tuple(true, "");
      } catch (boost::local_time::dst_not_valid & bad_dst) {
        invalidate_local_time(local_time_);
        return std::make_tuple(false, "DST invalid");
      } catch (boost::local_time::time_label_invalid & bad_time) {
        invalidate_local_time(local_time_);
        return std::make_tuple(false, "Time invalid");
      } 
      return std::make_tuple(true, "");
    }
    
    boost::posix_time::ptime time_service::utc_to_local(const boost::posix_time::ptime & utc_) const 
    {
      if (utc_.is_not_a_date_time()) {
        return boost::posix_time::ptime(boost::date_time::not_a_date_time);
      } else if (utc_.is_neg_infinity()) {
        return boost::posix_time::ptime(boost::date_time::neg_infin);
      } else if (utc_.is_pos_infinity()) {
        return boost::posix_time::ptime(boost::date_time::pos_infin);
      } 
      boost::local_time::local_date_time tz_time(utc_, get_time_zone());
      return tz_time.local_time();
    }

    void time_service::_set_defaults()
    {
      _time_zone_id_.clear();
      _time_zone_db_path_.clear();
      return;
    }

    bool time_service::is_initialized() const
    {
      return _initialized_;
    }

    int time_service::initialize(const datatools::properties & config_,
                                 datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Time service is already initialized !");
      
      this->::datatools::base_service::common_initialize(config_);

      
      if (!has_time_zone_database_path()) {
        std::string time_zone_database_path;
        if (config_.has_key("time_zone_database")) {
          time_zone_database_path = config_.fetch_string("time_zone_database");
        }
        set_time_zone_database_path(time_zone_database_path);
      }
      
      if (!has_time_zone_id()) {
        std::string time_zone_id;
        if (config_.has_key("time_zone_id")) {
          time_zone_id = config_.fetch_string("time_zone_id");
        }
        set_time_zone_id(time_zone_id);
      }
      
      _init();
      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }
    
    int time_service::reset()
    {
      DT_THROW_IF(!_initialized_, std::logic_error, "Time service is not initialized !");
      _reset();
      _initialized_ = false;
      return datatools::SUCCESS;
    }

    void time_service::_init()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      _work_.reset(new work_type);

      // Time zone database:
      if (!has_time_zone_database_path()) {
        DT_LOG_DEBUG(get_logging_priority(),
                     "Using default time zone database...");
        set_time_zone_database_path("@vire:data/time/date_time_zonespec.csv");
      }
      _work_->load_time_zone_db(_time_zone_db_path_);

      // Time zone:
      if (! has_time_zone_id()) {
        if (boost::filesystem::exists("/etc/time_zone")) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Fetching the current time zone from the system...");
          std::string time_zone_filename{"/etc/time_zone"};
          std::ifstream time_zone_file;
          time_zone_file.open(time_zone_filename.c_str());
          if (!time_zone_file) {
            DT_THROW(std::runtime_error,
                     "Cannot open the system time zone file '" << time_zone_filename << "'!");
          }
          std::string time_zone_id;
          {
            std::string tzline;
            std::getline(time_zone_file, tzline);
            std::istringstream tzline_ss(tzline);
            tzline_ss >> time_zone_id;
          }
          time_zone_file.close();
          if (! time_zone_id.empty()) {
            set_time_zone_id(time_zone_id);
          }
        }
      }
      DT_THROW_IF(!has_time_zone_id(), std::logic_error,
                  "Missing time zone!");
      _work_->tz_ptr = _work_->tz_db_ptr->time_zone_from_region(_time_zone_id_);
      DT_THROW_IF(_work_->tz_ptr == nullptr,
                  std::logic_error,
                  "Unknown time zone '" << _time_zone_id_ << "'!"
                  );
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
    
    void time_service::_reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (_work_.get() != nullptr) {
        _work_.reset();
      }
      _set_defaults();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void time_service::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);
      
      out_ << indent_ << i_tree_dumpable::tag
           << "Time zone DB path : '"
           << _time_zone_db_path_ << "'"
           << std::endl;
      
      out_ << indent_ << i_tree_dumpable::tag
           << "Time zone    : '"
           << _time_zone_id_ << "'"
           << std::endl;
      
      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Initialized : "
           << std::boolalpha << is_initialized()
           << std::endl;

      return;
    }
      
  } // end of namespace logging

} // end of namespace vire
