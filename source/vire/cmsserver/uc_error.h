//! \file  vire/cmsserver/uc_error.h
//! \brief Vire CMS error handling for use case
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

#ifndef VIRE_CMSSERVER_UC_ERROR_H
#define VIRE_CMSSERVER_UC_ERROR_H

// Standard Library:
#include <exception>

// - Boost:
#include <boost/property_tree/ptree.hpp>

namespace vire {

  namespace cmsserver {

      struct uc_error
        : public std::exception
      {
        explicit uc_error(const std::string & what_ = "");
        virtual void export_error_data(boost::property_tree::ptree & error_data_) const;
        virtual const char * what() const noexcept;
      private:
        std::string _what_;
      };

      struct uc_run_system_error
        : public uc_error
      {
        explicit uc_run_system_error(const std::string & what_);
      };

      struct uc_run_work_error
        : public uc_error
      {
        explicit uc_run_work_error(const std::string & what_);
      };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
