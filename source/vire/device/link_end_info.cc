
// Ourselves:
#include <vire/device/link_end_info.h>

// Standard library:
#include <string>

// Third party:
// - Boost:
#include <boost/spirit/include/qi.hpp>
// #include <boost/spirit/include/karma.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/base_device_model.h>
#include <vire/device/base_port_model.h>
#include <vire/device/physical_device.h>
#include <vire/device/logical_device.h>
#include <vire/device/physical_port.h>

namespace qi = boost::spirit::qi;

namespace parsing_impl {
  // Parsing struct:
  struct node_tokens
  {
    std::string name;
    std::string iter_rule;
  };
}

// Struct adapter:
BOOST_FUSION_ADAPT_STRUCT(parsing_impl::node_tokens,
                          (std::string, name)
                          (std::string, iter_rule)
                          )

namespace vire {

  namespace device {

    link_end_info::level_entry::level_entry()
    {
      return;
    }

    link_end_info::level_entry::~level_entry()
    {
      return;
    }

    bool link_end_info::level_entry::has_name() const
    {
      return !_name_.empty();
    }

    void link_end_info::level_entry::set_name(const std::string & name_)
    {
      _name_ = name_;
      return;
    }

    const std::string & link_end_info::level_entry::get_name() const
    {
      return _name_;
    }

    bool link_end_info::level_entry::has_instance() const
    {
      return _instance_.get() != nullptr;
    }

    void link_end_info::level_entry::set_instance(const i_instance * inst_ptr_)
    {
      _instance_.reset(inst_ptr_);
    }

    void link_end_info::level_entry::set_instance(const std::shared_ptr<i_instance> & inst_)
    {
      _instance_ = inst_;
      return;
    }

    const i_instance & link_end_info::level_entry::get_instance() const
    {
      return *_instance_;
    }

    void link_end_info::level_entry::reset()
    {
      _name_.clear();
      _instance_.reset();
      return;
    }

    link_end_info::link_end_info()
    {
      return;
    }

    link_end_info::~link_end_info()
    {
      _ports_path_.clear();
      return;
    }

    void link_end_info::reset()
    {
      _ports_path_.clear();
      return;
    }

    std::size_t link_end_info::get_depth() const
    {
      return _ports_path_.size();
    }

    const std::vector<link_end_info::level_entry> &
    link_end_info::get_ports_path() const
    {
      return _ports_path_;
    }

    std::size_t link_end_info::get_cardinality() const
    {
      if (_ports_path_.size() == 0) {
        return 0;
      }
      std::size_t sz = 1.0;
      for (auto lvl : _ports_path_) {
        if (lvl.has_instance()) {
          sz *= lvl.get_instance().get_number_of_items();
        }
      }
      return sz;
    }

    template <typename Iterator>
    struct node_parser : qi::grammar<Iterator,
                                     parsing_impl::node_tokens(),
                                     qi::ascii::space_type>
    {
      // Syntax: Name[IterRule]
      node_parser() : node_parser::base_type(start)
      {
        name_tok      %= +~qi::char_("[]/");
        spot_rule_tok %= '[' >> name_tok  >> ']';
        start         %= name_tok >> -spot_rule_tok;
      }
      // Parsing rules:
      qi::rule<Iterator, std::string(), qi::ascii::space_type>               name_tok;
      qi::rule<Iterator, std::string(), qi::ascii::space_type>               spot_rule_tok;
      qi::rule<Iterator, parsing_impl::node_tokens(), qi::ascii::space_type> start;
    };

    bool link_end_info::parse(const std::string & repr_,
                              const base_device_model & parent_device_model_)
    {
      datatools::logger::priority logging = datatools::logger::PRIO_FATAL;
      logging = datatools::logger::PRIO_DEBUG;
      namespace qi = boost::spirit::qi;
      _ports_path_.clear();
      std::string::const_iterator str_iter = repr_.begin();
      std::string::const_iterator end_iter = repr_.end();
      typedef std::string::const_iterator iterator_type;
      std::vector<parsing_impl::node_tokens> nodes;
      typedef node_parser<iterator_type> parser_type;
      parser_type g;
      bool res = qi::phrase_parse(str_iter,
                                  end_iter,
                                  g % '/',
                                  qi::ascii::space,
                                  nodes);
      if (!res || str_iter != end_iter) {
        DT_LOG_FATAL(logging, "Spirit/Qi parser failed to extract node tokens from '" << repr_ << "'!");
        return false;
      }
      if (nodes.size() == 0) {
        DT_LOG_FATAL(logging, "No port linking rule is provided!");
        return false;
      }
      // for (auto ntok : nodes) {
      //   DT_LOG_DEBUG(logging, "Name='" << ntok.name << "' " << "with rules '" << ntok.iter_rule << "'");
      // }
      const base_device_model * dev_model = &parent_device_model_;
      for (std::size_t ilevel = 0; ilevel < nodes.size(); ilevel++) {
        const std::string & name = nodes[ilevel].name;
        const std::string & iter_rule = nodes[ilevel].iter_rule;
        const i_instance * instance = nullptr;
        std::string embedded_label;
        DT_LOG_DEBUG(logging, "Level #" << ilevel << ": name='" << name << "' with iter_rule='" << iter_rule << "'");
        if (ilevel + 1 != nodes.size()) {
          DT_THROW_IF(!dev_model->has_embedded_device(name),
                      std::logic_error,
                      "No embedded device named '" << name << "' in device model '"
                      << dev_model->get_name() << "'!");
          const physical_device & phys_dev = dev_model->get_embedded_device(name);
          if (phys_dev.has_instance()) {
            instance = &phys_dev.get_instance();
            DT_LOG_DEBUG(logging, "Device instance = [@" << instance << "]");
          }
          const logical_device & log_dev = phys_dev.get_logical();
          dev_model = &log_dev.get_model();
          embedded_label = "device";
        } else {
          DT_THROW_IF(!dev_model->has_port(name),
                      std::logic_error,
                      "No port named '" << name << "' in device model '"
                      << dev_model->get_name() << "'!");
          DT_LOG_DEBUG(logging, "Found port='" << name << "'");
          const physical_port & phys_port = dev_model->get_port(name);
          phys_port.tree_dump(std::cerr, "Physical port: ", "[debug] ");
          if (phys_port.has_instance()) {
            instance = &phys_port.get_instance();
            DT_LOG_DEBUG(logging, "Port instance = [@" << instance << "]"
                         << " of type '" <<  typeid(*instance).name() << "'");
          } else {
            DT_LOG_DEBUG(logging, "No port instance...");
          }
          embedded_label = "port";
        }
        // DT_LOG_DEBUG(logging, "TEST A...");
        std::shared_ptr<i_instance> le_inst;
        if (instance != nullptr) {
          // DT_LOG_DEBUG(logging, "TEST B...");
          // DT_LOG_DEBUG(logging, "TEST B: singleton = " << instance->is_singleton());
          if (datatools::logger::is_debug(logging)) {
            // DT_LOG_DEBUG(logging, "TEST C: instance = [@" << std::hex << instance << std::dec << "]");
            // DT_LOG_DEBUG(logging, "TEST C: of type  = '" << typeid(*instance).name() << "'");
            instance->tree_dump(std::cerr, "Instance:", "[debug] ");
          }
          if (!iter_rule.empty()) {
            // DT_LOG_DEBUG(logging, "TEST C1...");
            if (instance->is_singleton()) {
              DT_LOG_FATAL(logging, "Linking rule '" << iter_rule
                           << "' is not compatible with singleton instance for " << embedded_label << " '" << name << "'!");
              return false;
            }
            std::set<std::string> inst_class_ids;
            inst_class_ids.insert("vire::device::single_instance");
            if (space::at_least_dimension_1(instance->get_dimension())) {
              inst_class_ids.insert("vire::device::multiple_instance");
              inst_class_ids.insert("vire::device::regular_1d_instance");
              if (space::at_least_dimension_2(instance->get_dimension())) {
                inst_class_ids.insert("vire::device::regular_2d_instance");
              }
            }
            std::ostringstream linking_rule_oss;
            linking_rule_oss << '[' << iter_rule << ']';
            std::string linking_rule = linking_rule_oss.str();
            for (const auto instance_class_id : inst_class_ids) {
              DT_LOG_DEBUG(logging, "Attempt to create a '" << instance_class_id << "' object for "
                           << embedded_label << " '" << name << "'...");
              le_inst.reset(i_instance::create(instance_class_id, linking_rule));
              if (le_inst.get() != nullptr) {
                break;
              }
            }
            if (le_inst.get() == nullptr) {
              DT_LOG_FATAL(logging, "Cannot parse linking rule '"
                           << linking_rule << "' for " << embedded_label << " '" << name << "'!");
              return false;
            }
            if (datatools::logger::is_debug(logging)) {
              le_inst->tree_dump(std::cerr, "Linked instance: ", "[debug] ");
            }
            if (!le_inst->belongs_to(*instance)) {
              DT_LOG_FATAL(logging, "Linked instance from rule '" << linking_rule
                           << "' is not included in " << embedded_label << " '" << name << "' instance!");
              return false;
            }
          } else {
            // DT_LOG_DEBUG(logging, "TEST C2...");
            if (instance->is_multiple()) {
              DT_LOG_FATAL(logging, "Linking rule is needed with multiple instance for " << embedded_label << " '" << name << "'!");
              return false;
            }
          }
        }
        // DT_LOG_DEBUG(logging, "TEST D : Populating level #" << ilevel << " ...");
        {
          level_entry newe;
          newe.set_name(name);
          _ports_path_.push_back(newe);
        }
        level_entry & le = _ports_path_.back();
        if (le_inst) {
          // DT_LOG_DEBUG(logging, "TEST E : Setting link port end instance...");
          le.set_instance(le_inst);
        }
      }
      DT_LOG_DEBUG(logging, "Successful parsing!");
      return true;
    }

    bool link_end_info::is_valid() const
    {
      if (get_cardinality() == 0) return false;
      return true;
    }

    void link_end_info::tree_dump(std::ostream & out_,
                                  const std::string & title_,
                                  const std::string & indent_,
                                  bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Port path: ";
      if (!_ports_path_.size()) {
        out_ << "<none>";
      } else {
        out_ << "(depth=" << _ports_path_.size() << ")";
      }
      out_ << std::endl;
      for (std::size_t ilvl = 0; ilvl < _ports_path_.size(); ilvl++) {
        out_ << indent_ << i_tree_dumpable::skip_tag;
        if (ilvl + 1 == _ports_path_.size()) {
          out_ << i_tree_dumpable::last_tag;
        } else {
          out_ << i_tree_dumpable::tag;
        }
        const level_entry & le = _ports_path_[ilvl];
        out_ << "Level #" << ilvl << " : Name='" << le.get_name() << "'";
        if (le.has_instance()) {
          out_ << " with cardinality [" << le.get_instance().get_number_of_items() << "]";
        }
        out_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Cardinality : " << get_cardinality() << std::endl;

      return;
    }

  } // end of namespace device

} // end of namespace vire
