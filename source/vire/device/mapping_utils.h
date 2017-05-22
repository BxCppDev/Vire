//! \file  vire/device/manager.h
//! \brief Vire device manager
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

#ifndef VIRE_DEVICE_MAPPING_UTILS_H
#define VIRE_DEVICE_MAPPING_UTILS_H

// Standard library:
#include <string>

namespace datatools {

  // Forward class declarations:
  class properties;
}

namespace vire {

  namespace device {

    //! \brief A collection of mapping utilities
    struct mapping_utils
    {
    public:

      //! Return the property key prefix for mapping
      static const std::string & mapping_prefix();

      static const std::string & mapping_category_property();

      static const std::string & mapping_address_property();

      static const std::string & mapping_id_property();

      static const std::string & mapping_embedded_id_property();

      static void extract(const datatools::properties & source_,
                          datatools::properties & target_);

      static std::string make_key(const std::string & key_);

      static bool has_key(const datatools::properties & config_,
                          const std::string & key_);

      static bool has_flag(const datatools::properties & config_,
                           const std::string & key_);

      static bool has_id(const datatools::properties & config_);

      static void fetch_id(const datatools::properties & config_,
                           std::string & id_info_);

      static bool has_embedded_id(const datatools::properties & config_,
                                  const std::string & embedded_name_);

      static void fetch_embedded_id(const datatools::properties & config_,
                                    const std::string & embedded_name_,
                                    std::string & id_info_);
    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_MAPPING_UTILS_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
