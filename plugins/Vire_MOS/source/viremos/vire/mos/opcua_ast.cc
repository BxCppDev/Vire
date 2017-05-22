// opcua_ast.cc
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
#include <vire/mos/opcua_ast.h>

// Standard library:
#include <sstream>

// Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>

namespace vire {

  namespace mos {

    std::ostream & operator<<(std::ostream & out_, const HexaValue & hex_)
    {
      std::ostringstream oss;
      oss << std::hex << hex_.bits;
      out_ << "(bits=" << oss.str() << ")";
      return out_;
    }

    std::ostream & operator<<(std::ostream & out_, const ElementArray & item_)
    {
      out_ << "(pos='" << item_.pos
           << ";value='" << item_.value
           << "')";
      return out_;
    }

    std::ostream & operator<<(std::ostream & out_, const Event & e_)
    {
      out_ << "(message='" << e_.message
           << "')";
      return out_;
    }

    std::ostream & operator<<(std::ostream & out_, const UserInfo & uinfo_)
    {
      out_ << "(name='" << uinfo_.name
           << ";value='" << uinfo_.value
           << "')";
      return out_;
    }

    std::ostream & operator<<(std::ostream & out_, const Attribut & attr_)
    {
      out_ << "(name='" << attr_.name
           << ";value='" << attr_.value
           << "')";
      return out_;
    }

    std::ostream & operator<<(std::ostream & out_, const Range & r_)
    {
      out_ << "(min=";
      if (r_.min) {
        out_ << "'" << r_.min.get() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << ";max=";
      if (r_.max) {
        out_ << "'" << r_.max.get() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << "')";
      return out_;
    }

    std::ostream & operator<<(std::ostream & out_, const Frame_elementString & fes_)
    {
      out_ << "(name='" << fes_.name << "'"
           << ";type=" << fes_.type
        // << ";end_delimiter=" << (int) fes_.end_delimiter
           << ")";
      return out_;
    }

    std::ostream & operator<<(std::ostream & out_, const Frame_element & fe_)
    {
      out_ << "(name='" << fe_.name << "'"
           << ";type=" << fe_.type
           << ";index=" << fe_.type << ")";
      return out_;
    }

    std::ostream & operator<<(std::ostream & out_, const Asynchronous & as_)
    {
      out_ << "(enable=" << as_.enable
           << ";timeout=" << as_.timeout
           << ";end_instruction='" << as_.end_instruction << "')";
      return out_;
    }

    void Info::tree_dump(std::ostream & out_,
                         const std::string & title_,
                         const std::string & indent_,
                         bool inherit_) const
    {
       if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_<< datatools::i_tree_dumpable::tag
           << "Config: ";
      optional_print_yesno<Config>(config, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "ScopeAccess: ";
      optional_print_yesno<ScopeAccess>(scope_access, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::last_tag
           << "ICD: ";
      optional_print_yesno<ICD>(icd, out_);
      out_ << std::endl;

      return;
    }

    bool Info::operator==(const Info & i_) const
    {
      return config == i_.config && icd == i_.icd && scope_access == i_.scope_access;
    }

    std::ostream & operator<<(std::ostream & out_, const Plugins & plugins_)
    {
      out_ << "(name='" << plugins_.name << ";location=" << plugins_.location << "')";
      return out_;
    }

    Instruction::~Instruction() {}

    void Instruction::tree_dump(std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_,
                                bool inherit_) const
    {
      if (! title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name  : '" << name << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Cmd   : '";
      optional_print<Cmd>(cmd, out_);
      out_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Acknowledge  : ";
      optional_print<Acknowledge>(acknowledge, out_);
      out_ << std::endl;

      // XXX
      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Asynchronous : ";
      optional_print<Asynchronous>(asynchronous, out_);
      out_ << std::endl;

      return;
    }

    Frame_elementString::~Frame_elementString()
    {
      return;
    }

    Type Frame_elementString::get_type() const
    {
      return type;
    }

    const boost::optional<Unit> & Frame_elementString::get_unit() const
    {
      return unit;
    }

    Frame_element::~Frame_element()
    {
      return;
    }

    Type Frame_element::get_type() const
    {
      return type;
    }

    const boost::optional<Unit> & Frame_element::get_unit() const
    {
      return unit;
    }

    Variable::~Variable()
    {
      return;
    }

    Type Variable::get_type() const
    {
      return type;
    }

    const boost::optional<Unit> & Variable::get_unit() const
    {
      return unit;
    }

    Instruction_set::~Instruction_set() {}

    void Instruction_set::tree_dump(std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_,
                                bool inherit_) const
    {
      if (! title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Instructions  : " << instructions.size() << std::endl;
      {
        unsigned int counter = 0;
        for (const auto & instruction : instructions) {
          out_ << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
          if (++counter == instructions.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "Instruction : name='" << instruction.name << "'" << std::endl;
        }
      }

      return;
    }

    Base_ErrorAlarm::~Base_ErrorAlarm() {}

    void Base_ErrorAlarm::tree_dump(std::ostream & out_,
                                    const std::string & title_,
                                    const std::string & indent_,
                                    bool inherit_) const
    {
      if (title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      // out_ << indent_ << datatools::i_tree_dumpable::tag
      //      << "Instructions  : " << instructions.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Value  : ";
      out_ << value;
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Enable  : ";
      optional_print<Enable>(enable, out_);
      out_ << "" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Acknowledge  : ";
      optional_print<Acknowledge>(acknowledge, out_);
      out_ << std::endl;

      return;
    }

    BaseDevice::~BaseDevice() {}

    const std::vector<SimpleDatapoint> & BaseDevice::get_simple_datapoints() const
    {
      return simple_datapoints;
    }

    const std::vector<CompoundDatapoint> & BaseDevice::get_compound_datapoints() const
    {
      return compound_datapoints;
    }

    const std::string & BaseDevice::get_name() const
    {
      return name;
    }

    const std::vector<UserInfo> & BaseDevice::get_userinfos() const
    {
      return userinfos;
    }

    const std::vector<Attribut> & BaseDevice::get_attributes() const
    {
      return attributes;
    }

    // virtual
    const boost::optional<Info> & BaseDevice::get_info() const
    {
      return info;
    }

    const boost::optional<Multiplicity> & BaseDevice::get_multiplicity() const
    {
      return multiplicity;
    }

    void BaseDevice::tree_dump(std::ostream & out_,
                               const std::string & title_,
                               const std::string & indent_,
                               bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name  : '" << name << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Multiplicity   : ";
      optional_print<Multiplicity>(multiplicity, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "StartNumberingMultiplicity : ";
      optional_print<StartNumberingMultiplicity>(start_numbering_multiplicity, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Interface   : '";
      optional_print<Interface>(interface, out_);
      out_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "DataFrameStructureRef  : '";
      optional_print<DataFrameStructureRef>(data_frame_structure_ref, out_);
      out_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Hardware configs : " << hardware_configs.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Attributes : " << attributes.size() << std::endl;
      {
        unsigned int counter = 0;
        for (const auto & attribute : attributes) {
          out_ << indent_ << datatools::i_tree_dumpable::skip_tag;
          if (++counter == attributes.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "Attribute : " << attribute << std::endl;
        }
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Compound datapoints : " << compound_datapoints.size() << std::endl;
      {
        unsigned int counter = 0;
        for (const auto & cdatapoint : compound_datapoints) {
          out_ << indent_ << datatools::i_tree_dumpable::skip_tag;
          if (++counter == compound_datapoints.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "Compound datapoint : name='" << cdatapoint.name << "'" << std::endl;
        }
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Simple datapoints : " << simple_datapoints.size() << std::endl;
      {
        unsigned int counter = 0;
        for (const auto & sdatapoint : simple_datapoints) {
          out_ << indent_ << datatools::i_tree_dumpable::skip_tag;
          if (++counter == simple_datapoints.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "Simple datapoint : name='" << sdatapoint.name << "'" << std::endl;
        }
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Info   : ";
      optional_print_yesno<Info>(info, out_);
      out_ << std::endl;
      if (info) {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
        info.get().tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "User infos : " << userinfos.size() << std::endl;
      {
        unsigned int counter = 0;
        for (const auto & userinfo : userinfos) {
          out_ << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
          if (++counter == userinfos.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "User info : " << userinfo << std::endl;
        }
      }

      return;
    }

    SimpleDevice::~SimpleDevice() {}

    const std::vector<Method> & SimpleDevice::get_methods() const
    {
      return methods;
    }

    void SimpleDevice::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      this->BaseDevice::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Plugins : ";
      optional_print<Plugins>(plugins, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Data ID descriptor ref : ";
      optional_print<DataIdDescriptorRef>(data_id_descriptor_ref, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Instruction set : ";
      if (instruction_set) {
        out_ << "yes";
      } else {
        out_ << "<not set>";
      }
      out_ << std::endl;
      if (instruction_set) {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
        instruction_set.get().tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "FSMs : " << FSMs.size()
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Methods : " << methods.size()
           << std::endl;
      {
        int counter = 0;
        for (const auto & method : methods) {
          out_ << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
          if (++counter == methods.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "Method '" << method.name << "'";
          out_ << std::endl;
        }
      }

      return;
    }

    CompoundDevice::~CompoundDevice() {}

    const std::vector<SimpleDevice> & CompoundDevice::get_simple_devices() const
    {
      return simple_devices;
    }

    const std::vector<CompoundDevice> & CompoundDevice::get_compound_devices() const
    {
      return compound_devices;
    }

    void CompoundDevice::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      this->BaseDevice::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Simple devices: " << simple_devices.size()
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Compound devices: " << compound_devices.size()
           << std::endl;

      return;
    }

    OPCUA::~OPCUA() {}

    const std::string & OPCUA::get_name() const
    {
      return name;
    }

    const std::vector<UserInfo> & OPCUA::get_userinfos() const
    {
      return userinfos;
    }

    const std::vector<Attribut> & OPCUA::get_attributes() const
    {
      return attributes;
    }

    const std::vector<SimpleDevice> & OPCUA::get_simple_devices() const
    {
      return simple_devices;
    }

    const std::vector<CompoundDevice> & OPCUA::get_compound_devices() const
    {
      return compound_devices;
    }

    void OPCUA::tree_dump(std::ostream & out_,
                               const std::string & title_,
                               const std::string & indent_,
                               bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name  : '" << name << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Server port : ";
      optional_print<ServerPort>(server_port, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Attributes : " << attributes.size() << std::endl;
      {
        unsigned int counter = 0;
        for (const auto & attribute : attributes) {
          out_ << indent_ << datatools::i_tree_dumpable::skip_tag;
          if (++counter == attributes.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "Attribute : " << attribute << std::endl;
        }
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Filenames   : " << filenames.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Simple devices   : " << simple_devices.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Compound devices   : " << compound_devices.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "User infos : " << userinfos.size() << std::endl;
      {
        unsigned int counter = 0;
        for (const auto & userinfo : userinfos) {
          out_ << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
          if (++counter == userinfos.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "User info : " << userinfo << std::endl;
        }
      }

      return;
    }

    CompoundDatapoint::~CompoundDatapoint() {}

    const std::vector<SimpleDatapoint> & CompoundDatapoint::get_simple_datapoints() const
    {
      return simple_datapoints;
    }

    const std::vector<CompoundDatapoint> & CompoundDatapoint::get_compound_datapoints() const
    {
      return compound_datapoints;
    }

    void CompoundDatapoint::tree_dump(std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_,
                                bool inherit_) const
    {
      this->BaseDatapoint::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Simple datapoints   : " << simple_datapoints.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Compound datapoints   : " << compound_datapoints.size() << std::endl;

      return;
    }

    SimpleDatapoint::~SimpleDatapoint()
    {
      return;
    }

    // virtual
    Type SimpleDatapoint::get_type() const
    {
      return type;
    }

    // virtual
    const boost::optional<Unit> & SimpleDatapoint::get_unit() const
    {
      return unit;
    }

    void SimpleDatapoint::tree_dump(std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_,
                                bool inherit_) const
    {
      this->BaseDatapoint::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Type  : " << type << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Id   : '";
      optional_print<Id>(id, out_);
      out_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Acces(s) level   : ";
      optional_print<AccesLevel>(acces_level, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Array size   : ";
      optional_print<ArraySize>(array_size, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Elements : " << elements.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Monitoring rate   : ";
      optional_print<MonitoringRate>(monitoring_rate, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Data Frame structure ref : ";
      optional_print_yesno<DataFrameStructureRef>(data_frame_structure_ref, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Default value   : ";
      optional_print_yesno<DefaultValue>(default_value, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Range   : ";
      optional_print_yesno<Range>(range, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Description   : ";
      optional_print<Description>(description, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Historizing   : ";
      optional_print_yesno<Historizing>(historizing, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Write mask   : ";
      optional_print_yesno<WriteMask>(write_mask, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "End delimiter   : ";
      optional_print_yesno<EndDelimiter>(end_delimiter, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Alarm   : ";
      optional_print_yesno<Alarm>(alarm, out_);
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Info   : ";
      optional_print_yesno<Info>(info, out_);
      out_ << std::endl;
      if (info) {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
        info.get().tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Alarm methods : " << alarm_methods.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Variables : " << variables.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Frame elements : " << frame_elements.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Frame element strings : " << frame_element_strings.size() << std::endl;

      return;
    }

    BaseDatapoint::~BaseDatapoint()
    {
      return;
    }

    const std::string & BaseDatapoint::get_name() const
    {
      return name;
    }

    const std::vector<UserInfo> & BaseDatapoint::get_userinfos() const
    {
      return userinfos;
    }

    const std::vector<Attribut> & BaseDatapoint::get_attributes() const
    {
      return attributes;
    }

    // virtual
    const boost::optional<Info> & BaseDatapoint::get_info() const
    {
      return info;
    }

    const boost::optional<Multiplicity> & BaseDatapoint::get_multiplicity() const
    {
      return multiplicity;
    }

    // virtual
    const std::vector<Method> & BaseDatapoint::get_methods() const
    {
      return methods;
    }

    void BaseDatapoint::tree_dump(std::ostream & out_,
                                  const std::string & title_,
                                  const std::string & indent_,
                                  bool inherit_) const
    {
      if (! title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name  : '" << name << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Multiplicity   : '";
      optional_print<Multiplicity>(multiplicity, out_);
      out_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Attributes : " << attributes.size() << std::endl;


      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Methods : " << methods.size() << std::endl;
      {
        int counter = 0;
        for (const auto & method : methods) {
          out_ << indent_ << datatools::i_tree_dumpable::skip_tag;
          if (++counter == methods.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "Method '" << method.name << "'";
          out_ << std::endl;
        }
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Info   : ";
      optional_print_yesno<Info>(info, out_);
      out_ << std::endl;
      if (info) {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
        info.get().tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "User infos : " << userinfos.size() << std::endl;

      return;
    }

    base_hidf::~base_hidf()
    {
      return;
    }

    Type base_hidf::get_type() const
    {
      return type;
    }

    const boost::optional<Unit> & base_hidf::get_unit() const
    {
      return unit;
    }

    Argument::~Argument()
    {
      return;
    }

    const std::string & Argument::get_name() const
    {
      return name;
    }

    Type Argument::get_type() const
    {
      return type;
    }

    const boost::optional<Unit> & Argument::get_unit() const
    {
      return unit;
    }

    const std::vector<UserInfo> & Argument::get_userinfos() const
    {
      return userinfos;
    }

    Method::~Method()
    {
      return;
    }

    // virtual
    const std::string & Method::get_name() const
    {
      return name;
    }

    const std::vector<UserInfo> & Method::get_userinfos() const
    {
      return userinfos;
    }

    // virtual
    const std::vector<Attribut> & Method::get_attributes() const
    {
      return attributes;
    }

    // virtual
    const boost::optional<Info> & Method::get_info() const
    {
      return info;
    }

    void Method::tree_dump(std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_,
                                bool inherit_) const
    {
      if (! title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name  : '" << name << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Arguments : " << arguments.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Device instructions : " << device_instructions.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Sequences : " << sequences.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "EndDelimiters : " << end_delimiters.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Events : " << events.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Info  : " ;
      optional_print_yesno<Info>(info, out_);
      out_ << std::endl;
      if (info) {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
        info.get().tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Attributes  : " ;
      out_ << '[' << get_attributes().size() << ']';
      out_ << std::endl;

      return;
    }

  } // namespace mos

} // namespace vire
