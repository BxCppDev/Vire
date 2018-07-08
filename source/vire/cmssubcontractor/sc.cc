//! \file vire/cmssubcontractor/sc.cc
//
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmssubcontractor/sc.h>


namespace vire {

  namespace cmssubcontractor {

    sc::sc()
    {
      return;
    }
    
    sc::~sc()
    {
      return;
    }

    void sc::set_description(const sc_description & desc_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Subcontractor is initialized and locked!");
      _desc_ = desc_;
      return;
    }

    const sc_description & sc::get_description() const
    {
      return desc_;
    }

    bool sc::is_initialized() const
    {
      return _initialized_;
    }

  } // namespace cmssubcontractor

} // namespace vire

