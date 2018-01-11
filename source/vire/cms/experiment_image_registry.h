//! \file  vire/cms/experiment_image_registry.h
//! \brief Registry of resource images
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMS_EXPERIMENT_IMAGE_REGISTRY_H
#define VIRE_CMS_EXPERIMENT_IMAGE_REGISTRY_H

// Standard Library:
#include <string>
#include <map>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/properties.h>
#include <datatools/bit_mask.h>

// This project:
#include <vire/cms/image.h>

namespace vire {

  //! Nested namespace for the Vire library's resource module
  namespace cms {

    class manager;

    //! \brief Registry of resource images
    class experiment_image_registry
      : public ::datatools::base_service
    {
    public:

      /// \brief Init flags
      enum class init_flags {
        INIT_DUMMY = datatools::bit_mask::bit00 //!< Dummy flag
      };

      //! Default constructor
      experiment_image_registry(uint32_t flags_ = 0);

      //! Destructor
      virtual ~experiment_image_registry();

      //! Check if an image with given path exists
      bool has(const std::string & path_) const;

      //! Check if a device image with given path exists
      bool has_device(const std::string & path_) const;

      //! Check if a resource image with given path exists
      bool has_resource(const std::string & path_) const;

      //! Return a handle to a const image
      const image & get_image(const std::string & path_) const;

      //! Return a handle to a mutable image
      image & grab_image(const std::string & path_);

      //! Check if the name of the device service is set
      bool has_device_service_name() const;

      //! Return the name of the device service
      const std::string & get_device_service_name() const;

      //! Set the name of the device service
      void set_device_service_name(const std::string &);

      //! Check if the name of the resource service is set
      bool has_resource_service_name() const;

      //! Return the name of the resource service
      const std::string & get_resource_service_name() const;

      //! Set the name of the resource service
      void set_resource_service_name(const std::string &);

      //! Check if the device manager is set
      bool has_device_manager() const;

      //! Set the handle to the device manager
      void set_device_manager(const vire::device::manager & mgr_);

      //! Return the handle to the device manager
      const vire::device::manager & get_device_manager() const;

      //! Check if the resource manager is set
      bool has_resource_manager() const;

      //! Set the handle to the resource manager
      void set_resource_manager(const vire::resource::manager & mgr_);

      //! Return the handle to the resource manager
      const vire::resource::manager & get_resource_manager() const;

      //! Check the initialization flag
      virtual bool is_initialized() const;

      //! Initialize the manager from a container of properties
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the manager
      virtual int reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected:

      void _set_defaults();

      const image * _get_image_ptr(const std::string & path_) const;

      void _build_images();

      void _insert_resource_image(const vire::resource::resource &);

      void _insert_device_image(const std::string & path_,
                                const vire::device::logical_device &);

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration parameters:
      std::string _device_service_name_;   //!< Device manager name
      std::string _resource_service_name_; //!< Resource manager name

      // Working data:
      const vire::device::manager   * _device_manager_   = nullptr; //!< Handle to the device manager
      const vire::resource::manager * _resource_manager_ = nullptr; //!< Handle to the resource manager
      std::map<std::string,image>     _images_;                     //!< Dictionary of resource images

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(experiment_image_registry);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace cms

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::cms::experiment_image_registry);
#endif // Q_MOC_RUN

#endif // VIRE_CMS_EXPERIMENT_IMAGE_REGISTRY_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
