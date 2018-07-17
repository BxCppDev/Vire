//! \file  vire/cmsserver/sc_info.h
//! \brief Vire CMS server subcontractor information
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_SC_INFO_H
#define VIRE_CMSSERVER_SC_INFO_H

// Standard Library:
#include <map>
#include <string>
#include <chrono>

// Third party:
// - Qt:
#include <QObject>
// - Boost:
#include <boost/noncopyable.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>

// This project:
#include <vire/device/manager.h>
#include <vire/resource/manager.h>
#include <vire/com/manager.h>
#include <vire/cms/image_status.h>
#include <vire/time/rt_utils.h>

namespace vire {

  namespace cms {
    class connection_success;
    class connection_failure;
    class disconnection_success;
    class disconnection_failure;
  }
  
  namespace cmsserver {

    class sc_info;
   
    struct sc_info_signal_emitter
      : public QObject
    {
      Q_OBJECT
    public:
      
      sc_info_signal_emitter(sc_info & sc_info_);

      const sc_info & get_subcontractor_info() const;
      
      sc_info & grab_subcontractor_info() const;

      void emit_connection_change();
 
      void emit_device_status_change(std::string device_path_);
 
      void emit_auto_connect_change();
     
    signals:
      
      void auto_connect_changed();
      
      void connection_changed();

      void device_status_changed(std::string device_path_);
  
    private:

      sc_info & _subcontractor_info_;

    };

    /// \brief Dynamic informations about a connectable subcontractor 
    class sc_info
      : public datatools::enriched_base
      , private boost::noncopyable
    {
    public:

      static float default_connection_timeout();

      struct device_info {
        // Configuration:
        std::string             selection;

        // Dynamic:
        vire::cms::image_status status;
      };

      sc_info();
      
      sc_info(const sc_info & other_) = delete;
      
      sc_info & operator=(const sc_info & other_) = delete;

      virtual ~sc_info();

      void set_com_manager(vire::com::manager & com_);

      void set_device_manager(const vire::device::manager & devices_);
      
      void set_resource_manager(const vire::resource::manager & resources_);
      
      void build_mounted_devices(std::set<std::string> & names_) const;

      void add_mounted_device(const std::string & device_path_, const std::string & selection_ = "");

      bool has_mounted_device(const std::string & device_path_);

      void remove_mounted_device(const std::string & device_path_);
      
      bool is_auto_connect() const;

      void set_auto_connect(const bool flag_);

      bool has_max_connection_attempts() const;
      
      std::size_t get_max_connection_attempts() const;

      void set_max_connection_attempts(const std::size_t);
      
      bool is_initialized() const;

      void initialize_simple();
      
      void initialize(const datatools::properties & config_);

      void reset();
      
      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;
  
      bool is_connected() const;

      void set_connected(const bool);

      bool has_connection_start_time() const;
      
      const vire::time::system_time_point & get_connection_start_time() const;

      const device_info & get_mounted_device_info(const std::string & device_path_) const;
 
      device_info & grab_mounted_device_info(const std::string & device_path_);
     
      const sc_info_signal_emitter & get_emitter() const; 
     
      sc_info_signal_emitter & grab_emitter(); 

      void connect();
      
      void disconnect();
      
      std::size_t get_number_of_failed_connection_attempts() const;

      bool can_reconnect() const;

      const std::set<std::string> & get_resource_path() const;
      
    private:
    
      void _at_init_();
     
      void _at_reset_();
    
      void _on_connect_();
  
      void _on_disconnect_();
     
      sc_info_signal_emitter & _grab_emitter_();

      void _process_connection_success_(const vire::cms::connection_success &);
      
      void _process_disconnection_success_(const vire::cms::disconnection_success &);
      
    private:

      // Management:
      bool _initialized_ = false;
      const vire::device::manager   * _devices_   = nullptr;
      const vire::resource::manager * _resources_ = nullptr;
      vire::com::manager            * _com_       = nullptr;

      // Configuration:
      bool        _auto_connect_ = true;
      std::size_t _max_connection_attempts_ = 0;
      std::string _sc_login_;
      std::string _sc_password_;
     
      // Work data:
      std::string _svr_login_;
      std::string _svr_password_;
      bool        _connected_ = false;
      std::size_t _number_of_failed_connection_attempts_ = 0;
      vire::time::system_time_point _connection_start_time_;
      vire::time::system_time_point _last_disconnection_time_;
      std::map<std::string, device_info> _mounted_devices_;
      std::unique_ptr<std::set<std::string>> _resource_paths_;
      
      // Qt signal emitter wrapper:
      std::unique_ptr<sc_info_signal_emitter> _emitter_;
       
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_SC_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
