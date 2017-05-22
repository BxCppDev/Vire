// dom_utils.cc
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
#include <vire/mos/dom_utils.h>

// Standart Library:
#include <iostream>
#include <sstream>

// Third Party:
// - Xerces-C++:
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/util/XMLString.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

namespace vire {

  namespace mos {

    // Base builder:
    base_builder::base_builder(datatools::logger::priority prio_)
    {
      _logging = prio_;
      return;
    }

    base_builder::~base_builder()
    {
      return;
    }

    void base_builder::set_logging(datatools::logger::priority prio_)
    {
      _logging = prio_;
      return;
    }

    datatools::logger::priority base_builder::get_logging() const
    {
      return _logging;
    }

    // Boolean:
    boolean_builder::boolean_builder(bool & value_,
                                     datatools::logger::priority prio_)
      : base_generic_builder<bool>(value_, prio_)
    {
      return;
    }

    boolean_builder::~boolean_builder()
    {
      return;
    }

    void boolean_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_generic_builder<bool>::grab() = false;
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
          bool val = false;
          if (val_str == "true" || val_str == "1") {
            val = true;
          } else if (val_str == "false" || val_str == "0") {
            val = false;
          } else {
            DT_THROW(std::domain_error, "Invalid boolean value '" << val_str << "'!");
          }
          base_generic_builder<bool>::grab() = val;
          success = true;
          break;
        }
      }
      DT_THROW_IF(! success, std::logic_error,
                  "Cannot parse a boolean from text node '" << xercesc::XMLString::transcode(node_name) << "'");
      return;
    }

    // Integer:
    integer_builder::integer_builder(int & value_,
                                     datatools::logger::priority prio_)
      : base_generic_builder<int>(value_, prio_)
    {
      return;
    }

    integer_builder::~integer_builder()
    {
      return;
    }

    void integer_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_generic_builder<int>::grab() = 0;
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
          int val;
          iss >> val;
          DT_THROW_IF (!iss, std::logic_error, "Invalid integer value!");
          base_generic_builder<int>::grab() = val;
          success = true;
          break;
        }
      }
      DT_THROW_IF(! success, std::logic_error,
                  "Cannot parse an integer from text node '" << xercesc::XMLString::transcode(node_name) << "'");
      return;
    }

    // Float:
    float_builder::float_builder(float & value_,
                                     datatools::logger::priority prio_)
      : base_generic_builder<float>(value_, prio_)
    {
      return;
    }

    float_builder::~float_builder()
    {
      return;
    }

    void float_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_generic_builder<float>::grab() = 0;
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
          float val;
          iss >> val;
          DT_THROW_IF (!iss, std::logic_error, "Invalid float value!");
          base_generic_builder<float>::grab() = val;
          success = true;
          break;
        }
      }
      DT_THROW_IF(! success, std::logic_error,
                  "Cannot parse a float from text node '" << xercesc::XMLString::transcode(node_name) << "'");
      return;
    }

    // Double:
    double_builder::double_builder(double & value_,
                                     datatools::logger::priority prio_)
      : base_generic_builder<double>(value_, prio_)
    {
      return;
    }

    double_builder::~double_builder()
    {
      return;
    }

    void double_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_generic_builder<double>::grab() = 0;
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
          double val;
          iss >> val;
          DT_THROW_IF (!iss, std::logic_error, "Invalid double value!");
          base_generic_builder<double>::grab() = val;
          success = true;
          break;
        }
      }
      DT_THROW_IF(! success, std::logic_error,
                  "Cannot parse a double from text node '" << xercesc::XMLString::transcode(node_name) << "'");
      return;
    }

    // String:
    string_builder::string_builder(std::string & value_,
                                   datatools::logger::priority prio_)
      : base_generic_builder<std::string>(value_, prio_)
    {
      return;
    }

    string_builder::~string_builder()
    {
      return;
    }

    void string_builder::operator()(const xercesc::DOMNode * node_)
    {
      base_generic_builder<std::string>::grab().clear();
      bool success = false;
      std::string node_name = xercesc::XMLString::transcode(node_->getNodeName());
      xercesc::DOMNode::NodeType node_type = node_->getNodeType();
      const xercesc::DOMNodeList * node_children = node_->getChildNodes();
      if (node_children->getLength() == 0) {
        // String is empty.
        base_generic_builder<std::string>::grab() = "";
        // std::cerr << "DEVEL: " << "string_builder::operator()(...): "
        //           << "No child nodes for node '" << node_name << "'!"
        //           << std::endl;
        return;
      }
      // if (node_name == "Value") {
      //   std::cerr << "DEVEL: " << "string_builder::operator()(...): "
      //             << "Value  #children=" << node_children->getLength()
      //             << std::endl;
      // }
      for (int inode = 0; inode < node_children->getLength(); inode++) {
        const xercesc::DOMNode * child_node = node_children->item(inode);
        std::string child_node_name = xercesc::XMLString::transcode(child_node->getNodeName());
        xercesc::DOMNode::NodeType child_node_type = child_node->getNodeType();
        if (child_node_type != xercesc::DOMNode::TEXT_NODE) {
          continue;
        } else {
          // Fetch string from a text node:
          const xercesc::DOMText * text_node
            = dynamic_cast<const xercesc::DOMText *>(child_node);
          base_generic_builder<std::string>::grab()
            = xercesc::XMLString::transcode(text_node->getData());
          success = true;
          break;
        }
      }
      // if (!success && node_name == "Value") {
      //   std::cerr << "DEVEL: " << "string_builder::operator()(...): "
      //             << "Failure node type=" << node_type << " "
      //             << std::endl;
      // }
      DT_THROW_IF(! success, std::logic_error,
                  "Cannot parse a string from text node '" << node_name << "'");
      return;
    }

  } // namespace mos

} // namespace vire
