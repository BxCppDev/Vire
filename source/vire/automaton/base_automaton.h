//! \file  vire/automaton/base_automaton.h
//! \brief The base automaton
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

#ifndef VIRE_AUTOMATON_BASE_AUTOMATON_H
#define VIRE_AUTOMATON_BASE_AUTOMATON_H

// Standard Library:
#include <string>
#include <map>
#include <memory>
#include <set>
#include <atomic>

// Third party:
// - Boost:
#include <boost/optional.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/automaton/utils.h>

namespace vire {

  namespace automaton {

    /// \brief Description of a CMS subcontractor base automaton 
    class base_automaton
      : public datatools::enriched_base
    {
    public:

      typedef std::map<std::string, std::shared_ptr<base_automaton>> child_dict_type;
       
      /// Default constructor
      base_automaton();

      /// Destructor
      ~base_automaton();

      bool has_run_tick() const;

      void set_run_tick(const vire::time::system_duration &);

      const vire::time::system_duration & get_run_tick() const;

      bool has_run_duration() const;

      void set_run_duration(const vire::time::system_duration &);
 
      const vire::time::system_duration & get_run_duration() const;

      bool has_run_start_time() const;

      void set_run_start_time(const vire::time::system_time_point &);
 
      const vire::time::system_time_point & get_run_start_time() const;
 
      bool has_run_stop_time() const;

      void set_run_stop_time(const vire::time::system_time_point &);
 
      const vire::time::system_time_point & get_run_stop_time() const;
     
      std::set<std::string> child_names() const;
      
      bool has_child(const std::string & name_) const;
      
      void add_child(const std::shared_ptr<base_automaton> & child_,
                     const std::string & name_ = "");

      void remove_child(const std::string & name_);

      const base_automaton & get_child(const std::string & name_) const;

      /// Check initialization flag
      bool is_initialized() const;

      void initialize_simple();
      
      void initialize(const datatools::properties &);
      
      void reset();
 
      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const override;

      //! Check running status
      bool is_running() const;

      //! Run
      void run();

      //! Send a stop request
      void stop_request();

      void common_initialize(const datatools::properties &);

      void common_reset();

      const automation_control & get_control() const;

      void operator()();
      
    private:

      virtual void _at_initialize_(const datatools::properties &);
      
      virtual void _at_reset_();
     
      virtual void _at_stop_request_();
      
      virtual void _at_run_loop_();
      
    private:

      // Management:
      bool               _initialized_ = false;
      automation_control _control_;
      
      // Configuration:
      vire::time::system_duration _run_tick_;
      // boost::optional<std::size_t>       _run_max_loop_;
      boost::optional<vire::time::system_time_point> _run_start_time_;
      boost::optional<vire::time::system_duration>   _run_duration_;
      boost::optional<vire::time::system_time_point> _run_stop_time_;
      child_dict_type _children_; //!< Dictionary of child automatons

      // Factory declaration :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_automaton)

    };
    
  } // namespace automaton

} // namespace vire

#define VIRE_AUTOMATON_REGISTRATION_INTERFACE(AUTOMATON_CLASS_NAME)     \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::automaton::base_automaton, AUTOMATON_CLASS_NAME) \
  public:                                                               \
  virtual std::string automaton_class_id() const;                       \
  /**/

#define VIRE_AUTOMATON_REGISTRATION_IMPLEMENT(AUTOMATON_CLASS_NAME,AUTOMATON_ID) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION (::vire::automaton::base_automaton,AUTOMATON_CLASS_NAME,AUTOMATON_ID) \
  std::string AUTOMATON_CLASS_NAME::automaton_class_id() const {return AUTOMATON_ID;} \
  /**/

#endif // VIRE_AUTOMATON_BASE_AUTOMATON_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
