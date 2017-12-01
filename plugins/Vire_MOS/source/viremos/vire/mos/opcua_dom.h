//! \file opcua_dom.h
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

#ifndef VIRE_MOS_OPCUA_DOM_H
#define VIRE_MOS_OPCUA_DOM_H

// Third Party:
// - Xerces-C++:
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/logger.h>

// This project:
#include <vire/mos/dom_utils.h>
#include <vire/mos/opcua_ast.h>

namespace vire {

  namespace mos {

    // Additional primitive builder:
    VIRE_MOS_DOM_BUILDER_DECLARE(hexa_value_builder, HexaValue);

    // Aliases for primitive builders:
    VIRE_MOS_DOM_BUILDER_ALIAS(value_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(default_value_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(enable_builder, boolean_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(name_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(severity_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(message_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(multiplicity_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(start_numbering_multiplicity_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(data_frame_structure_ref_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(pos_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(nb_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(index_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(id_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(read_id_collector_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(description_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(historizing_builder, boolean_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(acces_level_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(write_mask_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(file_name_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(ip_address_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(acknowledge_builder, boolean_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(array_size_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(address_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(file_mapping_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(min_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(max_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(server_port_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(port_builder, integer_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(config_builder, boolean_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(icd_builder, boolean_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(node_id_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(method_id_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(server_id_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(data_id_descriptor_ref_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(cmd_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(location_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(filename_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(end_delimiter_builder, hexa_value_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(end_instruction_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(file_fsm_ref_builder, string_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(size_builder, hexa_value_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(id_num_builder, hexa_value_builder);
    VIRE_MOS_DOM_BUILDER_ALIAS(timeout_builder, integer_builder);

    // Aliases for enumeration builders:
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(type_builder, Type);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(scope_access_builder, ScopeAccess);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(access_builder, Access);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(interface_builder, Interface);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(validity_builder, Validity);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(monitoring_rate_builder, MonitoringRate);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(connection_builder, Connection);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(state_fsm_builder, StateFSM);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(name_fsm_builder, NameFSM);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(name_hardware_config_builder, NameHardwareConfig);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(name_alarm_builder, NameAlarm);
    VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(name_space_level_builder, NameSpaceLevel);

    // AST builders:
    VIRE_MOS_DOM_BUILDER_DECLARE(instruction_builder, Instruction);
    VIRE_MOS_DOM_BUILDER_DECLARE(instruction_set_builder, Instruction_set);
    VIRE_MOS_DOM_BUILDER_DECLARE(base_error_alarm_builder, Base_ErrorAlarm);
    VIRE_MOS_DOM_BUILDER_DECLARE(alarm_builder, Alarm);
    VIRE_MOS_DOM_BUILDER_DECLARE(element_array_builder, ElementArray);
    VIRE_MOS_DOM_BUILDER_DECLARE(event_builder, Event);
    VIRE_MOS_DOM_BUILDER_DECLARE(unit_builder, Unit);
    VIRE_MOS_DOM_BUILDER_DECLARE(attribute_builder, Attribut);
    VIRE_MOS_DOM_BUILDER_DECLARE(userinfo_builder, UserInfo);
    VIRE_MOS_DOM_BUILDER_DECLARE(variable_builder, Variable);
    VIRE_MOS_DOM_BUILDER_DECLARE(range_builder, Range);
    VIRE_MOS_DOM_BUILDER_DECLARE(info_builder, Info);
    VIRE_MOS_DOM_BUILDER_DECLARE(sequence_builder, Sequence);
    VIRE_MOS_DOM_BUILDER_DECLARE(device_instruction_builder, DeviceInstruction);
    VIRE_MOS_DOM_BUILDER_DECLARE(argument_builder, Argument);
    VIRE_MOS_DOM_BUILDER_DECLARE(base_hidf_builder, base_hidf);
    VIRE_MOS_DOM_BUILDER_DECLARE(data_frame_structure_builder, DataFrameStructure);
    VIRE_MOS_DOM_BUILDER_DECLARE(frame_element_string_builder, Frame_elementString);
    VIRE_MOS_DOM_BUILDER_DECLARE(frame_element_builder, Frame_element);
    VIRE_MOS_DOM_BUILDER_DECLARE(plugins_builder, Plugins);
    VIRE_MOS_DOM_BUILDER_DECLARE(frame_definition_builder, Frame_definition);
    VIRE_MOS_DOM_BUILDER_DECLARE(fsm_builder, FSM);
    VIRE_MOS_DOM_BUILDER_DECLARE(hardware_config_builder, HardwareConfig);
    VIRE_MOS_DOM_BUILDER_DECLARE(alarm_method_builder, AlarmMethod);
    VIRE_MOS_DOM_BUILDER_DECLARE(method_builder, Method);
    VIRE_MOS_DOM_BUILDER_DECLARE(watchdog_builder, Watchdog);
    VIRE_MOS_DOM_BUILDER_DECLARE(asynchronous_builder, Asynchronous);
    VIRE_MOS_DOM_BUILDER_DECLARE(frames_description_builder, Frames_description);

    // Datapoints;
    VIRE_MOS_DOM_BUILDER_DECLARE(base_datapoint_builder, BaseDatapoint);
    VIRE_MOS_DOM_BUILDER_DECLARE(simple_datapoint_builder, SimpleDatapoint);
    VIRE_MOS_DOM_BUILDER_DECLARE(compound_datapoint_builder, CompoundDatapoint);

    // Devices:
    VIRE_MOS_DOM_BUILDER_DECLARE(base_device_builder, BaseDevice);
    VIRE_MOS_DOM_BUILDER_DECLARE(simple_device_builder, SimpleDevice);
    VIRE_MOS_DOM_BUILDER_DECLARE(compound_device_builder, CompoundDevice);

    // OPCUA:
    VIRE_MOS_DOM_BUILDER_DECLARE(opcua_builder, OPCUA);

  } // namespace mos

} // namespace vire

#define VIRE_MOS_BUILD_MANDATORY_VALUE(DomNode,ValueName,ValueType,BuilderType,Target,Logging) \
  {                                                                     \
    const std::string node_name =  xercesc::XMLString::transcode(DomNode->getNodeName()); \
    DT_LOG_DEBUG((Logging),"Searching mandatory child node '" << ValueName << "' from node '" << node_name << "'"); \
    const xercesc::DOMNodeList * node_children = DomNode->getChildNodes(); \
    int counter = 0;                                                    \
    for (int inode = 0; inode < node_children->getLength(); inode++) {  \
      const xercesc::DOMNode * child_node = node_children->item(inode); \
      if (child_node->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue; \
      DT_LOG_TRACE((Logging),"Child node '" << xercesc::XMLString::transcode(child_node->getNodeName()) << "' from node '" << node_name << "'"); \
      if (xercesc::XMLString::transcode(child_node->getNodeName()) == std::string(ValueName)) { \
        DT_LOG_DEBUG((Logging),"Found mandatory child node with name = '" << ValueName << "'"); \
        BuilderType builder(Target, Logging);                           \
        try {                                                           \
          builder(child_node);                                          \
          counter++;                                                    \
          break;                                                        \
        }                                                               \
        catch (std::exception & error) {                                \
          DT_THROW(std::logic_error, "Cannot build value '" << BOOST_PP_STRINGIZE(ValueType) << "' from mandatory child node named '" << ValueName << "': \n\t" << error.what()); \
        }                                                               \
      }                                                                 \
    }                                                                   \
    DT_THROW_IF(counter != 1, std::logic_error, "Missing mandatory value from from mandatory child node named '" << ValueName << "'!"); \
    DT_LOG_DEBUG((Logging), "Mandatory child node '" << ValueName << "' from node '" << node_name << "' successful.\n"); \
  }                                                                     \
  /**/

#define VIRE_MOS_BUILD_OPTIONAL_VALUE(DomNode,ValueName,ValueType,BuilderType,Target,Logging) \
  {                                                                     \
    const std::string node_name =  xercesc::XMLString::transcode(DomNode->getNodeName()); \
    DT_LOG_DEBUG((Logging),"Searching optional child node '" << ValueName << "' from node '" << node_name << "'"); \
    const xercesc::DOMNodeList * node_children = DomNode->getChildNodes(); \
    int counter = 0;                                                    \
    for (int inode = 0; inode < node_children->getLength(); inode++) {  \
      const xercesc::DOMNode * child_node = node_children->item(inode); \
      if (child_node->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue; \
      DT_LOG_TRACE((Logging),"Child optional node '" << xercesc::XMLString::transcode(child_node->getNodeName()) << "' from node '" << node_name << "'"); \
      if (xercesc::XMLString::transcode(child_node->getNodeName()) == std::string(ValueName)) { \
        DT_LOG_DEBUG((Logging),"Found child node with name = '" << ValueName << "'"); \
        optional_builder<ValueType, BuilderType> builder(Target, Logging); \
        try {                                                           \
          builder(child_node);                                          \
          counter++;                                                    \
          break;                                                        \
        }                                                               \
        catch (std::exception & error) {                                \
          DT_THROW(std::logic_error, "Cannot build optional value '" << BOOST_PP_STRINGIZE(ValueType) << "' from optional child node named '" << ValueName << "': \n\t" << error.what()); \
        }                                                               \
        counter++;                                                      \
      }                                                                 \
    }                                                                   \
    DT_THROW_IF(counter > 1, std::logic_error, "Too many values from optional child node named  '" << ValueName << "' !"); \
    DT_LOG_DEBUG((Logging), "Optional child node '" << ValueName << "' from node '" << node_name << "' successful.\n"); \
  }                                                                     \
  /**/

#define VIRE_MOS_BUILD_VECTOR_VALUE(DomNode,ValueName,ValueType,BuilderType,Target,Min,Logging) \
  {                                                                     \
    const std::string node_name =  xercesc::XMLString::transcode(DomNode->getNodeName()); \
    DT_LOG_DEBUG((Logging),"Searching vector element child nodes '" << ValueName << "' from node '" << node_name << "'"); \
    const xercesc::DOMNodeList * node_children = DomNode->getChildNodes(); \
    int counter = 0;                                                    \
    for (int inode = 0; inode < node_children->getLength(); inode++) {  \
      const xercesc::DOMNode * child_node = node_children->item(inode); \
      if (child_node->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) continue; \
      DT_LOG_TRACE((Logging),"Child node '" << xercesc::XMLString::transcode(child_node->getNodeName()) << "' from node '" << node_name << "'"); \
      if (xercesc::XMLString::transcode(child_node->getNodeName()) == std::string(ValueName)) { \
        DT_LOG_DEBUG((Logging),"Found vector element child node with name = '" << ValueName << "'"); \
        vector_builder<ValueType, BuilderType> builder(Target, Logging); \
        try {                                                           \
          builder(child_node);                                          \
          counter++;                                                    \
        }                                                               \
        catch (std::exception & error) {                                \
          DT_THROW(std::logic_error, "Cannot build vector element value '" << BOOST_PP_STRINGIZE(ValueType) << "' from vector element child node named '" << ValueName << "': \n\t" << error.what()); \
        }                                                               \
      }                                                                 \
    }                                                                   \
    DT_THROW_IF(counter < Min, std::logic_error, "Invalid vector size from vector element child node named  '" << ValueName << "' !"); \
    DT_LOG_DEBUG((Logging), "Vector element child nodes '" << ValueName << "' from node '" << node_name << "' successful.\n"); \
  }                                                                     \
  /**/

#endif // VIRE_MOS_OPCUA_DOM_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
