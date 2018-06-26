//! \file vire/automaton/base_automaton.cc
//
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/automaton/base_automaton.h>

// Standard Library:
#include <thread>
#include <random>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/clhep_units.h>

namespace vire {

  namespace automaton {

    // Factory system register :
    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_automaton,
                                                     "vire::automaton::base_automaton/_system_")
  
    base_automaton::base_automaton()
    {
      _control_.stop_requested.store(false);
      _run_tick_ = vire::time::system_duration::zero();
      return;
    }
    
    base_automaton::~base_automaton()
    {
      return;
    }

    bool base_automaton::has_run_tick() const
    {
      return _run_tick_ > std::chrono::microseconds(1);
    } 
    
    void base_automaton::set_run_tick(const vire::time::system_duration & tick_)
    {
      DT_THROW_IF(tick_ < std::chrono::microseconds(100),
                  std::logic_error,
                  "Invalid run tick value!");
      _run_tick_ = tick_;
      return;
    }

    const vire::time::system_duration & base_automaton::get_run_tick() const
    {
      return _run_tick_;
    }

    bool base_automaton::has_run_duration() const
    {
      return _run_duration_ != boost::none;
    }

    void base_automaton::set_run_duration(const vire::time::system_duration & d_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "CMS automaton is initialized and locked!");
      DT_THROW_IF(has_run_stop_time(), std::logic_error,
                  "CMS automaton has a run stop time!");
      _run_duration_ = d_;
      return;
    }
 
    const vire::time::system_duration & base_automaton::get_run_duration() const
    {
      DT_THROW_IF(!has_run_duration(), std::logic_error,
                  "Automaton '" << get_name() << "' has no run duration!");
      return _run_duration_.value();
    }

    bool base_automaton::has_run_start_time() const
    {
      return _run_start_time_ != boost::none;
    }

    void base_automaton::set_run_start_time(const vire::time::system_time_point & t_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "CMS automaton is initialized and locked!");
      DT_THROW_IF(has_run_stop_time() && t_ >= get_run_stop_time(), std::logic_error,
                  "Invalid start/stop time!");
      _run_start_time_ = t_;
      return;
    }

    const vire::time::system_time_point & base_automaton::get_run_start_time() const
    {
      DT_THROW_IF(!has_run_start_time(), std::logic_error,
                  "Automaton '" << get_name() << "' has no start time!");
      return _run_start_time_.value();
    }
 
    bool base_automaton::has_run_stop_time() const
    {
      return _run_stop_time_ != boost::none;
    }

    void base_automaton::set_run_stop_time(const vire::time::system_time_point & t_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "CMS automaton is initialized and locked!");
      DT_THROW_IF(has_run_duration(), std::logic_error,
                  "CMS automaton has a run duration!");
      DT_THROW_IF(has_run_start_time() && t_ <= get_run_start_time(), std::logic_error,
                  "Invalid start/stop time!");
      _run_stop_time_ = t_;
      return;
    }

    const vire::time::system_time_point & base_automaton::get_run_stop_time() const
    {
      DT_THROW_IF(!has_run_stop_time(), std::logic_error,
                  "Automaton '" << get_name() << "' has no stop time!");
      return _run_stop_time_.value();
    }
 
    std::set<std::string> base_automaton::child_names() const
    {
      std::set<std::string> nms;
      for (const auto & p : _children_) {
        nms.insert(p.first);
      }
      return nms;
    }
 
    bool base_automaton::has_child(const std::string & name_) const
    {
      return _children_.count(name_) > 0;
    }
      
    void base_automaton::add_child(const std::shared_ptr<base_automaton> & child_,
                                   const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "CMS automaton is initialized and locked!");
      DT_THROW_IF(!child_, std::logic_error,
                  "CMS automaton's child is not instantiated!");
      DT_THROW_IF(!child_->is_initialized(), std::logic_error,
                  "CMS automaton's child is not initialized!");
      std::string name = name_;
      if (name.empty()) {
        DT_LOG_DEBUG(get_logging_priority(), "Name is empty. Using child's name...");
        name = child_->get_name();
      }
      DT_THROW_IF(has_child(name), std::logic_error,
                  "CMS automaton already has a child named '" << name << "'!");
      DT_LOG_DEBUG(get_logging_priority(), "Adding child automaton '" << name << "'...");
      _children_[name] = child_;
      return;
    }

    void base_automaton::remove_child(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "CMS automaton is initialized and locked!");
      DT_THROW_IF(!has_child(name_), std::logic_error,
                  "CMS automaton has not child named '" << name_ << "'!");
      _children_.erase(name_);
      return;
    }

    const base_automaton & base_automaton::get_child(const std::string & name_) const
    {
      child_dict_type::const_iterator found = _children_.find(name_);
      DT_THROW_IF(found == _children_.end(),
                  std::logic_error,
                  "CMS automaton has not child named '" << name_ << "'!");
      return *found->second.get();
    }
    
    bool base_automaton::is_initialized() const
    {
      return _initialized_;
    }

    void base_automaton::common_initialize(const datatools::properties & config_)
    {
      datatools::enriched_base::initialize(config_, false);
      DT_THROW_IF(!has_name(), std::logic_error,
                  "Missing automation name!");

      if (! has_run_tick()) {
        if (config_.has_key("run.tick")) {
          double run_tick_value
            = config_.fetch_real_with_explicit_dimension("run.tick", "time");
          DT_THROW_IF(run_tick_value < 0.0,
                      std::logic_error,
                      "Invalid run tick value!");
          std::size_t run_tick_musec = (std::size_t) (run_tick_value / CLHEP::microsecond);
          set_run_tick(std::chrono::microseconds(run_tick_musec));
        }
      }
      
      if (!has_run_duration()) {
        if (config_.has_key("run.duration")) {
          double run_duration_value
            = config_.fetch_real_with_explicit_dimension("run.duration", "time");
          DT_THROW_IF(run_duration_value < 0.0,
                      std::logic_error,
                      "Invalid run duration value!");
          std::size_t run_duration_sec = (std::size_t) (run_duration_value / CLHEP::second);
          // Default run duration:
          set_run_duration(std::chrono::seconds(run_duration_sec));
        }
      }
  
      return;
    }

    void base_automaton::common_reset()
    {
      _control_.reset();
      datatools::enriched_base::reset();
      return;
    }

    void base_automaton::initialize_simple()
    {
      initialize(datatools::empty_config());
      return;
    }
    
    void base_automaton::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "CMS automaton is already initialized!");

      common_initialize(config_);
      
      _at_initialize_(config_);
        
      _initialized_ = true;
      return;
    }
     
    void base_automaton::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "CMS automaton is not initialized!");
      _initialized_ = false;

      _at_reset_();
      
      common_reset();
        return;
    }

    // virtual
    void base_automaton::_at_initialize_(const datatools::properties &)
    {
      return;
    }
      
    // virtual
    void base_automaton::_at_reset_()
    {
      return;
    }

    void base_automaton::stop_request()
    {
      _control_.stop_requested.store(true);
      _at_stop_request_();
      return;
    }

    // virtual
    void base_automaton::_at_stop_request_()
    {
      return;
    }

    std::default_random_engine & random_engine()
    {
      static std::default_random_engine _reg;
      return _reg;
    }

    // virtual
    void base_automaton::_at_run_loop_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "Automaton '" << get_name() << "' loop...");

      int test_mode = 1;
      vire::time::system_time_point until = _control_.loop_stop_time - _run_tick_ / 2;
      if (test_mode == 1) {
        std::uniform_int_distribution<int> distribution(0, 2);
        int random_n = distribution(random_engine());
        until += random_n * _run_tick_;
      }
      std::this_thread::sleep_until(until);
      return;
    }

    bool base_automaton::is_running() const
    {
      return _control_.start_time != vire::time::system_epoch();
    }
   
    void base_automaton::operator()()
    {
      run();
      return;
    }
 
    void base_automaton::run()
    {
      DT_LOG_DEBUG(get_logging_priority(), "Running automaton '" << get_name() << "'...");
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Automaton '" << get_name() << "' is not initialized!");
      vire::time::system_time_point now = std::chrono::system_clock::now();
      if (has_run_start_time()) {
        DT_LOG_DEBUG(get_logging_priority(), "Automaton '" << get_name() << "' waits before to start...");
        std::this_thread::sleep_until(_run_start_time_.value());
      }
      _control_.start_time = std::chrono::system_clock::now();
      if (has_run_duration()) {
        _control_.requested_stop_time = _control_.start_time + _run_duration_.value();
      } else if (has_run_stop_time()) {
        _control_.requested_stop_time = _run_stop_time_.value();
      }
      bool stop_it = _control_.stop_requested.load();
      if ( _control_.start_time > _control_.requested_stop_time) {
        // We started too late:
        stop_it = true;
      }
      try {
        while (! stop_it) {
          _control_.loop_start_time = std::chrono::system_clock::now();
          _control_.loop_stop_time = _control_.loop_start_time + _run_tick_;
          _at_run_loop_();
          vire::time::system_time_point now = std::chrono::system_clock::now();
          if (!stop_it) {
            if (has_run_duration() && (now > _control_.requested_stop_time)) {
              stop_it = true;
            }
          }
          if (!stop_it) {
            if (now < _control_.loop_stop_time) {
              DT_LOG_DEBUG(get_logging_priority(), "Automaton '" << get_name() << "' must sleep a while...");
              std::this_thread::sleep_until(_control_.loop_stop_time);
            } 
            _control_.loop++;
            stop_it = _control_.stop_requested.load();
          }
          std::this_thread::yield();
        }
        DT_LOG_DEBUG(get_logging_priority(), "Automaton '" << get_name() << "' has stopped.");
      } catch (std::exception & error) {
        _control_.error_code = 1;
        _control_.error_type_id = typeid(error).name();
        _control_.error_message = error.what();
      }
      _control_.stop_time = std::chrono::system_clock::now();
      return;
    }

    const automation_control & base_automaton::get_control() const
    {
      return _control_;
    }
    
    // virtual
    void base_automaton::print_tree(std::ostream & out_,
                                    const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      datatools::enriched_base::tree_dump(out_, "", popts.indent, true);

      out_ <<  popts.indent << tag
           << "Run tick : " << _run_tick_.count() << " system tick" << std::endl;

      out_ <<  popts.indent << tag
           << "Run start time : ";
      if (has_run_start_time()) {
        out_ << _run_start_time_.value().time_since_epoch().count() << " system tick";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Run duration : ";
      if (has_run_duration()) {
        out_ << _run_duration_.value().count() << " system tick";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Run stop time : ";
      if (has_run_stop_time()) {
        out_ << _run_stop_time_.value().time_since_epoch().count() << " system tick";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      
      out_ <<  popts.indent << tag
           << "Control : " << std::endl;

      out_ <<  popts.indent << skip_tag << tag
           << "Loop #" << _control_.loop << std::endl;

      out_ <<  popts.indent << skip_tag << tag
           << "Error code : " << _control_.error_code << std::endl;

      out_ <<  popts.indent << skip_tag << tag
           << "Error type ID : " << _control_.error_type_id << std::endl;

      out_ <<  popts.indent << skip_tag << tag
           << "Error message : '" << _control_.error_message << "'" << std::endl;

      out_ <<  popts.indent << skip_tag << last_tag
           << "Stop requested : " << std::boolalpha
           << _control_.stop_requested.load() << std::endl;

      out_ <<  popts.indent << tag
           << "Children : " << _children_.size() << std::endl;

      // List of children
      {
        std::size_t counter = 0;
        for (const auto & p : _children_) {
          out_ <<  popts.indent << skip_tag;
          if (++counter == _children_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Child '" << p.first << "'";
          out_ << std::endl;
        }
      }
      
      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Initialized : " << std::boolalpha << is_initialized()
           << std::endl;

      return;
    }

  } // namespace automaton

} // namespace vire

