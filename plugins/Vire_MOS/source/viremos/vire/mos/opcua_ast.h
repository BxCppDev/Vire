//! \file vire/mos/opcua_ast.h
//
// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_MOS_OPCUA_AST_H
#define VIRE_MOS_OPCUA_AST_H

// Standard library:
#include <string>
#include <vector>
#include <iostream>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/optional.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>
#include <datatools/i_predicate.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// http://stackoverflow.com/questions/4821477/xml-schema-minoccurs-maxoccurs-default-values

namespace vire {

  //! Multipurpose OPCUA Server (MOS) namespace
  namespace mos {

    typedef unsigned long int hexBinary;

    //! Explicit hexadecimal value
    struct HexaValue
    {
      hexBinary bits;
      friend std::ostream & operator<<(std::ostream & out_, const HexaValue & attr_);
    };

    //! Primitive types for value data
    enum Type {
      TYPE_INVALID    = -1,
      TYPE_BOOL       =  0,
      TYPE_INT8       =  1,
      TYPE_INT16      =  2,
      TYPE_INT32      =  3,
      TYPE_INT64      =  4,
      TYPE_FLOAT      =  5,
      TYPE_DOUBLE     =  6,
      TYPE_STRING     =  7,
      TYPE_XML        =  8,
      TYPE_BYTESTRING =  9
    };

    //! Scope access
    enum ScopeAccess {
      SCOPE_ACCESS_INVALID   = -1,
      SCOPE_ACCESS_INTERNAL  =  0,
      SCOPE_ACCESS_PROTECTED =  1,
      SCOPE_ACCESS_EXTERNAL  =  2
    };

    //! \brief Type of access
    enum Access {
      ACCESS_INVALID = -1,
      ACCESS_INPUT,
      ACCESS_OUTPUT
    };

    //! \brief Type of communication interface
    enum Interface {
      INTERFACE_INVALID = -1,
      INTERFACE_GPIB,
      INTERFACE_UDP,
      INTERFACE_TCP,
      INTERFACE_COM,
      INTERFACE_PCI,
      INTERFACE_USB,
      INTERFACE_SERIAL,
      INTERFACE_OPCUA_PLC_EMULATION
    };

    //! \brief Duration of validity
    enum Validity {
      VALIDITY_INVALID = -1,
      VALIDITY_PERMANENT,
      VALIDITY_TEMPORARY
    };

    //! \brief Supported monitoring rate in seconds
    enum MonitoringRate {
      MONRATE_INVALID = -1,
      MONRATE_1    = 1,
      MONRATE_2    = 2,
      MONRATE_5    = 5,
      MONRATE_10   = 10,
      MONRATE_30   = 30,
      MONRATE_60   = 60,
      MONRATE_900  = 900,
      MONRATE_1800 = 1800,
      MONRATE_3600 = 3600
    };

    //! \brief Type of connection
    enum Connection {
      CONNECTION_INVALID = -1,
      CONNECTION_CLIENT,
      CONNECTION_SERVER
    };

    //! \brief Supported states for Finite State Machine
    enum StateFSM {
      STATE_FSM_INVALID   = -1,
      STATE_FSM_NOT_READY = 0,
      STATE_FSM_READY     = 1,
      STATE_FSM_RUNNING   = 2,
      STATE_FSM_PAUSING   = 3,
      STATE_FSM_SAFE      = 4
    };

    enum NameFSM {
      NAME_FSM_INVALID  = -1,
      NAME_FSM_CONFIGURE,
      NAME_FSM_START,
      NAME_FSM_STOP,
      NAME_FSM_PAUSE,
      NAME_FSM_RESUME,
      NAME_FSM_RESET
    };

    enum NameHardwareConfig {
      NAME_HW_CONFIG_INVALID = -1,
      NAME_HW_CONFIG_INIT,
      NAME_HW_CONFIG_CLOSE
    };

    // USED OR NOT ?
    enum NameAlarm {
      NAME_ALARM_INVALID = -1,
      NAME_ALARM_CLEAR_ALARM,
      NAME_ALARM_TROUBLE_SHOOTING
    };

    // XXX
    enum NameSpaceLevel {
      NSL_INVALID = -1,
      NSL_USER    =  0,
      NSL_EXPERT  =  1,
      NSL_DEBUG   =  2,
      NSL_SYSTEM  =  3
    };

    typedef std::string DefaultValue;
    typedef std::string Value;
    typedef std::string Message;
    typedef bool        Enable;
    typedef int         Severity;
    typedef std::string Name;
    typedef std::string Mnemonic;
    typedef std::string ResolutionDataChange;
    typedef std::string ResolutionDataChangeMode;
    typedef int         Multiplicity;
    typedef int         StartNumberingMultiplicity;
    typedef std::string DataFrameStructureRef;
    typedef int         Pos;
    typedef int         Nb;
    typedef int         Index;
    typedef std::string Id;
    typedef std::string ReadIdCollector;
    typedef std::string Description;
    typedef bool        Historizing;
    typedef int         AccessLevel;
    typedef int         WriteMask;
    typedef HexaValue   EndDelimiter;
    typedef HexaValue   Size;
    typedef std::string FileName;
    typedef HexaValue   IdNum;
    typedef std::string IpAddress;
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
    typedef std::string FileFSMRef;
    typedef std::string Cmd;
    typedef std::string Location;
    typedef int         TimeOut;
    typedef std::string EndInstruction;
    typedef std::string FileMapping;

    //! Print an optional object in an output stream (primitive types only)
    template<typename PrimitiveType>
    void optional_print(const boost::optional<PrimitiveType> & opt_,
                        std::ostream & out_ = std::clog)
    {
      // if (opt_ == boost::none) {
      if (!opt_) {
        out_ << "<option not set>";
      } else {
        if (typeid(opt_) == typeid(std::string)) {
          out_ << "'";
        }
        if (typeid(opt_) == typeid(bool)) {
          out_ << std::boolalpha;
        }
        out_ << opt_.get();
        if (typeid(opt_) == typeid(std::string)) {
          out_ << "'";
        }
      }
      return;
    }

    //! Print an optional object in an output stream (primitive types only)
    template<typename PrimitiveType>
    void optional_print_yesno(const boost::optional<PrimitiveType> & opt_,
                              std::ostream & out_ = std::clog)
    {
      // if (opt_ == boost::none) {
      if (!opt_) {
        out_ << "<option not set>";
      } else {
        out_ << "<option is set>";
      }
      return;
    }

    struct has_name_interface
    {
      virtual const std::string & get_name() const = 0;
    };

    struct Unit
    {
      std::string dimension; //!< Unit dimension
      std::string unit;      //!< Unit symbol
      friend std::ostream & operator<<(std::ostream & out_, const Unit & unit_);
    };

    // New in 2.0 (was bool typedef)
    struct Asynchronous
    {
      Enable         enable;
      TimeOut        timeout;
      EndInstruction end_instruction;
      friend std::ostream & operator<<(std::ostream & out_, const Asynchronous & item_);
    };

    struct Info;

    struct has_info_interface
    {
      virtual const boost::optional<Info> & get_info() const = 0;
      bool is_config() const;
      bool is_scope_internal() const;
      bool is_scope_protected() const;
      bool is_scope_external() const;
    };

    struct Attribut;

    struct has_attributes_interface
    {
      virtual const std::vector<Attribut> & get_attributes() const = 0;
    };

    struct UserInfo;

    struct has_userinfos_interface
    {
      virtual const std::vector<UserInfo> & get_userinfos() const = 0;
      std::vector<UserInfo>::const_iterator find_userinfo_name(const std::string &) const;
      bool has_userinfo_name(const std::string &) const;
      const std::string & get_userinfo_value(const std::string &) const;
    };

    //! \brief Description of an instruction
    struct Instruction
      : public datatools::i_tree_dumpable
    {
      Name                          name;
      boost::optional<Cmd>          cmd;
      boost::optional<Acknowledge>  acknowledge;
      boost::optional<Asynchronous> asynchronous;

      virtual ~Instruction();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    //! \brief A set of instructions
    struct Instruction_set
      : public datatools::i_tree_dumpable
    {
      std::vector<Instruction>     instructions;

      virtual ~Instruction_set();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    //! \brief Base class for alarm and error objects
    struct Base_ErrorAlarm
      : public datatools::i_tree_dumpable
    {
      Value                        value;
      boost::optional<Message>     message;
      boost::optional<Severity>    severity;
      boost::optional<Enable>      enable;
      boost::optional<Acknowledge> acknowledge;

      virtual ~Base_ErrorAlarm();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    //! \brief Undocumented
    struct Error_LimitHigh
      : public Base_ErrorAlarm
    {
    };

    //! \brief Undocumented
    struct Error_LimitLow
      : public Base_ErrorAlarm
    {
    };

    //! \brief Undocumented
    struct Error_Equal
      : public Base_ErrorAlarm
    {
    };

    //! \brief Undocumented
    struct Error_NotEqual
      : public Base_ErrorAlarm
    {
    };

    //! \brief Undocumented
    struct Alarm_LimitHigh
      : public Base_ErrorAlarm
    {
    };

    //! \brief Undocumented
    struct Alarm_LimitLow
      : public Base_ErrorAlarm
    {
    };

    //! \brief Undocumented
    struct Alarm_Equal
      : public Base_ErrorAlarm
    {
    };

    //! \brief Undocumented
    struct Alarm_NotEqual
      : public Base_ErrorAlarm
    {
    };

    //! \brief Generic alarm/error object
    struct Alarm
    {
      boost::optional<Alarm_LimitHigh> alarm_limit_high;
      boost::optional<Error_LimitHigh> error_limit_high;
      boost::optional<Alarm_LimitLow>  alarm_limit_low;
      boost::optional<Error_LimitLow>  error_limit_low;
      boost::optional<Alarm_Equal>     alarm_equal;
      boost::optional<Error_Equal>     error_equal;
      boost::optional<Alarm_NotEqual>  alarm_not_equal;
      boost::optional<Error_NotEqual>  error_not_equal;
    };

    //! \brief Undocumented
    struct ElementArray
    {
      Pos   pos;
      Value value;
      friend std::ostream & operator<<(std::ostream & out_, const ElementArray & item_);
    };

    //! \brief Undocumented
    struct Event
    {
      Message message;
      friend std::ostream & operator<<(std::ostream & out_, const Event & attr_);
    };

    //! \brief UserInfo with key/value pair
    struct UserInfo
    {
      Name  name;
      Value value;
      friend std::ostream & operator<<(std::ostream & out_, const UserInfo & uinfo_);
      struct has_name_predicate : datatools::i_predicate<UserInfo>
      {
        has_name_predicate(const std::string & name_)
          : _name_(name_) {}
        bool operator()(const UserInfo & uinfo_) const {
          return uinfo_.name == _name_;
        }
      private:
        std::string _name_;
      };
    };

    //! \brief Attribut with key/value pair
    struct Attribut
    {
      Name  name;
      Value value;
      friend std::ostream & operator<<(std::ostream & out_, const Attribut & attr_);
      struct has_name_predicate
        : datatools::i_predicate<Attribut>
      {
        has_name_predicate(const std::string & name_) : _name_(name_) {}
        bool operator()(const Attribut & attr_) const {
          return attr_.name == _name_;
        }
      private:
        std::string _name_;
      };
    };

    //! \brief Object interface with type and optional unit support
    struct has_type_interface
    {
      virtual Type get_type() const = 0;
      virtual const boost::optional<Unit> & get_unit() const = 0;
    };

    //! \brief Optional informations
    struct Info
    {
      boost::optional<Config>         config;
      boost::optional<ICD>            icd;
      boost::optional<NameSpaceLevel> name_space_level;
      boost::optional<ScopeAccess>    scope_access;
      bool is_config() const;
      bool operator==(const Info & i_) const;
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
      friend std::ostream & operator<<(std::ostream & out_, const Info & info_);
    };

    //! \brief Description of a variable
    struct Variable
      : public has_type_interface
      , public has_userinfos_interface
      , public has_info_interface
    {
      boost::optional<Info>        info;
      std::vector<UserInfo>        userinfos;
      Name                         name;
      Type                         type;
      boost::optional<Unit>        unit;
      Multiplicity                 multiplicity;
      boost::optional<StartNumberingMultiplicity> start_numbering_multiplicity;
      boost::optional<Value>       value;
      boost::optional<ArraySize>   array_size;
      std::vector<ElementArray>    elements;
      boost::optional<Description> description;
      boost::optional<Historizing> historizing;
      boost::optional<AccessLevel>  access_level;
      boost::optional<WriteMask>   write_mask;

      virtual ~Variable();
      virtual Type get_type() const;
      virtual const boost::optional<Unit> & get_unit() const;
      virtual const boost::optional<Info> & get_info() const;
      virtual const std::vector<UserInfo> & get_userinfos() const;

    };

    //! \brief Range with min/max directives
    struct Range
    {
      boost::optional<Min> min;
      boost::optional<Max> max;
      friend std::ostream & operator<<(std::ostream & out_, const Range & r_);
    };

    //! \brief Undocumented
    struct DefaultArgument
    {
      Value value;
      Type type;
      boost::optional<Description> description;
    };

    //! \brief Undocumented
    struct Sequence
    {
      NodeId node_id;
      MethodId method_id;
      boost::optional<ServerId> server_id;
      std::vector<DefaultArgument> default_arguments;
    };

    //! \brief Undocumented
    struct DeviceInstruction
    {
      Value value;
    };

    //! \brief Description of an argument
    struct Argument
      : public has_name_interface
      , public has_type_interface
      , public has_userinfos_interface
    {
      Name                          name;
      Type                          type;
      boost::optional<Unit>         unit;
      boost::optional<Access>       access;
      boost::optional<DefaultValue> default_value;
      boost::optional<Description>  description;
      std::vector<UserInfo>         userinfos;

      virtual ~Argument();

      virtual const std::string & get_name() const;
      virtual Type get_type() const;
      virtual const boost::optional<Unit> & get_unit() const;
      virtual const std::vector<UserInfo> & get_userinfos() const;
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    };

    //! \brief Base class for other data structures
    struct base_hidf
      : public has_type_interface
    {
      Name                  name;
      Type                  type;
      boost::optional<Unit> unit;
      boost::optional<Size> size;
      boost::optional<EndDelimiter> end_delimiter;

      virtual ~base_hidf();

      virtual Type get_type() const;
      virtual const boost::optional<Unit> & get_unit() const;

    };

    //! \brief Undocumented
    struct Header
      : public base_hidf
    {
    };

    //! \brief Undocumented
    struct Id1
      : public base_hidf
    {
    };

    //! \brief Undocumented
    struct Id2
      : public base_hidf
    {
    };

    //! \brief Undocumented
    struct Data
      : public base_hidf
    {
    };

    //! \brief Undocumented
    struct Footer
      : public base_hidf
    {
    };

    //! \brief Undocumented
    struct DataFrameStructure
    {
      Header header;
      Id1    id1;
      Id2    id2;
      Data   data;
      Footer footer;
    };

    //! \brief Undocumented
    struct Frame_elementString
      : public has_type_interface
    {
      Name                         name;
      Type                         type;
      boost::optional<Unit>        unit;
      EndDelimiter end_delimiter;

      virtual ~Frame_elementString();

      virtual Type get_type() const;
      virtual const boost::optional<Unit> & get_unit() const;

      friend std::ostream & operator<<(std::ostream & out_, const Frame_elementString & fes_);
    };

    //! \brief Undocumented
    struct Frame_element
      : public has_type_interface
    {
      Name                  name;
      Type                  type;
      boost::optional<Unit> unit;
      Index                 index;
      boost::optional<Pos>  pos;
      boost::optional<Nb>   nb;

      virtual ~Frame_element();
      virtual Type get_type() const;
      virtual const boost::optional<Unit> & get_unit() const;

      friend std::ostream & operator<<(std::ostream & out_, const Frame_element & fe_);
    };

    //! \brief Description for a plugin DLL
    struct Plugins
    {
      Name     name;
      Location location;

      friend std::ostream & operator<<(std::ostream & out_, const Plugins & plugins_);
    };

    //! \brief Undocumented
    struct Frame_definition
    {
      Name name;
      std::vector<Frame_elementString> frame_element_strings;
      std::vector<Frame_element>       frame_elements;
    };

    //! \brief Description of a Finite State Machine
    struct FSM
    {
      NameFSM                        name_fsm;
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
    };

    //! \brief Undocumented
    struct Start
    {
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
    };

    //! \brief Undocumented
    struct HardwareConfig
    {
      NameHardwareConfig             name_hardware_config;
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
      std::vector<EndDelimiter>      end_delimiters;
    };

    //! \brief Undocumented
    struct Watchdog
    {
      boost::optional<Enable>      enable;
      boost::optional<Acknowledge> acknowledge;
      boost::optional<Cmd>         cmd;
    };

    //! \brief Undocumented
    struct AlarmMethod
    {
      NameAlarm                      name_alarm;
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
    };

    //! \brief Description of a method
    struct Method
      : public has_name_interface
      , public has_attributes_interface
      , public has_userinfos_interface
      , public has_info_interface
    {
      Name                           name;
      std::vector<Argument>          arguments;
      std::vector<DeviceInstruction> device_instructions;
      std::vector<Sequence>          sequences;
      std::vector<EndDelimiter>      end_delimiters;
      std::vector<Event>             events;
      boost::optional<Info>          info;
      std::vector<Attribut>          attributes;
      std::vector<UserInfo>          userinfos;

      virtual const std::string & get_name() const;
      virtual const std::vector<Attribut> & get_attributes() const;
      virtual const boost::optional<Info> & get_info() const;
      virtual const std::vector<UserInfo> & get_userinfos() const;

      virtual ~Method();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    struct has_common_interface
      : public has_name_interface
      , public has_attributes_interface
    {
      virtual const boost::optional<Multiplicity> & get_multiplicity() const = 0;
    };

    struct has_methods_interface
    {
      virtual const std::vector<Method> & get_methods() const = 0;
    };

    struct SimpleDatapoint;
    struct CompoundDatapoint;

    struct has_datapoints_interface
    {
      virtual const std::vector<SimpleDatapoint> & get_simple_datapoints() const = 0;
      virtual const std::vector<CompoundDatapoint> & get_compound_datapoints() const = 0;
    };

    //! \brief Base description of a datapoint
    struct BaseDatapoint
      : public has_common_interface
      , public has_userinfos_interface
      , public has_methods_interface
      , public has_info_interface
    {
      Name                          name;
      boost::optional<Multiplicity> multiplicity;
      boost::optional<StartNumberingMultiplicity> start_numbering_multiplicity;
      std::vector<Attribut>         attributes;
      std::vector<Method>           methods;
      boost::optional<Info>         info;
      std::vector<UserInfo>         userinfos;

      virtual const std::string & get_name() const;
      virtual const boost::optional<Multiplicity> & get_multiplicity() const;
      virtual const std::vector<Attribut> & get_attributes() const;
      virtual const std::vector<UserInfo> & get_userinfos() const;
      virtual const boost::optional<Info> & get_info() const;
      virtual const std::vector<Method> & get_methods() const;

      virtual ~BaseDatapoint();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    //! \brief Description of a simple datapoint
    struct SimpleDatapoint
      : public BaseDatapoint
      , public has_type_interface
    {
      Type                          type;
      boost::optional<Unit>         unit;
      boost::optional<Id>           id;
      boost::optional<AccessLevel>  access_level;
      boost::optional<ArraySize>    array_size;
      boost::optional<MonitoringRate> monitoring_rate;
      boost::optional<DataFrameStructureRef> data_frame_structure_ref;
      boost::optional<DefaultValue> default_value;
      std::vector<ElementArray>     elements;
      boost::optional<Range>        range;
      boost::optional<Description>  description;
      boost::optional<Historizing>  historizing;
      boost::optional<WriteMask>    write_mask;
      boost::optional<EndDelimiter> end_delimiter;
      boost::optional<Alarm>        alarm;
      std::vector<AlarmMethod>      alarm_methods;
      std::vector<Variable>         variables;
      std::vector<Frame_element>                frame_elements;
      std::vector<Frame_elementString>          frame_element_strings;
      boost::optional<ResolutionDataChange>     resolution_data_change;
      boost::optional<ResolutionDataChangeMode> resolution_data_change_mode;

      virtual Type get_type() const;
      virtual const boost::optional<Unit> & get_unit() const;

      virtual ~SimpleDatapoint();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    //! \brief Description of a compound datapoint
    struct CompoundDatapoint
      : public BaseDatapoint
      , public has_datapoints_interface
    {
      std::vector<CompoundDatapoint> compound_datapoints;
      std::vector<SimpleDatapoint>   simple_datapoints;

      virtual const std::vector<SimpleDatapoint> & get_simple_datapoints() const;
      virtual const std::vector<CompoundDatapoint> & get_compound_datapoints() const;
      virtual ~CompoundDatapoint();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    //! \brief Description of a frame
    struct Frames_description
    {
      std::vector<SimpleDatapoint>   simple_datapoints;
      std::vector<CompoundDatapoint> compound_datapoints;
    };

    //! \brief Base description of a device
    struct BaseDevice
      : public datatools::i_tree_dumpable
      , public has_common_interface
      , public has_userinfos_interface
      , public has_datapoints_interface
      , public has_info_interface
    {
      Name                                   name;
      boost::optional<Multiplicity>          multiplicity;
      boost::optional<StartNumberingMultiplicity> start_numbering_multiplicity;
      boost::optional<Interface>             interface;
      boost::optional<DataFrameStructureRef> data_frame_structure_ref;
      std::vector<HardwareConfig>            hardware_configs;
      std::vector<Attribut>                  attributes;
      std::vector<CompoundDatapoint>         compound_datapoints;
      std::vector<SimpleDatapoint>           simple_datapoints;
      std::vector<UserInfo>                  userinfos;
      boost::optional<Info>                  info;

      virtual const std::string & get_name() const;
      virtual const boost::optional<Multiplicity> & get_multiplicity() const;
      virtual const std::vector<Attribut> & get_attributes() const;
      virtual const std::vector<UserInfo> & get_userinfos() const;
       virtual const boost::optional<Info> & get_info() const;

      virtual const std::vector<SimpleDatapoint> & get_simple_datapoints() const;
      virtual const std::vector<CompoundDatapoint> & get_compound_datapoints() const;

      virtual ~BaseDevice();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    struct SimpleDevice;
    struct CompoundDevice;

    struct has_devices_interface
    {
      virtual const std::vector<SimpleDevice> & get_simple_devices() const = 0;
      virtual const std::vector<CompoundDevice> & get_compound_devices() const = 0;
    };

    //! \brief Description of a simple device
    struct SimpleDevice
      : public BaseDevice
      , public has_methods_interface
    {
      boost::optional<Plugins>             plugins;
      boost::optional<Instruction_set>     instruction_set;
      boost::optional<DataIdDescriptorRef> data_id_descriptor_ref;
      boost::optional<FileFSMRef>          file_fsm_ref;
      std::vector<FSM>                     FSMs;
      std::vector<Method>                  methods;
      boost::optional<Watchdog>            watchdog;

      virtual const std::vector<Method> & get_methods() const;
      virtual ~SimpleDevice();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    //! \brief Description of a compound device
    struct CompoundDevice
      : public BaseDevice
      , public has_devices_interface
    {
      std::vector<CompoundDevice> compound_devices;
      std::vector<SimpleDevice>   simple_devices;

      virtual const std::vector<SimpleDevice> & get_simple_devices() const;
      virtual const std::vector<CompoundDevice> & get_compound_devices() const;

      virtual ~CompoundDevice();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

    //! \brief Description of an OPCUA server
    struct OPCUA
      : public datatools::i_tree_dumpable
      , public has_name_interface
      , public has_attributes_interface
      , public has_devices_interface
      , public has_userinfos_interface
    {
      std::vector<UserInfo>         userinfos;
      Name                          name; //!< Name of the OPCUA server
      // std::vector<OPCUA>            opcuas; //!< Added in 0.2 but not supported yet
      boost::optional<ServerPort>   server_port;
      std::vector<Attribut>         attributes;
      std::vector<FileName>         filenames;
      std::vector<CompoundDevice>   compound_devices;
      std::vector<SimpleDevice>     simple_devices;

      virtual const std::string & get_name() const;
      virtual const std::vector<Attribut> & get_attributes() const;
      virtual const std::vector<SimpleDevice> & get_simple_devices() const;
      virtual const std::vector<CompoundDevice> & get_compound_devices() const;
      virtual const std::vector<UserInfo> & get_userinfos() const;

      virtual ~OPCUA();
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

  } // namespace mos

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_TYPE_INIT(::vire::mos::Type);
DR_TYPE_INIT(::vire::mos::ScopeAccess);
DR_TYPE_INIT(::vire::mos::Access);
DR_TYPE_INIT(::vire::mos::Interface);
DR_TYPE_INIT(::vire::mos::Validity);
DR_TYPE_INIT(::vire::mos::MonitoringRate);
DR_TYPE_INIT(::vire::mos::Connection);
DR_TYPE_INIT(::vire::mos::StateFSM);
DR_TYPE_INIT(::vire::mos::NameFSM);
DR_TYPE_INIT(::vire::mos::NameHardwareConfig);
DR_TYPE_INIT(::vire::mos::NameAlarm);
DR_TYPE_INIT(::vire::mos::NameSpaceLevel);
#endif // Q_MOC_RUN

#endif // VIRE_MOS_OPCUA_AST

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
