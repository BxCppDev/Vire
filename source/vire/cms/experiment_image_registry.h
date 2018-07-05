//! \file  vire/cms/experiment_image_registry.h
//! \brief Registry of resource images
//
// Copyright (c) 2016-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <memory>
#include <tuple>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/properties.h>
#include <datatools/bit_mask.h>

// This project:
#include <vire/cms/base_image.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>

namespace vire {

  //! Nested namespace for the Vire library's resource module
  namespace cms {

    class manager;
    class resource_image;
    class device_image;

    //! \brief Registry of resource/device images
    class experiment_image_registry
      : public ::datatools::base_service
    {
    public:

      /// \brief Init flags
      enum class init_flags {
        INIT_DUMMY = datatools::bit_mask::bit00 //!< Dummy flag
      };

      typedef std::shared_ptr<base_image>           image_ptr_type;
      typedef std::map<std::string, image_ptr_type> image_dict_type;
      
      static const std::string & default_service_name();
      
      //! Default constructor
      experiment_image_registry(uint32_t flags_ = 0);

      //! Destructor
      virtual ~experiment_image_registry();

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

      bool get_with_devices() const; 

      void set_with_devices(const bool wd_);

      bool get_only_writable_resources() const;

      void set_only_writable_resources(const bool owr_);
 
      //! Check the initialization flag
      virtual bool is_initialized() const;

      //! Initialize the manager from a container of properties
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the manager
      virtual int reset();

      //! Check if an image with given path exists
      bool has(const std::string & path_) const;

      //! Check if a device image with given path exists
      bool has_device(const std::string & path_) const;

      //! Check if a resource image with given path exists
      bool has_resource(const std::string & path_) const;

      //! Return a handle to a const image
      const base_image & get_image(const std::string & path_) const;

      //! Return a handle to a mutable image
      base_image & grab_image(const std::string & path_);
 
      //! Return a handle to a const resource image
      const resource_image & get_resource_image(const std::string & path_) const;

      //! Return a handle to a mutableresource  image
      resource_image & grab_resource_image(const std::string & path_);
      
      // //! Add the image associated to a device
      // void add_device_image(const std::string & path_);
      
      //! Add the image associated to a resource
      std::tuple<bool, std::string> add_resource_image(const std::string & path_);
    
      const image_dict_type & get_images() const;
      
      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

    protected:

      void _set_defaults();

      base_image & _grab_image_(const std::string & path_);

      // void _build_images();

      // void _insert_resource_image(const vire::resource::resource &);

      // void _insert_device_image(const std::string & path_,
      //                           const vire::device::logical_device &);

    private:

      // Management:
      bool _initialized_ = false;          //!< Initialization flag

      // Configuration parameters:
      std::string _device_service_name_;   //!< Device manager name
      std::string _resource_service_name_; //!< Resource manager name
      const vire::device::manager   * _device_manager_   = nullptr; //!< Handle to the device manager
      const vire::resource::manager * _resource_manager_ = nullptr; //!< Handle to the resource manager
      bool _with_devices_            = false;
      bool _only_writable_resources_ = false;         

      // Working data:
      image_dict_type _images_; //!< Dictionary of resource images

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
