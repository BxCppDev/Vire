//! \file vire/cmsserver/uc_error.cc
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

// Ourselves:
#include <vire/cmsserver/uc_error.h>

namespace vire {

  namespace cmsserver {

    uc_error::uc_error(const std::string & what_)
      : std::exception()
      , _what_(what_)
    {
      return;
    }

    // virtual
    const char * uc_error::what() const noexcept
    {
      return _what_.c_str();
    }

    // virtual
    void uc_error::export_error_data(boost::property_tree::ptree & error_data_) const
    {
      error_data_.put("what", std::string(this->what()));
      return;
    }
 
    uc_run_system_error::uc_run_system_error(const std::string & what_)
      : uc_error(what_)
    {
      return;
    }
 
    uc_run_work_error::uc_run_work_error(const std::string & what_)
      : uc_error(what_)
    {
      return;
    }
  
  } // namespace cmsserver

} // namespace vire
