//! \file  vire/device/instance_to_mapping.h
//! \brief Algorithm that build mapping rule from instance objects
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_DEVICE_INSTANCE_TO_MAPPING_H
#define VIRE_DEVICE_INSTANCE_TO_MAPPING_H

// Standard library:
#include <string>

// Third party:
// - Bayeux/geomtools:
#include <geomtools/id_mgr.h>

// This project:
#include <vire/device/i_instance.h>

namespace vire {

  namespace device {

    class instance_to_mapping
    {
    public:

      //! Default constructor
      instance_to_mapping();

      //! Destructor
      virtual ~instance_to_mapping();

      //! Check validity
      bool is_valid() const;

      //! Reset
      void reset();

      void set_id_manager(const geomtools::id_mgr &);

      void set_category(const std::string &);

      bool build(const i_instance & instance_, std::vector<std::string> & mapping_rules_);

    private:

      void _sync_();

    private:

      // Configuration:
      const geomtools::id_mgr * _id_mgr_ = nullptr;
      std::string               _category_;

      // Working data:
      const geomtools::id_mgr::category_info * _catinfo_ = nullptr;

    };

  } // namespace device

} // namespace vire

#endif // VIRE_DEVICE_INSTANCE_TO_MAPPING_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
