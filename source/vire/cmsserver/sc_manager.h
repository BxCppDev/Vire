/// \file  vire/cmsserver/sc_manager.h
/// \brief Vire CMS server subcontractor manager
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_SC_MANAGER_H
#define VIRE_CMSSERVER_SC_MANAGER_H

// Standard Library:
#include <string>
#include <map>
#include <memory>

// Third party:
// - Boost:
#include <boost/noncopyable.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/base_service.h>

// This project:
#include <vire/resource/manager.h>
#include <vire/device/manager.h>
#include <vire/com/manager.h>
#include <vire/cmsserver/sc_driver.h>
#include <vire/running/run_control.h>
#include <vire/running/i_runnable.h>
#include <vire/user/simple_password_generator.h>

namespace vire {

  namespace cmsserver {
    
    /// \brief Vire CMS server
    class sc_manager
      : public datatools::base_service
      , public vire::running::i_runnable
      , private boost::noncopyable
     {
    public:
      
      /// \brief Dictionary of subcontractor configuration records
      typedef std::map<std::string, datatools::properties> sc_config_dict_type;
      
      /// \brief Dictionary of subcontractor R/T proxies
      typedef std::map<std::string, std::shared_ptr<sc_driver>> sc_dict_type;

      /// Default constructor
      sc_manager(uint32_t flags_ = 0);
      
      sc_manager(const sc_manager & other_) = delete;
      
      sc_manager & operator=(const sc_manager & other_) = delete;

      /// Destructor
      virtual ~sc_manager();

      bool has_device_service_name() const;

      void set_device_service_name(const std::string &);
      
      const std::string & get_device_service_name() const;

      bool has_resource_service_name() const;

      void set_resource_service_name(const std::string &);
      
      const std::string & get_resource_service_name() const;

      bool has_com_service_name() const;

      void set_com_service_name(const std::string &);
      
      const std::string & get_com_service_name() const;

      bool has_devices() const;

      void set_devices(const vire::device::manager &);

      const vire::device::manager & get_devices() const;

      bool has_resources() const;

      void set_resources(const vire::resource::manager &);

      const vire::resource::manager & get_resources() const;

      bool has_com() const;

      void set_com(vire::com::manager &);

      const vire::com::manager & get_com() const;

      vire::com::manager & grab_com();

      /// Check if the seed of the password generator is set
      bool has_password_gen_seed() const;

      /// Return the seed of the password generator
      unsigned int get_password_gen_seed() const;

      /// Set the seed of the password generator
      void set_password_gen_seed(const unsigned int);

      /// Check if the service is initialized
      bool is_initialized() const override;

      /// Initialize the service using a list of properties with access to a dictionary of services
      int initialize(const datatools::properties & config_,
                     datatools::service_dict_type & service_dict_) override;

      /// Reset the service
      int reset() override;

      /// Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

      void build_subcontractor_names(std::set<std::string> & names_) const;
      
      bool has_subcontractor(const std::string & name_) const;

      void add_subcontractor(const std::string & name_,
                             const datatools::properties & config_ = datatools::empty_config());
      
      const sc_driver & get_driver(const std::string & name_) const;

      sc_driver & grab_driver(const std::string & name_);

      void run() override;

      void stop() override;

      const vire::running::run_control & get_rc() const override;
      
      vire::user::simple_password_generator & grab_password_gen();
      
    private:

       void _post_initialization_();

       void _pre_reset_();
 
      // Running:
      void _at_run_start_();
      
      void _at_run_();
      
      void _at_run_loop_();
      
      void _at_run_stop_();
      
      void _set_defaults_();

    private:

      // Management:
      bool _initialized_ = false; ///< Initialization flag

      // Configuration:
      std::string _device_service_name_; 
      std::string _resource_service_name_; 
      std::string _com_service_name_; 
      const vire::device::manager   * _devices_   = nullptr;
      const vire::resource::manager * _resources_ = nullptr;
      vire::com::manager            * _com_       = nullptr;
      unsigned int                    _password_gen_seed_ = 0; ///< Seed of the password generator
      // Subcontractors
      sc_config_dict_type _subcontractor_configs_; ///< Collection of subcontractor configuration records

      // Working data:
      vire::running::run_control _rc_;      ///< Run control
      std::unique_ptr<vire::user::simple_password_generator> _password_gen_; ///< Embedded password generator
      sc_dict_type               _drivers_; ///< Dictionary of subcontractor driver handlers

      /// Auto-registration of this service class in the Bayeux/datatools' central service database
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(sc_manager)

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_SC_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
