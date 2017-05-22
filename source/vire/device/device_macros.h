//! \file  vire/device/device_macros.h
//! \brief Vire device macros
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

#ifndef VIRE_DEVICE_DEVICE_MACROS_H
#define VIRE_DEVICE_DEVICE_MACROS_H

// Device model registration:

#define VIRE_DEVICE_REGISTRATION_INTERFACE(DeviceClassName)             \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::device::base_device_model, DeviceClassName) \
  /**/

#define VIRE_DEVICE_REGISTRATION_IMPLEMENT(DeviceClassName,DeviceId)    \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(::vire::device::base_device_model,DeviceClassName,DeviceId) \
  /**/

// Port model registration:

#define VIRE_DEVICE_PORT_REGISTRATION_INTERFACE(DevicePortClassName)    \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::device::base_port_model, DevicePortClassName) \
  /**/

#define VIRE_DEVICE_PORT_REGISTRATION_IMPLEMENT(DevicePortClassName,DevicePortId) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(::vire::device::base_port_model,DevicePortClassName,DevicePortId) \
  /**/

// Link model registration:

#define VIRE_DEVICE_LINK_REGISTRATION_INTERFACE(DeviceLinkClassName)    \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::device::base_link_model, DeviceLinkClassName) \
  /**/

#define VIRE_DEVICE_LINK_REGISTRATION_IMPLEMENT(DeviceLinkClassName,DeviceLinkId) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(::vire::device::base_link_model,DeviceLinkClassName,DeviceLinkId) \
  /**/

#endif // VIRE_DEVICE_DEVICE_MACROS_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
