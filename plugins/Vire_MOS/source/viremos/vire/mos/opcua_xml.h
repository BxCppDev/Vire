//! \file opcua_xml.h
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

#ifndef VIRE_MOS_OPCUA_XML
#define VIRE_MOS_OPCUA_XML

// Standard library:
#include <string>
#include <vector>
#include <iostream>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/optional.hpp>

// http://stackoverflow.com/questions/4821477/xml-schema-minoccurs-maxoccurs-default-values
//

namespace vire {

  //! Multipurpose OPCUA Server (MOS) namespace
  namespace mos {

    typedef unsigned long int hexBinary;

    struct HexaValue {
      hexBinary bits;
    };

    enum Type {
      TYPE_BOOL,
      TYPE_INT8,
      TYPE_INT16,
      TYPE_INT32,
      TYPE_INT64,
      TYPE_FLOAT,
      TYPE_DOUBLE,
      TYPE_STRING,
      TYPE_XML
    };

    typedef std::string DefaultValue;
    typedef std::string Value;
    typedef bool        Enable;
    typedef std::string Name;
    typedef int         Severity;
    typedef std::string Message;
    typedef int         Multiplicity;
    typedef std::string DataFrameStructureRef;
    typedef int         Pos;
    typedef int         Nb;
    typedef int         Index;
    typedef std::string Id;
    typedef std::string ReadIdCollector;
    typedef std::string Description;
    typedef bool        Historizing;
    typedef int         AccesLevel;
    typedef int         WriteMask;
    typedef HexaValue   EndDelimiter;
    typedef HexaValue   Size;
    typedef std::string FileName;
    typedef HexaValue   IdNum;
    typedef std::string IpAddress;
    typedef bool        Asynchronous;
    typedef bool        Acknowledge;
    typedef int         ArraySize;
    typedef std::string Address;
    typedef std::string FileMapping;
    typedef std::string Min;
    typedef std::string Max;
    typedef int         ServerPort;
    typedef int         Port;
    typedef bool        Config;
    typedef bool        ICD;
    typedef std::string NodeId;
    typedef std::string MethodId;
    typedef std::string ServerId;
    typedef std::string DataIdDescriptorRef;
    typedef std::string Cmd;

    struct Instruction {
      Name                          name;
      boost::optional<Cmd>          cmd;
      boost::optional<Acknowledge>  acknowledge;
      boost::optional<Asynchronous> asynchronous;
    };

    struct Instruction_set {
      std::vector<Instruction>     instructions;
      boost::optional<Acknowledge> acknowledge;
    };

    struct Base_ErrorAlarm {
      Value                        value;
      boost::optional<Message>     message;
      boost::optional<Severity>    severity;
      boost::optional<Enable>      enable;
      boost::optional<Acknowledge> acknowledge;
    };

    struct Error_LimitHigh : public Base_ErrorAlarm {
    };

    struct Error_LimitLow : public Base_ErrorAlarm {
    };

    struct Error_Equal : public Base_ErrorAlarm {
    };

    struct Error_NotEqual : public Base_ErrorAlarm {
    };

    struct Alarm_LimitHigh : public Base_ErrorAlarm {
    };

    struct Alarm_LimitLow : public Base_ErrorAlarm {
    };

    struct Alarm_Equal : public Base_ErrorAlarm {
    };

    struct Alarm_NotEqual : public Base_ErrorAlarm {
    };

    struct Alarm {
      boost::optional<Alarm_LimitHigh> alarm_limit_high;
      boost::optional<Error_LimitHigh> error_limit_high;
      boost::optional<Alarm_LimitLow>  alarm_limit_low;
      boost::optional<Error_LimitLow>  error_limit_low;
      boost::optional<Alarm_Equal>     alarm_equal;
      boost::optional<Error_Equal>     error_equal;
      boost::optional<Alarm_NotEqual>  alarm_not_equal;
      boost::optional<Error_NotEqual>  error_not_equal;
    };

    struct ElementArray {
      Pos   pos;
      Value value;
    };

    struct Event {
      Message message;
    };

    struct Attribut {
      Name  name;
      Value value;
      void dump(std::ostream & out_) const
      {
        out_ << "Name  : '" << name << "'" << std::endl;
        out_ << "Value : '" << value << "'" << std::endl;
        return;
      }
    };

    struct Acces {

    };

    struct Variable {
      Name                         name;
      Type                         type;
      Multiplicity                 multiplicity;
      boost::optional<Value>       value;
      boost::optional<ArraySize>   array_size;
      std::vector<ElementArray>    elements;
      boost::optional<Description> description;
      boost::optional<Historizing> historizing;
      boost::optional<AccesLevel>  acces_level;
      boost::optional<WriteMask>   write_mask;
    };

    struct Range {
      Min min;
      Max max;
    };

    struct Info {
      boost::optional<Config> config;
      boost::optional<ICD> icd;
    };

    struct Sequence {
      NodeId node_id;
      MethodId methode_id;
      boost::optional<ServerId> server_id;
    };

    struct DeviceInstruction {
      Value value;
    };

    enum Access {
      ACCESS_INPUT,
      ACCESS_OUTPUT
    };

    struct Argument {
      Name  name;
      Value value;
      boost::optional<Access>       access;
      boost::optional<DefaultValue> default_value;
      boost::optional<Description>  description;
    };

    struct Header {
      Name name;
      Type type;
      boost::optional<Size> size;
      boost::optional<EndDelimiter> end_delimiter;
    };

    struct Id1 {
      Name name;
      Type type;
      boost::optional<Size> size;
      boost::optional<EndDelimiter> end_delimiter;
    };

    struct Id2 {
      Name name;
      Type type;
      boost::optional<Size> size;
      boost::optional<EndDelimiter> end_delimiter;
    };

    struct Data {
      Name name;
      Type type;
      boost::optional<Size> size;
      boost::optional<EndDelimiter> end_delimiter;
    };

    struct Footer {
      Name name;
      Type type;
      boost::optional<Size> size;
      boost::optional<EndDelimiter> enddelimiter;
    };

    struct DataFrameStructure {
      Header header;
      Id1    id1;
      Id2    id2;
      Data   data;
      Footer footer;
    };

    struct Frame_elementString {
      Name name;
      Type type;
      EndDelimiter end_delimiter;
    };

    struct Frame_element {
      Name  name;
      Type  type;
      Index index;
      boost::optional<Pos> pos;
      boost::optional<Nb>  nb;
    };

    typedef std::string Location;

    struct Plugins {
      Name     name;
      Location location;
    };

    struct Frame_definition {
      Name name;
      std::vector<Frame_elementString> frame_elementStrings;
      std::vector<Frame_element>       frame_elements;
    };

    enum Interface {
      INTERFACE_GPIB,
      INTERFACE_UDP,
      INTERFACE_TCP,
      INTERFACE_COM,
      INTERFACE_PCI,
      INTERFACE_USB,
      INTERFACE_SERIAL
    };

    enum Validity {
      VALIDITY_PERMANENT,
      VALIDITY_TEMPORARY
    };

    enum MonitoringRate {
      MONRATE_1 = 1,
      MONRATE_2 = 2,
      MONRATE_5 = 5,
      MONRATE_10 = 10,
      MONRATE_30 = 30,
      MONRATE_60 = 60,
      MONRATE_900 = 900,
      MONRATE_1800 = 1800,
      MONRATE_3600 = 3600
    };

    enum Connection {
      CONNECTION_CLIENT,
      CONNECTION_SERVER
    };

    enum StateFSM {
      STATE_FSM_NOT_READY = 0,
      STATE_FSM_READY     = 1,
      STATE_FSM_RUNNING   = 2,
      STATE_FSM_PAUSING   = 3,
      STATE_FSM_SAFE      = 4
    };

    enum NameFSM {
      NAME_FSM_CONFIGURE,
      NAME_FSM_START,
      NAME_FSM_STOP,
      NAME_FSM_PAUSE,
      NAME_FSM_RESUME,
      NAME_FSM_RESET
    };

    struct FSM {
      NameFSM                        name_fsm;
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
    };

    struct Start {
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
    };

    enum NameHardwareConfig {
      NAME_HW_CONFIG_INIT,
      NAME_HW_CONFIG_CLOSE
    };

    struct HardwareConfig {
      NameHardwareConfig             name_hardware_config;
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
      std::vector<EndDelimiter>      end_delimiters;
    };

    enum NameAlarm {
      NAME_ALARM_CLEAR_ALARM,
      NAME_ALARM_TROUBLE_SHOOTING
    };

    struct AlarmMethod {
      NameAlarm                      name_alarm;
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
    };

    struct Method {
      Name                           name;
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
      std::vector<EndDelimiter>      end_delimiters;
      std::vector<Event>             events;
      boost::optional<Info>          info;
    };

    struct BaseDatapoint {
      Name                          name;
      boost::optional<Multiplicity> multiplicity;
      std::vector<Attribut>         attributes;
    };

    struct SimpleDatapoint : public BaseDatapoint {
      Type                          type;
      boost::optional<Id>           id;
      boost::optional<AccesLevel>   acces_level;
      boost::optional<ArraySize>    array_size;
      std::vector<ElementArray>     elements;
      boost::optional<MonitoringRate> monitoring_rate;
      boost::optional<DataFrameStructureRef> dataframe_structure_ref;
      boost::optional<DefaultValue> default_value;
      boost::optional<Range>        range;
      boost::optional<Description>  description;
      boost::optional<Historizing>  historizing;
      boost::optional<WriteMask>    write_mask;
      boost::optional<EndDelimiter> end_delimiter;
      boost::optional<Alarm>        alarm;
      boost::optional<Info>         info;
      std::vector<AlarmMethod>      alarm_methods;
      std::vector<Variable>         variables;
      std::vector<Method>           methods;
      std::vector<Frame_element>    frame_elements;
      std::vector<Frame_elementString> frame_element_strings;
    };

    struct CompoundDatapoint : public BaseDatapoint {
      std::vector<SimpleDatapoint>   simple_datapoints;
      std::vector<CompoundDatapoint> compound_datapoints;
    };

    struct Frames_description {
      std::vector<SimpleDatapoint>   simple_datapoints;
      std::vector<CompoundDatapoint> compound_datapoints;
    };

    struct BaseDevice {
      Name                                   name;
      boost::optional<Multiplicity>          multiplicity;
      boost::optional<Interface>             interface;
      boost::optional<DataFrameStructureRef> dataframe_structure_ref;
      std::vector<HardwareConfig>            hardware_configs;
      std::vector<SimpleDatapoint>           simple_datapoints;
      std::vector<CompoundDatapoint>         compound_datapoints;
      std::vector<Attribut>                  attributes;
    };

    struct SimpleDevice : public BaseDevice {
      boost::optional<Plugins>             plugins;
      boost::optional<DataIdDescriptorRef> data_id_descriptor_ref;
      boost::optional<Instruction_set>     instruction_set;
      std::vector<FSM>                     fsms;
      std::vector<Method>                  methods;
    };

    struct CompoundDevice : public BaseDevice {
      std::vector<SimpleDevice>   simple_devices;
      std::vector<CompoundDevice> compound_devices;
    };

    //! \brief OPC Unified Architecture (OPCUA) server description
    struct OPCUA {

      Name                          name; //!< Name of the OPCUA server
      Attribut                      dummy;
      std::vector<FileName>         filenames;
      std::vector<Attribut>         attributes;
      boost::optional<ServerPort>   server_port;
      boost::optional<Multiplicity> multiplicity;
      std::vector<CompoundDevice>   compound_devices;
      std::vector<SimpleDevice>     simple_devices;

      void dump(std::ostream & out_) const
      {
        out_ << "Name: '" << name << "'" << std::endl;
        out_ << "Dummy: name='" << dummy.name << "' value='" << dummy.value << "'" << std::endl;
        for (const auto & attr : attributes) {
          out_ << "Attribut: name='" << attr.name << "' value='" << attr.value << "'" << std::endl;
        }
        for (const auto & filename : filenames) {
          out_ << "FileName: '" << filename << "'" << std::endl;
        }
        if (server_port) {
          out_ << "ServerPort: " << server_port.get() << std::endl;
        }
        if (multiplicity) {
          out_ << "Multiplicity: " << multiplicity.get() << std::endl;
        }
        return;
      }
    };

  } // namespace mos

} // namespace vire

#endif // VIRE_MOS_OPCUA_XML

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --c
