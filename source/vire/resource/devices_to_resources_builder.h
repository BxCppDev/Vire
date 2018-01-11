//! \file  vire/resource/devices_to_resources_builder.h
//! \brief Vire resource builder for the resource manager
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_RESOURCE_DEVICES_TO_RESOURCES_BUILDER_H
#define VIRE_RESOURCE_DEVICES_TO_RESOURCES_BUILDER_H

// Third party:
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/properties.h>

// This project:
#include <vire/resource/base_resource_builder.h>

namespace vire {

  namespace device {
    class manager;
    // class mapping;
    class instance_tree;
  }

  namespace resource {

    class manager;

    //! \brief Builder of resources from a device dictionary
    class devices_to_resources_builder
      : public base_resource_builder
    {
    public:

      //! Default constructor
      devices_to_resources_builder();

      //! Destructor
      ~devices_to_resources_builder();

      //! Check if the device manager is set
      bool has_device_manager() const;

      //! Set the device manager
      void set_device_manager(const ::vire::device::manager & dev_mgr_);

      //! Return the device manager
      const ::vire::device::manager & get_device_manager() const;

      //! Check initialization status
      bool is_initialized() const;

      //! Initialize
      void initialize(const datatools::properties & config_);

      //! Reset
      void reset();

      //! Build the resources from the device manager and add them in the resource manager
      void build_resources();

    private:

      // Management:
      bool _initialized_; //!< Initialization flag

      // Data:
      const ::vire::device::manager * _device_manager_;    //!< Handle to the device manager
      // const ::vire::device::mapping * _device_mapping_;    //!< Device mapping
      // const ::vire::device::instance_tree * _device_tree_; //!< Device instance tree

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace resource

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::resource::devices_to_resources_builder);
#endif // Q_MOC_RUN

#endif // VIRE_RESOURCE_DEVICES_TO_RESOURCES_BUILDER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
