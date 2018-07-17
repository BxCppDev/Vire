//! \file vire/com/addressing.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

// Ourselves:
#include <vire/com/addressing.h>

// Third party:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>

namespace vire {

  namespace com {
 
    std::string to_string(const address_category_type category_)
    {
      switch(category_) {
      case ADDR_CATEGORY_PROTOCOL: return "protocol";
      case ADDR_CATEGORY_DEVICE:   return "device";
      case ADDR_CATEGORY_RESOURCE: return "resource";
      default:
        break;
      }
      return "";
    }

    bool from_string(const std::string & label_, address_category_type & category_)
    {
      category_ = ADDR_CATEGORY_INVALID;
      if (label_ == to_string(ADDR_CATEGORY_PROTOCOL)) {
        category_ = ADDR_CATEGORY_PROTOCOL;
      }
      if (label_ == to_string(ADDR_CATEGORY_DEVICE)) {
        category_ = ADDR_CATEGORY_DEVICE;
      }
      if (label_ == to_string(ADDR_CATEGORY_RESOURCE)) {
        category_ = ADDR_CATEGORY_RESOURCE;
      }
      return category_ != ADDR_CATEGORY_INVALID;
    }
    
    address::address(const address_category_type category_, const std::string & value_)
    {   
      _category_ = category_;
      switch (_category_) {
      case ADDR_CATEGORY_PROTOCOL:
        DT_THROW_IF(!datatools::name_validation(value_,
                                                datatools::NV_INSTANCE | datatools::NV_NO_DOT),
                    std::logic_error,
                    "Protocol value '" << value_ << "' is not valid!");
        _value_ = value_;
        break;
      case ADDR_CATEGORY_DEVICE:
      case ADDR_CATEGORY_RESOURCE:
        DT_THROW_IF(!_cached_path_.from_string(value_),
                    std::logic_error,
                    "Device/resource path representation '" << value_ << "' is not valid!");
        _value_ = value_;
      }
      return;
    }

    bool address::is_complete() const
    {
      if (_category_ == ADDR_CATEGORY_INVALID) {
        return false;
      }
      if (_value_.empty()) {
        return false;
      }     
      return true;
    }
 
    bool address::is_protocol() const
    {
      return _category_ == ADDR_CATEGORY_PROTOCOL;
    }

    bool address::is_device() const
    {
      return _category_ == ADDR_CATEGORY_DEVICE;
    }

    bool address::is_resource() const
    {
      return _category_ == ADDR_CATEGORY_RESOURCE;
    }

    const std::string & address::get_value() const
    {
      return _value_;
    }

    bool address::has_path() const
    {
      return _cached_path_.is_valid();
    }

    const vire::utility::path & address::get_path() const
    {
      DT_THROW_IF(!has_path(), std::logic_error, "Address has no path!");
      return _cached_path_;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const address & addr_)
    {
      if (addr_.is_complete()) {
        out_ << '[' << to_string(addr_._category_) << ':' << addr_._value_ << ']';
      } else {
        out_ << "[?:?]";
      }
      return out_;
    }

  } // namespace com

} // namespace vire
