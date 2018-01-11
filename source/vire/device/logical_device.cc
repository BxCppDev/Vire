//! \file vire/device/logical_device.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/logical_device.h>

// - Bayeux/datatools
#include <datatools/exception.h>

// This project
#include <vire/device/device_types.h>
#include <vire/device/base_device_model.h>
#include <vire/device/physical_device.h>
#include <vire/device/physical_port.h>
#include <vire/device/physical_link.h>
#include <vire/device/logical_port.h>
#include <vire/device/base_port_model.h>

namespace vire {

  namespace device {

    logical_device::logical_device()
    {
      _initialized_ = false;
      _model_ = 0;
      return;
    }

    logical_device::~logical_device()
    {
      if (_initialized_) {
        reset();
      }
      return;
    }

    bool logical_device::has_model() const
    {
      return _model_ != 0;
    }

    void logical_device::set_model(const base_device_model & m_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' is locked !");
      _model_ = &m_;
      return;
    }

    const base_device_model & logical_device::get_model() const
    {
      DT_THROW_IF(! has_model(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not have an associated model !");
      return *_model_;
    }

    void logical_device::build_daughters_labels(std::vector<std::string>& labels_) const
    {
      labels_.clear();
      labels_.reserve(_daughters_.size());
      for (daughters_dict_type::const_iterator i = _daughters_.begin();
           i != _daughters_.end();
           i++) {
        labels_.push_back(i->first);
      }
      return;
    }

    void logical_device::build_ports_labels(std::vector<std::string>& labels_) const
    {
      labels_.clear();
      labels_.reserve(_ports_.size());
      for (ports_dict_type::const_iterator i = _ports_.begin();
           i != _ports_.end();
           i++) {
        labels_.push_back(i->first);
      }
      return;
    }

    void logical_device::build_links_labels(std::vector<std::string>& labels_) const
    {
      labels_.clear();
      labels_.reserve(_links_.size());
      for (links_dict_type::const_iterator i = _links_.begin();
           i != _links_.end();
           i++) {
        labels_.push_back(i->first);
      }
      return;
    }

    bool logical_device::has_daughter(const std::string & daughter_label_) const
    {
      return _daughters_.find(daughter_label_) != _daughters_.end();
    }

    bool logical_device::has_port(const std::string & port_label_) const
    {
      return _ports_.find(port_label_) != _ports_.end();
    }

    bool logical_device::has_link(const std::string & link_label_) const
    {
      return _links_.find(link_label_) != _links_.end();
    }

    void logical_device::add_daughter(const std::string & daughter_label_,
                                      const physical_device & physical_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' is locked !");
      DT_THROW_IF(daughter_label_.empty(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not accept a daughter physical compoent with an empty label !");
      DT_THROW_IF(has_daughter(daughter_label_),
                  std::logic_error,
                  "Logical device '" << get_name() << "' already has a daughter physical device with label '"
                  << daughter_label_ << "' !");
      if (physical_.has_mother_logical()) {
        DT_THROW_IF(&physical_.get_mother_logical() != this,
                    std::logic_error,
                    "Logical device '" << get_name() << "' : physical '"
                    << physical_.get_name() << "' already has a mother logical device !"
                    );
      }
      _daughters_[daughter_label_] = &physical_;
      {
        physical_device & phys = const_cast<physical_device &>(physical_);
        phys.set_mother_logical(*this);
      }
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void logical_device::add_port(const std::string & port_label_,
                                  const physical_port & physical_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' is locked !");
      DT_THROW_IF(port_label_.empty(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not accept a physical port with an empty label !");
      DT_THROW_IF(has_port(port_label_),
                  std::logic_error,
                  "Logical device '" << get_name() << "' already has a physical port with label '"
                  << port_label_ << "' !");
      if (physical_.has_mother_logical()) {
        DT_THROW_IF(&physical_.get_mother_logical() != this,
                    std::logic_error,
                    "Logical device '" << get_name() << "' : physical '"
                    << physical_.get_name() << "' already has a mother logical device !"
                    );
      }
      _ports_[port_label_] = &physical_;
      {
        physical_port & phys = const_cast<physical_port &>(physical_);
        phys.set_mother_logical(*this);
      }
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void logical_device::add_link(const std::string & link_label_,
                                  const physical_link & physical_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' is locked !");
      DT_THROW_IF(link_label_.empty(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not accept a physical link with an empty label !");
      DT_THROW_IF(has_link(link_label_),
                  std::logic_error,
                  "Logical device '" << get_name() << "' already has a physical link with label '"
                  << link_label_ << "' !");
      if (physical_.has_mother_logical()) {
        DT_THROW_IF(&physical_.get_mother_logical() != this,
                    std::logic_error,
                    "Logical device '" << get_name() << "' : physical '"
                    << physical_.get_name() << "' already has a mother logical device !"
                    );
      }
      _links_[link_label_] = &physical_;
      {
        physical_link & phys = const_cast<physical_link &>(physical_);
        phys.set_mother_logical(*this);
      }
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void logical_device::remove_daughter(const std::string & daughter_label_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      DT_LOG_TRACE(get_logging_priority(),
                   "Logical device '" << get_name()
                   << "' : remove device with label='" << daughter_label_ << "'");
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' is locked !");
      DT_THROW_IF(daughter_label_.empty(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' : Invalid empty daughter device label !");
      DT_THROW_IF(!has_daughter(daughter_label_),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not have a daughter physical device with label '"
                  << daughter_label_ << "' !");
      daughters_dict_type::iterator found = _daughters_.find(daughter_label_);
      _daughters_.erase(found);
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void logical_device::remove_port(const std::string & port_label_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      DT_LOG_TRACE(get_logging_priority(),
                   "Logical device '" << get_name()
                   << "' : remove port with label='" << port_label_ << "'");
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' is locked !");
      DT_THROW_IF(port_label_.empty(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' : Invalid empty port label !");
      DT_THROW_IF(!has_port(port_label_),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not have a physical port with label '"
                  << port_label_ << "' !");
      ports_dict_type::iterator found = _ports_.find(port_label_);
      _ports_.erase(found);
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void logical_device::remove_link(const std::string & link_label_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      DT_LOG_TRACE(get_logging_priority(),
                   "Logical device '" << get_name()
                   << "' : remove link with label='" << link_label_ << "'");
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' is locked !");
      DT_THROW_IF(link_label_.empty(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' : Invalid empty link label !");
      DT_THROW_IF(!has_link(link_label_),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not have a physical link with label '"
                  << link_label_ << "' !");
      links_dict_type::iterator found = _links_.find(link_label_);
      _links_.erase(found);
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    const logical_device::daughters_dict_type &
    logical_device::get_daughters() const
    {
      return _daughters_;
    }

    const logical_device::ports_dict_type &
    logical_device::get_ports() const
    {
      return _ports_;
    }

    const logical_device::links_dict_type &
    logical_device::get_links() const
    {
      return _links_;
    }

    const physical_device &
    logical_device::get_daughter(const std::string & daughter_label_) const
    {
      daughters_dict_type::const_iterator found
        = _daughters_.find(daughter_label_);
      DT_THROW_IF(found == _daughters_.end(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not have a daughter physical device with label '"
                  << daughter_label_ << "' !");
      return *found->second;
    }

    const physical_port &
    logical_device::get_port(const std::string & port_label_) const
    {
      ports_dict_type::const_iterator found
        = _ports_.find(port_label_);
      DT_THROW_IF(found == _ports_.end(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not have a physical port with label '"
                  << port_label_ << "' !");
      return *found->second;
    }

    const physical_link &
    logical_device::get_link(const std::string & link_label_) const
    {
      links_dict_type::const_iterator found
        = _links_.find(link_label_);
      DT_THROW_IF(found == _links_.end(),
                  std::logic_error,
                  "Logical device '" << get_name() << "' does not have a physical link with label '"
                  << link_label_ << "' !");
      return *found->second;
    }

    bool logical_device::is_initialized() const
    {
      return _initialized_;
    }

    void logical_device::initialize()
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Logical device '"
                  << get_name() << "' is already initialized !");
      _initialized_ = true;
      return;
    }

    void logical_device::reset()
    {
      DT_THROW_IF(! is_initialized(), std::logic_error, "Logical device '"
                  << get_name() << "' is not initialized !");
      _initialized_ = false;
      _model_ = 0;
      _daughters_.clear();
      _ports_.clear();
      _links_.clear();
      return;
    }

    void logical_device::print_tree(std::ostream & out_,
                                    const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);
      this->enriched_base::tree_dump(out_, popts.title, popts.indent, true);
      bool print_daughters_list = options_.get<bool>("list_daughters", false);
      bool print_ports_list = options_.get<bool>("list_ports", false);
      bool print_links_list = options_.get<bool>("list_links", false);

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Initialized : " << std::boolalpha << _initialized_ << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Model : ";
      if (_model_) {
        out_ << "'" << _model_->get_name() << "'";
        out_ << " (as '" << _model_->system_factory_auto_registration_id() << "' of type '"
             << vire::device::get_device_type_label(_model_->get_type()) << "')";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Physical ports : ";
        if (_ports_.size() == 0) {
          out_ << "<none>";
        } else {
          out_ << '[' << _ports_.size() << ']';
        }
        out_ << std::endl;
        if (print_ports_list) {
          for (ports_dict_type::const_iterator i = _ports_.begin();
               i != _ports_.end();
               i++) {
            ports_dict_type::const_iterator j = i;
            j++;
            out_ << popts.indent;
            out_ << datatools::i_tree_dumpable::skip_tag;
            if (j == _ports_.end()) {
              out_ << datatools::i_tree_dumpable::last_tag;
            } else {
              out_ << datatools::i_tree_dumpable::tag;
            }
            const std::string & port_label = i->first;
            const physical_port & port_phys = *i->second;
            out_ << "'" << port_label << "' ";
            if (port_phys.has_logical()) {
              out_ << "(as logical '" << port_phys.get_logical().get_name() << "'";
              if (port_phys.get_logical().has_model()) {
                out_ << " of type '" << get_device_type_label((device_type) port_phys.get_logical().get_model().get_type()) << "'";
              }
              out_ << ")";
            } else {
              out_ << "<no logical>";
            }
            out_ << std::endl;
          }
        }
      }

      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Daughter physical devices : ";
        if (_daughters_.size() == 0) {
          out_ << "<none>";
        } else {
          out_ << '[' << _daughters_.size() << ']';
        }
        out_ << std::endl;
        if (print_daughters_list) {
          for (daughters_dict_type::const_iterator i = _daughters_.begin();
               i != _daughters_.end();
               i++) {
            daughters_dict_type::const_iterator j = i;
            j++;
            out_ << popts.indent;
            out_ << datatools::i_tree_dumpable::skip_tag;
            if (j == _daughters_.end()) {
              out_ <<  datatools::i_tree_dumpable::last_tag;
            } else {
              out_ <<  datatools::i_tree_dumpable::tag;
            }
            const std::string & daughter_label = i->first;
            const physical_device & daughter_phys = *i->second;
            out_ << "'" << daughter_label << "' ";
            if (daughter_phys.has_logical()) {
              out_ << "(as logical '" << daughter_phys.get_logical().get_name() << "'";
              if (daughter_phys.get_logical().has_model()) {
                out_ << " of type '" << get_device_type_label((device_type) daughter_phys.get_logical().get_model().get_type()) << "'";
              }
              out_ << ")";
            } else {
              out_ << "<no logical>";
            }
            out_ << std::endl;
          }
        }
      }

      {
        out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
             << "Physical links : ";
        if (_links_.size() == 0) {
          out_ << "<none>";
        } else {
          out_ << '[' << _links_.size() << ']';
        }
        out_ << std::endl;
        if (print_links_list) {
          for (links_dict_type::const_iterator i = _links_.begin();
               i != _links_.end();
               i++) {
            links_dict_type::const_iterator j = i;
            j++;
            out_ << popts.indent;
            out_ << datatools::i_tree_dumpable::inherit_skip_tag(popts.inherit);
            if (j == _links_.end()) {
              out_ <<  datatools::i_tree_dumpable::last_tag;
            } else {
              out_ <<  datatools::i_tree_dumpable::tag;
            }
            const std::string & link_label = i->first;
            const physical_link & link_phys = *i->second;
            out_ << "'" << link_label << "' ";
            out_ << std::endl;
          }
        }
      }

      return;
    }

  } // end of namespace device

} // end of namespace vire
