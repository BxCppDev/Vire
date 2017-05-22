//! \file opcua_ast-reflect.h
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

#ifndef VIRE_MOS_OPCUA_AST_REFLECT_H
#define VIRE_MOS_OPCUA_AST_REFLECT_H

// Ourselves:
#include <vire/mos/opcua_ast.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

DR_TYPE_EXPORT(::vire::mos::Type, "vire::mos::Type");
DR_TYPE_EXPORT(::vire::mos::ScopeAccess, "vire::mos::ScopeAccess");
DR_TYPE_EXPORT(::vire::mos::Access, "vire::mos::Access");
DR_TYPE_EXPORT(::vire::mos::Interface, "vire::mos::Interface");
DR_TYPE_EXPORT(::vire::mos::Validity, "vire::mos::Validity");
DR_TYPE_EXPORT(::vire::mos::MonitoringRate, "vire::mos::MonitoringRate");
DR_TYPE_EXPORT(::vire::mos::Connection, "vire::mos::Connection");
DR_TYPE_EXPORT(::vire::mos::StateFSM, "vire::mos::StateFSM");
DR_TYPE_EXPORT(::vire::mos::NameFSM, "vire::mos::NameFSM");
DR_TYPE_EXPORT(::vire::mos::NameHardwareConfig, "vire::mos::NameHardwareConfig");
DR_TYPE_EXPORT(::vire::mos::NameAlarm, "vire::mos::NameAlarm");

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::Type, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::Type)
          .value("invalid",::vire::mos::TYPE_INVALID)
          .value("bool",   ::vire::mos::TYPE_BOOL)
          .value("int8",   ::vire::mos::TYPE_INT8)
          .value("int16",  ::vire::mos::TYPE_INT16)
          .value("int32",  ::vire::mos::TYPE_INT32)
          .value("int64",  ::vire::mos::TYPE_INT64)
          .value("float",  ::vire::mos::TYPE_FLOAT)
          .value("double", ::vire::mos::TYPE_DOUBLE)
          .value("string", ::vire::mos::TYPE_STRING)
          .value("xml",        ::vire::mos::TYPE_XML)
          .value("byteString", ::vire::mos::TYPE_BYTESTRING)
         ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::ScopeAccess, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::ScopeAccess)
          .value("invalid",   ::vire::mos::SCOPE_ACCESS_INVALID)
          .value("internal",  ::vire::mos::SCOPE_ACCESS_INTERNAL)
          .value("protected", ::vire::mos::SCOPE_ACCESS_PROTECTED)
          .value("external",  ::vire::mos::SCOPE_ACCESS_EXTERNAL)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::Access, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::Access)
          .value("invalid",  ::vire::mos::ACCESS_INVALID)
          .value("Input",  ::vire::mos::ACCESS_INPUT)
          .value("Output", ::vire::mos::ACCESS_OUTPUT)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::Interface, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::Interface)
          .value("gpib",   ::vire::mos::INTERFACE_GPIB)
          .value("udp",    ::vire::mos::INTERFACE_UDP)
          .value("tcp",    ::vire::mos::INTERFACE_TCP)
          .value("com",    ::vire::mos::INTERFACE_COM)
          .value("pci",    ::vire::mos::INTERFACE_PCI)
          .value("usb",    ::vire::mos::INTERFACE_USB)
          .value("serial", ::vire::mos::INTERFACE_SERIAL)
          .value("OPCUA_PLC_Emulation", ::vire::mos::INTERFACE_OPCUA_PLC_EMULATION)
         ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::Validity, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::Validity)
          .value("permanent",  ::vire::mos::VALIDITY_PERMANENT)
          .value("temporary",  ::vire::mos::VALIDITY_TEMPORARY)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::MonitoringRate, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::MonitoringRate)
          .value("1",    ::vire::mos::MONRATE_1)
          .value("2",    ::vire::mos::MONRATE_2)
          .value("5",    ::vire::mos::MONRATE_5)
          .value("10",   ::vire::mos::MONRATE_10)
          .value("30",   ::vire::mos::MONRATE_30)
          .value("60",   ::vire::mos::MONRATE_60)
          .value("900",  ::vire::mos::MONRATE_900)
          .value("1800", ::vire::mos::MONRATE_1800)
          .value("3600", ::vire::mos::MONRATE_3600)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::Connection, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::Connection)
          .value("client",  ::vire::mos::CONNECTION_CLIENT)
          .value("server",  ::vire::mos::CONNECTION_SERVER)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::StateFSM, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::StateFSM)
          .value("0",  ::vire::mos::STATE_FSM_NOT_READY)
          .value("1",  ::vire::mos::STATE_FSM_READY)
          .value("2",  ::vire::mos::STATE_FSM_RUNNING)
          .value("3",  ::vire::mos::STATE_FSM_PAUSING)
          .value("4",  ::vire::mos::STATE_FSM_SAFE)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::NameFSM, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::NameFSM)
          .value("Configure", ::vire::mos::NAME_FSM_CONFIGURE)
          .value("Start",     ::vire::mos::NAME_FSM_START)
          .value("Stop",      ::vire::mos::NAME_FSM_STOP)
          .value("Pause",     ::vire::mos::NAME_FSM_PAUSE)
          .value("Resume",    ::vire::mos::NAME_FSM_RESUME)
          .value("Reset",     ::vire::mos::NAME_FSM_RESET)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::NameHardwareConfig, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::NameHardwareConfig)
          .value("Init",  ::vire::mos::NAME_HW_CONFIG_INIT)
          .value("Close", ::vire::mos::NAME_HW_CONFIG_CLOSE)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::NameAlarm, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::NameAlarm)
          .value("clearAlarm",      ::vire::mos::NAME_ALARM_CLEAR_ALARM)
          .value("troubleShooting", ::vire::mos::NAME_ALARM_TROUBLE_SHOOTING)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::mos::NameSpaceLevel, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::mos::NameSpaceLevel)
          .value("User",   ::vire::mos::NSL_USER)
          .value("Expert", ::vire::mos::NSL_EXPERT)
          .value("Debug",  ::vire::mos::NSL_DEBUG)
          .value("System", ::vire::mos::NSL_SYSTEM)
          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_MOS_OPCUA_AST_REFLECT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
