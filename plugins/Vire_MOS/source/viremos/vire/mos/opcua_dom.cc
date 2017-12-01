// opcua_dom.cc
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

// Ourselves:
#include <vire/mos/opcua_dom.h>

// Standard Library:
#include <iostream>
#include <sstream>

// Third Party:
// - Xerces-C++:
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/util/XMLString.hpp>
// - Boost:
#include <boost/algorithm/string.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

namespace vire {

  namespace mos {

    // HexaValue:
    hexa_value_builder::hexa_value_builder(HexaValue & value_,
                                           datatools::logger::priority prio_)
      : base_generic_builder<HexaValue>(value_, prio_)
    {
      return;
    }

    void hexa_value_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_generic_builder<HexaValue>::grab().bits = 0;
      bool success = false;
      const XMLCh * node_name = node_->getNodeName();
      xercesc::DOMNode::NodeType node_type = node_->getNodeType();
      const xercesc::DOMNodeList * node_children = node_->getChildNodes();
      for (int inode = 0; inode < node_children->getLength(); inode++) {
        const xercesc::DOMNode * child_node = node_children->item(inode);
        xercesc::DOMNode::NodeType child_node_type = child_node->getNodeType();
        if (child_node_type != xercesc::DOMNode::TEXT_NODE) {
          continue;
        } else {
          const xercesc::DOMText * text_node
            = dynamic_cast<const xercesc::DOMText *>(child_node);
          std::string val_str = xercesc::XMLString::transcode(text_node->getData());
          std::istringstream iss(val_str);
          hexBinary val;
          iss >> std::hex >> val;
          DT_THROW_IF (!iss, std::logic_error, "Invalid hex value!");
          base_generic_builder<HexaValue>::grab().bits = val;
          success = true;
          break;
        }
      }
      DT_THROW_IF(! success, std::logic_error,
                  "Cannot parse an integer from text node '" << xercesc::XMLString::transcode(node_name) << "'");
      return;
    }

    // Instruction:
    asynchronous_builder::asynchronous_builder(Asynchronous & value_,
                                               datatools::logger::priority prio_)
      : base_generic_builder<Asynchronous>(value_, prio_)
    {
      return;
    }

    void asynchronous_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Enable",         Enable,         enable_builder,          grab().enable,          _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_,  "TimeOut",        TimeOut,        timeout_builder,         grab().timeout,         _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_,  "EndInstruction", EndInstruction, end_instruction_builder, grab().end_instruction, _logging);
      return;
    }

    // Instruction:
    instruction_builder::instruction_builder(Instruction & value_,
                                             datatools::logger::priority prio_)
      : base_generic_builder<Instruction>(value_, prio_)
    {
      return;
    }

    void instruction_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",         Name,         name_builder,         grab().name,        _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Cmd",          Cmd,          cmd_builder,          grab().cmd,         _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Acknowledge",  Acknowledge,  acknowledge_builder,  grab().acknowledge,  _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Asynchronous", Asynchronous, asynchronous_builder, grab().asynchronous, _logging);
      return;
    }

    // Instruction set:
    instruction_set_builder::instruction_set_builder(Instruction_set & value_,
                                                     datatools::logger::priority prio_)
      : base_generic_builder<Instruction_set>(value_, prio_)
    {
      return;
    }

    void instruction_set_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "Instruction", Instruction, instruction_builder, grab().instructions, 1, _logging);
      return;
    }

    // Base_ErrorAlarm:
    base_error_alarm_builder::base_error_alarm_builder(Base_ErrorAlarm & value_,
                                                       datatools::logger::priority prio_)
      : base_generic_builder<Base_ErrorAlarm>(value_, prio_)
    {
      return;
    }

    void base_error_alarm_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Value",       Value,       value_builder,       grab().value,       _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Message",     Message,     message_builder,     grab().message,     _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Severity",    Severity,    severity_builder,    grab().severity,    _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Enable",      Enable,      enable_builder,      grab().enable,      _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Acknowledge", Acknowledge, acknowledge_builder, grab().acknowledge, _logging);
      return;
    }

    // Alarm:
    alarm_builder::alarm_builder(Alarm & value_, datatools::logger::priority prio_)
      : base_generic_builder<Alarm>(value_, prio_)
    {
      return;
    }

    void alarm_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Alarm_LimitHigh", Alarm_LimitHigh, base_error_alarm_builder, grab().alarm_limit_high, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Error_LimitHigh", Error_LimitHigh, base_error_alarm_builder, grab().error_limit_high, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Alarm_LimitLow",  Alarm_LimitLow,  base_error_alarm_builder, grab().alarm_limit_low,  _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Error_LimitLow",  Error_LimitLow,  base_error_alarm_builder, grab().error_limit_low,  _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Alarm_Equal",     Alarm_Equal,     base_error_alarm_builder, grab().alarm_equal,      _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Error_Equal",     Error_Equal,     base_error_alarm_builder, grab().error_equal,      _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Alarm_NotEqual",  Alarm_NotEqual,  base_error_alarm_builder, grab().alarm_not_equal,  _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Error_NotEqual",  Error_NotEqual,  base_error_alarm_builder, grab().error_not_equal,  _logging);
      return;
    }

    // ElementArray:
    element_array_builder::element_array_builder(ElementArray & attr_,
                                                 datatools::logger::priority prio_)
      : base_generic_builder<ElementArray>(attr_, prio_)
    {
      return;
    }

    void element_array_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Pos",   Pos,   pos_builder,   grab().pos,   _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Value", Value, value_builder, grab().value, _logging);
      return;
    }

    // Event:
    event_builder::event_builder(Event & attr_,
                                 datatools::logger::priority prio_)
      : base_generic_builder<Event>(attr_, prio_)
    {
      return;
    }

    void event_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Message", Message, message_builder, grab().message, _logging);
      return;
    }

    // Variable:
    variable_builder::variable_builder(Variable & value_,
                                       datatools::logger::priority prio_)
      : base_generic_builder<Variable>(value_, prio_)
    {
      return;
    }

    void variable_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",         Name,         name_builder,          grab().name,         _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Type",         Type,         type_builder,          grab().type,         _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Unit",         Unit,         unit_builder,          grab().unit,         _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Multiplicity", Multiplicity, multiplicity_builder,  grab().multiplicity, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Value",        Value,        value_builder,         grab().value,        _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "ArraySize",    ArraySize,    array_size_builder,    grab().array_size,   _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "ElementArray", ElementArray, element_array_builder, grab().elements, 0,  _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Description",  Description,  description_builder,   grab().description,  _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Historizing",  Historizing,  historizing_builder,   grab().historizing,  _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "AccesLevel",   AccesLevel,   acces_level_builder,   grab().acces_level,  _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "WriteMask",    WriteMask,    write_mask_builder,    grab().write_mask,   _logging);
      return;
    }

    // Range:
    range_builder::range_builder(Range & value_,
                                 datatools::logger::priority prio_)
      : base_generic_builder<Range>(value_, prio_)
    {
      return;
    }

    void range_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Min", Min, min_builder, grab().min, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Max", Max, max_builder, grab().max, _logging);
      return;
    }

    // Info:
    info_builder::info_builder(Info & value_,
                               datatools::logger::priority prio_)
      : base_generic_builder<Info>(value_, prio_)
    {
      return;
    }

    void info_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Config", Config, config_builder, grab().config, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "ICD",   ICD,     icd_builder,    grab().icd,    _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "NameSpaceLevel", NameSpaceLevel, name_space_level_builder, grab().name_space_level, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "ScopeAccess", ScopeAccess, scope_access_builder, grab().scope_access, _logging);
      return;
    }

    // Sequence:
    sequence_builder::sequence_builder(Sequence & value_,
                                       datatools::logger::priority prio_)
      : base_generic_builder<Sequence>(value_, prio_)
    {
      return;
    }

    void sequence_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "NodeId",   NodeId,   node_id_builder,   grab().node_id,   _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "MethodId", MethodId, method_id_builder, grab().method_id, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "ServerId", ServerId, server_id_builder, grab().server_id, _logging);
      return;
    }

    // DeviceInstruction:
    device_instruction_builder::device_instruction_builder(DeviceInstruction & value_,
                                                           datatools::logger::priority prio_)
      : base_generic_builder<DeviceInstruction>(value_, prio_)
    {
      return;
    }

    void device_instruction_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Value", Value, value_builder, grab().value, _logging);
      return;
    }

    // Argument:
    argument_builder::argument_builder(Argument & value_,
                                       datatools::logger::priority prio_)
      : base_generic_builder<Argument>(value_, prio_)
    {
      return;
    }

    void argument_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",         Name,         name_builder,          grab().name,          _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Type",         Type,         type_builder,          grab().type,          _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Unit",         Unit,         unit_builder,          grab().unit,          _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Access",       Access,       access_builder,        grab().access,        _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "DefaultValue", DefaultValue, default_value_builder, grab().default_value, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Description",  Description,  description_builder,   grab().description,   _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "UserInfo",     UserInfo,     userinfo_builder,      grab().userinfos, 0,  _logging);
      return;
    }

    // base_hidf:
    base_hidf_builder::base_hidf_builder(base_hidf & value_,
                                       datatools::logger::priority prio_)
      : base_generic_builder<base_hidf>(value_, prio_)
    {
      return;
    }

    void base_hidf_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",         Name,         name_builder,          grab().name,          _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Type",         Type,         type_builder,          grab().type,          _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Unit",         Unit,         unit_builder,          grab().unit,         _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Size",         Size,         size_builder,          grab().size,          _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "EndDelimiter", EndDelimiter, end_delimiter_builder, grab().end_delimiter, _logging);
      return;
    }

    // DataFrameStructure:
    data_frame_structure_builder::data_frame_structure_builder(DataFrameStructure & value_,
                                                               datatools::logger::priority prio_)
      : base_generic_builder<DataFrameStructure>(value_, prio_)
    {
      return;
    }

    void data_frame_structure_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Header",  Header,  base_hidf_builder,  grab().header,  _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Id1",     Id1,     base_hidf_builder,  grab().id1,     _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Id2",     Id2,     base_hidf_builder,  grab().id2,     _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Data",    Data,    base_hidf_builder,  grab().data,    _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Footer",  Footer,  base_hidf_builder,  grab().footer,  _logging);
      return;
    }

    // Frame_elementString:
    frame_element_string_builder::frame_element_string_builder(Frame_elementString & value_,
                                                               datatools::logger::priority prio_)
      : base_generic_builder<Frame_elementString>(value_, prio_)
    {
      return;
    }

    void frame_element_string_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",         Name,         name_builder,          grab().name,          _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Type",         Type,         type_builder,          grab().type,          _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Unit",         Unit,         unit_builder,          grab().unit,         _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "EndDelimiter", EndDelimiter, end_delimiter_builder, grab().end_delimiter, _logging);
      return;
    }

    // Frame_element:
    frame_element_builder::frame_element_builder(Frame_element & value_,
                                                 datatools::logger::priority prio_)
      : base_generic_builder<Frame_element>(value_, prio_)
    {
      return;
    }

    void frame_element_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",  Name,  name_builder,  grab().name,  _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Type",  Type,  type_builder,  grab().type,  _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Unit",         Unit,         unit_builder,          grab().unit,         _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Index", Index, index_builder, grab().index, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Pos",   Pos,   pos_builder,   grab().pos,   _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Nb",    Nb,    nb_builder,    grab().nb,    _logging);
      return;
    }

    // Unit:
    unit_builder::unit_builder(Unit & unit_,
                               datatools::logger::priority prio_)
      : base_generic_builder<Unit>(unit_, prio_)
    {
      return;
    }

    void unit_builder::operator()(const xercesc::DOMNode * node_)
    {
      const std::string node_name =  xercesc::XMLString::transcode(node_->getNodeName());
      DT_LOG_DEBUG(_logging, "Searching mandatory child node '" << "Unit" << "' from node '" << node_name << "'");
      const xercesc::DOMNodeList * node_children = node_->getChildNodes();
      int counter = 0;
      DT_LOG_DEBUG(_logging, "  Number of child nodes = [" << node_children->getLength() << "] for node '" << node_name << "'");
      for (int inode = 0; inode < node_children->getLength(); inode++) {
        const xercesc::DOMNode * child_node = node_children->item(inode);
        DT_LOG_DEBUG(_logging, "  Child node '" << xercesc::XMLString::transcode(child_node->getNodeName())
                     << "' from node '" << node_name << "'");
        std::string dimension_label = xercesc::XMLString::transcode(child_node->getNodeName());
        if (boost::algorithm::ends_with(dimension_label, "Unit")) {
          grab().dimension = dimension_label;
          DT_LOG_DEBUG(_logging, " Attributes: " << child_node->hasAttributes() << ".");
          const xercesc::DOMNamedNodeMap * attr_map = child_node->getAttributes();
          if (attr_map->getLength() == 1) {
            XMLCh * attr_name = xercesc::XMLString::transcode("unit");
            const xercesc::DOMNode * attr_node = attr_map->getNamedItem(attr_name);
            if (attr_node != 0) {
              DT_LOG_DEBUG(_logging, " Found attribute with name '" << xercesc::XMLString::transcode(attr_name) << "'.");
              string_builder str_build(grab().unit, _logging);
              str_build(attr_node);
            }
            xercesc::XMLString::release(&attr_name);
          }
          DT_LOG_DEBUG(_logging, " Attribute unit symbol is '" << get().unit << "' of dimension '" << get().dimension << "'.");
        }
      }
      return;
    }

    // UserInfo:
    userinfo_builder::userinfo_builder(UserInfo & uinfo_,
                                       datatools::logger::priority prio_)
      : base_generic_builder<UserInfo>(uinfo_, prio_)
    {
      return;
    }

    void userinfo_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",  Name,  name_builder,  grab().name,  _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Value", Value, value_builder, grab().value, _logging);
      return;
    }

    // Attribut:
    attribute_builder::attribute_builder(Attribut & attr_,
                                         datatools::logger::priority prio_)
      : base_generic_builder<Attribut>(attr_, prio_)
    {
      return;
    }

    void attribute_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",  Name,  name_builder,  grab().name,  _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Value", Value, value_builder, grab().value, _logging);
      return;
    }

    // Plugins:
    plugins_builder::plugins_builder(Plugins & obj_,
                                     datatools::logger::priority prio_)
      : base_generic_builder<Plugins>(obj_, prio_)
    {
      return;
    }

    void plugins_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",     Name,     name_builder,     grab().name,     _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Location", Location, location_builder, grab().location, _logging);
      return;
    }

    // Frame_definition:
    frame_definition_builder::frame_definition_builder(Frame_definition & obj_,
                                                       datatools::logger::priority prio_)
      : base_generic_builder<Frame_definition>(obj_, prio_)
    {
      return;
    }

    void frame_definition_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",                Name,
                                     name_builder, grab().name, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "Frame_elementString", Frame_elementString,
                                  frame_element_string_builder, grab().frame_element_strings, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "Frame_element",       Frame_element,
                                  frame_element_builder, grab().frame_elements, 0, _logging);
      return;
    }

    // FSM:
    fsm_builder::fsm_builder(FSM & obj_, datatools::logger::priority prio_)
      : base_generic_builder<FSM>(obj_, prio_)
    {
      return;
    }

    void fsm_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "NameFSM", NameFSM, name_fsm_builder, grab().name_fsm, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "Argument", Argument,
                                  argument_builder, grab().arguments, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "DeviceInstruction", DeviceInstruction,
                                  device_instruction_builder, grab().device_instructions, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "Sequence", Sequence,
                                  sequence_builder, grab().sequences, 0, _logging);
      return;
    }

    // HardwareConfig:
    hardware_config_builder::hardware_config_builder(HardwareConfig & obj_, datatools::logger::priority prio_)
      : base_generic_builder<HardwareConfig>(obj_, prio_)
    {
      return;
    }

    void hardware_config_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "NameHardwareConfig", NameHardwareConfig,
                                     name_hardware_config_builder, grab().name_hardware_config, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "Argument", Argument,
                                  argument_builder, grab().arguments, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "DeviceInstruction", DeviceInstruction,
                                  device_instruction_builder, grab().device_instructions, 0,_logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "Sequence", Sequence,
                                  sequence_builder, grab().sequences, 0,_logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "EndDelimiter", EndDelimiter,
                                  end_delimiter_builder, grab().end_delimiters, 0,_logging);
      return;
    }

    // AlarmMethod:
    alarm_method_builder::alarm_method_builder(AlarmMethod & obj_, datatools::logger::priority prio_)
      : base_generic_builder<AlarmMethod>(obj_, prio_)
    {
      return;
    }

    void alarm_method_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "NameAlarm", NameAlarm, name_alarm_builder, grab().name_alarm, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "Argument", Argument, argument_builder, grab().arguments, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "DeviceInstruction", DeviceInstruction, device_instruction_builder, grab().device_instructions, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "Sequence", Sequence, sequence_builder, grab().sequences,0,  _logging);
      return;
    }

    // Watchdog:
    watchdog_builder::watchdog_builder(Watchdog & obj_, datatools::logger::priority prio_)
      : base_generic_builder<Watchdog>(obj_, prio_)
    {
      return;
    }

    void watchdog_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Enable",      Enable,      enable_builder,       grab().enable,      _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Acknowledge", Acknowledge, acknowledge_builder,  grab().acknowledge, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Cmd",         Cmd,         cmd_builder,          grab().cmd,         _logging);
       return;
    }

    // Method:
    method_builder::method_builder(Method & obj_, datatools::logger::priority prio_)
      : base_generic_builder<Method>(obj_, prio_)
    {
      return;
    }

    void method_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",         Name, name_builder, grab().name, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "Argument",     Argument, argument_builder, grab().arguments, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "DeviceInstruction", DeviceInstruction, device_instruction_builder, grab().device_instructions, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "Sequence",     Sequence, sequence_builder, grab().sequences, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "EndDelimiter", EndDelimiter, end_delimiter_builder, grab().end_delimiters, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "Event",        Event, event_builder, grab().events, 0, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "Info",         Info,     info_builder,          grab().info,          _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "Info",         Info,     info_builder,      grab().infos,      0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "Attribut",     Attribut, attribute_builder, grab().attributes, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "UserInfo",     UserInfo, userinfo_builder,  grab().userinfos,  0, _logging);
      return;
    }

    // BaseDatapoint:
    base_datapoint_builder::base_datapoint_builder(BaseDatapoint & obj_, datatools::logger::priority prio_)
      : base_generic_builder<BaseDatapoint>(obj_, prio_)
    {
      return;
    }

    void base_datapoint_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE (node_, "Name",         Name,         name_builder,         grab().name,         _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Multiplicity", Multiplicity, multiplicity_builder, grab().multiplicity, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "StartNumberingMultiplicity", StartNumberingMultiplicity,
                                      start_numbering_multiplicity_builder, grab().start_numbering_multiplicity,       _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "Attribut",     Attribut,     attribute_builder,    grab().attributes, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "Method",       Method,       method_builder,       grab().methods,    0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "UserInfo",     UserInfo,     userinfo_builder,     grab().userinfos,  0, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Info",         Info,         info_builder,         grab().info,          _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "Info",         Info,         info_builder,         grab().infos,      0, _logging);
      return;
    }

    // SimpleDatapoint:
    simple_datapoint_builder::simple_datapoint_builder(SimpleDatapoint & obj_,
                                                       datatools::logger::priority prio_)
      : base_generic_builder<SimpleDatapoint>(obj_, prio_) // base_datapoint_builder(obj_, prio_)
    {
      return;
    }

    void simple_datapoint_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_datapoint_builder base_builder(dynamic_cast<BaseDatapoint &>(grab()));
      base_builder(node_);
      VIRE_MOS_BUILD_MANDATORY_VALUE (node_, "Type",       Type, type_builder, grab().type, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Id",         Id, id_builder, grab().id, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "AccesLevel", AccesLevel, acces_level_builder, grab().acces_level, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "ArraySize",  ArraySize, array_size_builder, grab().array_size, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "MonitoringRate", MonitoringRate, monitoring_rate_builder, grab().monitoring_rate, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "DataFrameStructureRef", DataFrameStructureRef, data_frame_structure_ref_builder, grab().data_frame_structure_ref, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "DefaultValue", DefaultValue, default_value_builder, grab().default_value, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Unit",       Unit, unit_builder, grab().unit, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "ElementArray",   ElementArray, element_array_builder, grab().elements, 0, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Range",        Range, range_builder, grab().range, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Description",  Description, description_builder, grab().description, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Historizing",  Historizing, historizing_builder, grab().historizing, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "WriteMask",    WriteMask, write_mask_builder, grab().write_mask, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "EndDelimiter", EndDelimiter, end_delimiter_builder, grab().end_delimiter, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Alarm",        Alarm, alarm_builder, grab().alarm, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "AlarmMethod",  AlarmMethod, alarm_method_builder, grab().alarm_methods, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "Variable",     Variable, variable_builder, grab().variables, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "Frame_element",       Frame_element, frame_element_builder, grab().frame_elements, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "Frame_elementString", Frame_elementString, frame_element_string_builder, grab().frame_element_strings, 0, _logging);
      return;
    }

    // CompoundDatapoint:
    compound_datapoint_builder::compound_datapoint_builder(CompoundDatapoint & obj_,
                                                       datatools::logger::priority prio_)
      : base_generic_builder<CompoundDatapoint>(obj_, prio_) // base_datapoint_builder(obj_, prio_)
    {
      return;
    }

    void compound_datapoint_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_datapoint_builder base_builder(dynamic_cast<BaseDatapoint &>(grab()));
      base_builder(node_);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "CompoundDatapoint", CompoundDatapoint, compound_datapoint_builder, grab().compound_datapoints, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "SimpleDatapoint", SimpleDatapoint, simple_datapoint_builder, grab().simple_datapoints, 0, _logging);
      return;
    }

    // Frames_description:
    frames_description_builder::frames_description_builder(Frames_description & obj_,
                                                           datatools::logger::priority prio_)
      : base_generic_builder<Frames_description>(obj_, prio_)
    {
      return;
    }

    void frames_description_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "SimpleDatapoint", SimpleDatapoint, simple_datapoint_builder, grab().simple_datapoints, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "CompoundDatapoint", CompoundDatapoint, compound_datapoint_builder, grab().compound_datapoints, 0, _logging);
      return;
    }

    // BaseDevice:
    base_device_builder::base_device_builder(BaseDevice & obj_,
                                                 datatools::logger::priority prio_)
      : base_generic_builder<BaseDevice>(obj_, prio_)
    {
      return;
    }

    void base_device_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_MANDATORY_VALUE (node_, "Name", Name,
                                      name_builder, grab().name, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Multiplicity", Multiplicity,
                                      multiplicity_builder, grab().multiplicity, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "StartNumberingMultiplicity", StartNumberingMultiplicity,
                                      start_numbering_multiplicity_builder, grab().start_numbering_multiplicity, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Interface", Interface,
                                      interface_builder, grab().interface, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "DataFrameStructureRef", DataFrameStructureRef,
                                      data_frame_structure_ref_builder, grab().data_frame_structure_ref, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "HardwareConfig", HardwareConfig,
                                      hardware_config_builder, grab().hardware_configs, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "Attribut", Attribut,
                                      attribute_builder, grab().attributes, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "CompoundDatapoint", CompoundDatapoint,
                                      compound_datapoint_builder, grab().compound_datapoints, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "SimpleDatapoint", SimpleDatapoint,
                                      simple_datapoint_builder, grab().simple_datapoints, 0, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE  (node_, "Info", Info,
                                      info_builder, grab().info, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "Info", Info,
                                      info_builder, grab().infos, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE    (node_, "UserInfo", UserInfo,
                                      userinfo_builder, grab().userinfos, 0, _logging);
      return;
    }

    // SimpleDevice:
    simple_device_builder::simple_device_builder(SimpleDevice & obj_,
                                                 datatools::logger::priority prio_)
     : base_generic_builder<SimpleDevice>(obj_, prio_) // ase_device_builder(obj_, prio_)
    {
      return;
    }

    void simple_device_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_device_builder base_builder(dynamic_cast<BaseDevice &>(grab()));
      base_builder(node_);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Plugins",             Plugins, plugins_builder, grab().plugins, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Instruction_set",     Instruction_set, instruction_set_builder, grab().instruction_set, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "DataIdDescriptorRef", DataIdDescriptorRef, data_id_descriptor_ref_builder, grab().data_id_descriptor_ref, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "FileFSMRef",          FileFSMRef, file_fsm_ref_builder, grab().file_fsm_ref, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,   "FSM",                 FSM, fsm_builder, grab().FSMs, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,   "Method",              Method, method_builder, grab().methods, 0, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_, "Watchdog",            Watchdog, watchdog_builder, grab().watchdog, _logging);
      return;
    }

    // Compound device:
    compound_device_builder::compound_device_builder(CompoundDevice & obj_,
                                                     datatools::logger::priority prio_)
      : base_generic_builder<CompoundDevice>(obj_, prio_)
    {
      return;
    }

    void compound_device_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_device_builder base_builder(dynamic_cast<BaseDevice &>(grab()));
      base_builder(node_);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "CompoundDevice", CompoundDevice, compound_device_builder, grab().compound_devices, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_, "SimpleDevice",   SimpleDevice,   simple_device_builder,   grab().simple_devices,   0, _logging);
      return;
    }

    // OPCUA:
    opcua_builder::opcua_builder(OPCUA & opcua_,
                                 datatools::logger::priority prio_)
      : base_generic_builder<OPCUA>(opcua_, prio_)
    {
      return;
    }

    void opcua_builder::operator()(const xercesc::DOMNode * node_)
    {
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "UserInfo",       UserInfo,       userinfo_builder,        grab().userinfos, 0, _logging);
      // VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "OPCUA",          OPCUA,          opcua_builder,           grab().opcuas, 0, _logging);
      VIRE_MOS_BUILD_MANDATORY_VALUE(node_, "Name",           Name,           name_builder,            grab().name, _logging);
      VIRE_MOS_BUILD_OPTIONAL_VALUE(node_,  "ServerPort",     ServerPort,     server_port_builder,     grab().server_port, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "Attribut",       Attribut,       attribute_builder,       grab().attributes, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "FileName",       FileName,       filename_builder,        grab().filenames, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "CompoundDevice", CompoundDevice, compound_device_builder, grab().compound_devices, 0, _logging);
      VIRE_MOS_BUILD_VECTOR_VALUE(node_,    "SimpleDevice",   SimpleDevice,   simple_device_builder,   grab().simple_devices, 0, _logging);
      return;
    }

  } // namespace mos

} // namespace vire
