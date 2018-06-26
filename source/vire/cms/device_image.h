//! \file  vire/cms/device_image.h
//! \brief Device image
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_CMS_DEVICE_IMAGE_H
#define VIRE_CMS_DEVICE_IMAGE_H

// Standard Library:
#include <string>

// This project:
#include <vire/cms/base_image.h>
#include <vire/device/logical_device.h>

namespace vire {

  namespace cms {

    //! \brief Device image
    class device_image
      : public base_image
    {
    public:
      
      //! Constructor
      device_image(const std::string & path_,
                   const vire::device::logical_device & logical_);

      //! Destructor
      virtual ~device_image();

      //! Reset the image
      void reset() override;
 
      /// Check if image is a device
      bool is_device() const override;

      /// Check if image is a resource
      bool is_resource() const override;

      //! Check if image can store a value
      bool can_store_value() const override;

      //! Return the device description
      const vire::device::logical_device & get_logical() const;

      const std::string & get_path() const override;

      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

    private :
      
      // Attributes:
      std::string _path_;
      const vire::device::logical_device * _logical_ = nullptr; ///< Handle to device description

    };

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_DEVICE_IMAGE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
