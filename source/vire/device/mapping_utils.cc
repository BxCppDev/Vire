//! \file vire/device/mapping_utils.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/mapping_utils.h>

// Standard library:
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>

namespace vire {

  namespace device {

    // static
    const std::string & mapping_utils::mapping_prefix()
    {
      static std::string prefix = "mapping.";
      return prefix;
    }

    const std::string & mapping_utils::mapping_category_property()
    {
      static std::string property = "category";
      return property;
    }

    const std::string & mapping_utils::mapping_address_property()
    {
      static std::string property = "address";
      return property;
    }

    const std::string & mapping_utils::mapping_id_property()
    {
      static std::string property = "id";
      return property;
    }

    const std::string & mapping_utils::mapping_embedded_id_property()
    {
      static std::string property = "embedded_id";
      return property;
    }

    void mapping_utils::extract(const datatools::properties & source_,
                                datatools::properties & target_)
    {
      source_.export_starting_with(target_, mapping_utils::mapping_prefix());
      return;
    }

    std::string mapping_utils::make_key(const std::string & key_)
    {
      std::ostringstream key_oss;
      key_oss << mapping_utils::mapping_prefix() << key_;
      return key_oss.str();
    }

    bool mapping_utils::has_key(const datatools::properties & config_,
                                const std::string & key_)
    {
      return (config_.has_key(mapping_utils::make_key(key_)));
    }

    bool mapping_utils::has_flag(const datatools::properties & config_,
                                 const std::string & key_)
    {
      return (config_.has_flag(mapping_utils::make_key(key_)));
    }

    bool mapping_utils::has_id(const datatools::properties & config_)
    {
      return (config_.has_key(mapping_utils::make_key(mapping_utils::mapping_id_property())));
    }

    void mapping_utils::fetch_id(const datatools::properties & config_,
                                 std::string & id_info_)
    {
      config_.fetch(mapping_utils::make_key(mapping_utils::mapping_id_property()), id_info_);
      return;
    }

    bool mapping_utils::has_embedded_id(const datatools::properties & config_,
                                        const std::string & embedded_name_)
    {
      std::ostringstream embedded_id_name;
      embedded_id_name << mapping_utils::make_key(mapping_utils::mapping_embedded_id_property())
                       << "." << embedded_name_;
      return (config_.has_key(embedded_id_name.str()));
    }

    void mapping_utils::fetch_embedded_id(const datatools::properties & config_,
                                          const std::string & embedded_name_,
                                          std::string & id_info_)
    {
      std::ostringstream embedded_id_name;
      embedded_id_name << mapping_utils::make_key(mapping_utils::mapping_embedded_id_property())
                       << "." << embedded_name_;
      config_.fetch(embedded_id_name.str(), id_info_);
      return;
    }

  } // end of namespace device

} // end of namespace vire
