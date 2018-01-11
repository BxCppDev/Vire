//! \file  vire/inspector/ihs_handler.h
//! \brief Vire inspector IHS handler
//
// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of datatools.
//
// datatools is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// datatools is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with datatools. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_INSPECTOR_IHS_HANDLER_H
#define VIRE_INSPECTOR_IHS_HANDLER_H

// Standard library:
#include <string>
#include <map>
#include <memory>

// Third Party:
// - Boost:
#include <boost/noncopyable.hpp>
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/ui/ihs.h>

// Vire library:
#include <vire/device/manager.h>
#include <vire/device/instance_tree.h>

// This project:
#include <vire/inspector/device_desc.h>
#include <vire/inspector/device_command_interface.h>

namespace vire {

  namespace inspector {

    //! \brief A IHS handler
    class ihs_handler
      : private boost::noncopyable
    {
    public:

      typedef std::shared_ptr<device_command_interface>    dev_interface_handler;
      typedef std::map<std::string, dev_interface_handler> dev_interfaces_dict_type;
      typedef std::map<std::string, device_descriptor_ptr> dev_descriptors_dict_type;

      bool ui_is_root_path(const std::string & ui_path_) const;

      std::string ui_parent_path(const std::string & ui_path_) const;

      std::string ui_to_instance_path(const std::string & ui_path_) const;

      std::string ui_leaf_path(const std::string & ui_path_) const;

      std::string instance_to_ui_path(const std::string & path_) const;

      //! Default constructor
      ihs_handler();

      //! Destructor
      virtual ~ihs_handler();

      //! Return the logging priority threshold
      datatools::logger::priority get_logging() const;

      //! Set the logging priority threshold
      void set_logging(datatools::logger::priority);

      //! Check if the device manager is set
      bool has_device_manager() const;

      //! Set the device manager
      void set_device_manager(const vire::device::manager &);

      //! Return the device manager
      const vire::device::manager & get_device_manager() const;

      //! Return a const reference to the embedded IHS
      const datatools::ui::ihs & get_ihs() const;

      //! Return a mutable reference to the embedded IHS
      datatools::ui::ihs & grab_ihs();

      //! Check initialization flag
      bool is_initialized() const;

      //! Initialization
      void initialize();

      //! Reset
      void reset();

    protected:

      //! Protected initialization
      void _init_();

      //! Construction of device descriptors
      void _construct_device_descriptors();

      // //! Construction of logical device descriptors
      // void _construct_logical_device_descriptors(const std::string & path_,
      //                                            const vire::device::logical_device &);

      //! Protected IHS construction
      void _construct_ihs();

      //! Protected devices IHS construction
      void _construct_devices_ihs();

      //! Protected IHS destruction
      void _destroy_ihs();

      void _plug_device_interface(const std::string & path_);

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag
      datatools::logger::priority _logging_; //! Logging priority threshold

      // Configuration:
      const vire::device::manager * _device_manager_ = nullptr; //!< Handle to the device manager

      // Working data/resources:
      const vire::device::instance_tree * _tree_ = nullptr;  //!< Handle to the device instance tree
      std::string                         _top_path_;        //!< Top path
      dev_descriptors_dict_type           _dev_descriptors_; //!< Dictionary of device descriptors
      dev_interfaces_dict_type            _dev_interfaces_;  //!< Dictionary of device command interfaces
      std::unique_ptr<datatools::ui::ihs> _ihs_;             //!< Device IHS

    };

  } // namespace inspector

} // namespace vire

#endif // VIRE_INSPECTOR_IHS_HANDLER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
