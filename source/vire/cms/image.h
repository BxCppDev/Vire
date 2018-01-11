//! \file  vire/cms/image.h
//! \brief Image of the current/dynamic status of a device or resource
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

#ifndef VIRE_CMS_IMAGE_H
#define VIRE_CMS_IMAGE_H

// Standard Library:
#include <string>
#include <set>
#include <vector>
#include <map>

// Third Party:
// - Boost:
#include <boost/logic/tribool.hpp>
// #include <boost/optional.hpp>
#include <boost/variant.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// This project:
#include <vire/device/logical_device.h>
#include <vire/resource/resource.h>
#include <vire/cms/status.h>
#include <vire/cms/image_status.h>
#include <vire/cms/monitoring.h>
#include <vire/cms/resource_status_record.h>
#include <vire/time/utils.h>

namespace vire {

  namespace cms {

    //! \brief Dynamic status of a device or resource
    class image
      : public datatools::i_tree_dumpable
    {
    public:

      struct base_info
      {
        std::map<std::string, std::string> metadata;
      };

      struct resource_info : public base_info
      {
        void reset();
        bool is_valid() const;
        // Attributes:
        const vire::resource::resource * res = nullptr; ///< Handle to resource description
      };

      struct device_info : public base_info
      {
        void reset();
        bool is_valid() const;
        // Attributes:
        std::string path;
        const vire::device::logical_device * log = nullptr; ///< Handle to device description
      };

      //! Default constructor
      image();

      //! Constructor for a resource
      image(const vire::resource::resource &);

      //! Constructor for a device
      image(const std::string & device_path_,
            const vire::device::logical_device & device_log_);

      //! Destructor
      virtual ~image();

      //! Check the validity of the resource image
      bool is_valid() const;

      /// Check if the path is defined
      bool has_path() const;

      /// Return the path
      const std::string & get_path() const;

      /// Return the leaf name
      const std::string & get_leaf_name() const;

      /// Check if image is a device
      bool is_device() const;

      /// Check if image is a resource
      bool is_resource() const;

      //! Reset the device handler
      void reset_device();

      //! Reset the resource handler
      void reset_resource();

      //! Return the reference to the device handler
      const vire::device::logical_device & get_log_device() const;

      //! Return the reference to the resource handler
      const vire::resource::resource & get_resource() const;

      //! Reset the resource image
      void reset();

      //! Return the non mutable status
      const image_status & get_status() const;

      //! Return the mutable status
      image_status & grab_status();

      //! Apply a resource status record
      void update(const resource_status_record & record_);

      // //! Make all status bits indeterminate
      // void indeterminate_status();

      //! Check if image can store a value
      bool can_value() const;

      //! Check if value is set
      bool has_value(std::size_t index_ = 0) const;

      //! Reset the value
      void reset_value(std::size_t index_ = 0);

      //! Reset the values
      void reset_values();

      //! Return the size of the value
      std::size_t get_values_size() const;

      //! Return the number of the set value
      std::size_t get_number_of_set_values() const;

      //! Return the boolean value
      bool get_boolean_value(std::size_t index_ = 0) const;

      //! Set the boolean value
      void set_boolean_value(const bool value_, std::size_t index_ = 0);

      //! Return the integer value
      int32_t get_integer_value(std::size_t index_ = 0) const;

      //! Return the real value
      double get_real_value(std::size_t index_ = 0) const;

      //! Return the string value
      const std::string & get_string_value(std::size_t index_ = 0) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_ = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      /// Set the leaf name
      void _set_leaf_name_(const std::string &);

    private:

      // Description:
      device_info        _device_;           //!< Device description
      resource_info      _resource_;         //!< Resource description

      // Internal data:
      std::string           _leaf_name_; //!< Leaf name

      std::list<value_monitoring_record> _record_; //! Current array of value monitoring records

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace cms

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::cms::image);
#endif // Q_MOC_RUN

#endif // VIRE_CMS_IMAGE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
