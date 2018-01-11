//! \file  vire/cmsserver/detail/use_case_macros.h
//! \brief Macros for cmsserver use case classes
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

#ifndef VIRE_CMSSERVER_DETAIL_USE_CASE_MACROS_H
#define VIRE_CMSSERVER_DETAIL_USE_CASE_MACROS_H

// Standard Library:
#include <string>

// This project:
// - Bayeux/datatools:
#include <datatools/factory_macros.h>

#define VIRE_CMSSERVER_USE_CASE_REGISTRATION_INTERFACE(USE_CASE_CLASS_NAME) \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::cmsserver::base_use_case, USE_CASE_CLASS_NAME) \
  public:                                                               \
  virtual std::string use_case_class_id() const;                        \
  /**/

#define VIRE_CMSSERVER_USE_CASE_REGISTRATION_IMPLEMENT(USE_CASE_CLASS_NAME,USE_CASE_ID) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION (::vire::cmsserver::base_use_case, USE_CASE_CLASS_NAME, USE_CASE_ID) \
  std::string USE_CASE_CLASS_NAME::use_case_class_id() const {return USE_CASE_ID;} \
  /**/

#endif // VIRE_CMSSERVER_DETAIL_USE_CASE_MACROS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
