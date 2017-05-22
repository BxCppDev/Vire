//! \file  vire/utility/payload_macros.h
//! \brief Useful macros associated to payload objects
//
// Copyright (c) 2016 by Jean Hommet <hommet@lpccaen.in2p3.fr>,
//                       François Mauger <mauger@lpccaen.in2p3.fr>,
//                       Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_PAYLOAD_MACROS_H
#define VIRE_UTILITY_PAYLOAD_MACROS_H

#define VIRE_UTILITY_PAYLOAD_REGISTRATION_INTERFACE(PayloadClass)       \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::utility::base_payload, PayloadClass) \
  public:                                                               \
  virtual std::string payload_class_id() const;                         \
  /**/

#define VIRE_UTILITY_PAYLOAD_REGISTRATION_IMPLEMENTATION(PayloadClass,PayloadClassId) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(::vire::utility::base_payload,PayloadClass,PayloadClassId) \
  std::string PayloadClass::payload_class_id() const {return PayloadClassId;} \
  /**/

#define VIRE_UTILITY_PAYLOAD_INTERFACE(PayloadClass)            \
  DATATOOLS_SERIALIZATION_DECLARATION()                         \
  DATATOOLS_CLONEABLE_DECLARATION(PayloadClass)                 \
  VIRE_UTILITY_PAYLOAD_REGISTRATION_INTERFACE(PayloadClass)     \
  /**/

#define VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(PayloadClass,PayloadClassId) \
  DATATOOLS_SERIALIZATION_IMPLEMENTATION(PayloadClass, PayloadClassId)  \
  DATATOOLS_CLONEABLE_IMPLEMENTATION(PayloadClass)                      \
  VIRE_UTILITY_PAYLOAD_REGISTRATION_IMPLEMENTATION(PayloadClass, PayloadClassId) \
  /**/

#endif // VIRE_UTILITY_PAYLOAD_MACROS_H

// Local Variables: --
// Mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
