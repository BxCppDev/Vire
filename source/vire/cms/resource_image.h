//! \file  vire/cms/resource_image.h
//! \brief Resource image
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

#ifndef VIRE_CMS_RESOURCE_IMAGE_H
#define VIRE_CMS_RESOURCE_IMAGE_H

// Standard Library:
#include <string>

// This project:
#include <vire/cms/base_image.h>
#include <vire/resource/resource.h>
#include <vire/cms/monitoring.h>

namespace vire {

  namespace cms {

    //! \brief Resource image
    class resource_image
      : public base_image
    {
    public:

      //! Constructor
      resource_image(const vire::resource::resource & resource_);

      //! Destructor
      virtual ~resource_image();

      //! Reset
      void reset() override;

      /// Return the path
      const std::string & get_path() const override;

      /// Check if image is a resource
      bool is_resource() const override;

      //! Check if image can store a value
      bool can_store_value() const override;

      //! Return the resource description
      const vire::resource::resource & get_resource() const;

      /*
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
      */

      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;
     
    private :

      const vire::resource::resource * _res_ = nullptr; ///< Handle to resource description
      
    };
    
  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_RESOURCE_IMAGE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
