//! \file vire/cmsserver/uc_composition_description.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/uc_composition_description.h>

// Third party:
// - Boost:
#include <boost/algorithm/string/trim.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace cmsserver {

    // Serialization implementation
    DATATOOLS_SERIALIZATION_IMPLEMENTATION(uc_composition_description, "vire::cmsserver::uc_composition_description")


    uc_composition_description::uc_composition_description()
    {
      return;
    }

    uc_composition_description::~uc_composition_description()
    {
      return;
    }

    bool uc_composition_description::has_scheduling() const
    {
      return _scheduling_ != vire::running::SCHED_UNDEF;
    }
 
    void uc_composition_description::set_scheduling(const vire::running::scheduling_type sched_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Composition is locked!");
      _scheduling_ = sched_;
      return;
    }
   
    vire::running::scheduling_type uc_composition_description::get_scheduling() const
    {
      return _scheduling_;
    }
    
    bool uc_composition_description::is_scheduling_parallel() const
    {
      return _scheduling_ == vire::running::SCHED_PARALLEL;
    }
    
    bool uc_composition_description::is_scheduling_serial() const
    {
      return _scheduling_ == vire::running::SCHED_SERIAL;
    }
    
    bool uc_composition_description::is_scheduling_repeat() const
    {
      return _scheduling_ == vire::running::SCHED_REPEAT;
    }

    bool uc_composition_description::has_repeat_number_of_repetitions() const
    {
      return _number_of_repetitions_ > 0;
    }
    
    void uc_composition_description::set_repeat_number_of_repetitions(const std::size_t n_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Composition is locked!");
      if (has_scheduling()) {
        DT_THROW_IF(!is_scheduling_repeat(), std::logic_error, "Composition does not use the 'repeat' scheduling model!");
      } else {
        // Force the 'repeat' scheduling model: 
        set_scheduling(vire::running::SCHED_REPEAT);
      }
      _number_of_repetitions_ = n_;
      return;
    }
    
    std::size_t uc_composition_description::get_repeat_number_of_repetitions() const
    {
      return _number_of_repetitions_;
    }
   
    void uc_composition_description::export_to_config(datatools::properties & config_,
                                                      const uint32_t flags_,
                                                      const std::string & prefix_) const
    {
      if (flags_ & EXPORT_CONFIG_CLEAR) {
        config_.clear();
      }

      if (flags_ & EXPORT_CONFIG_LOCKED) {
        if (is_locked()) {
          config_.store_boolean(prefix_ + "locked", _locked_, "Lock flag");
        }
      }

      if (flags_ & EXPORT_CONFIG_SCHEDULING) {
        if (has_scheduling()) {
          config_.store_string(prefix_ + "scheduling", vire::running::scheduling_to_label(_scheduling_), "Lock flag");
        }
      }

      if (flags_ & EXPORT_CONFIG_REPETITIONS) {
        if (is_scheduling_repeat() && has_repeat_number_of_repetitions()) {
          config_.store_integer(prefix_ + "number_of_repetitions", _number_of_repetitions_, "Number of repetitions (repeat scheduling)");
        }
      }
  
      if (flags_ & EXPORT_CONFIG_DAUGHTERS) {
        if (has_daughters()) {
          std::vector<std::string> names;
          build_daughter_names(names);
          config_.store(prefix_ + "names", names, "List of daughter use cases");
          for (const std::string & name : names) {
            const daughter_entry & de = get_daughter(name);
            std::string model_id_key = prefix_ + name + ".model_id";
            std::string description_key = prefix_ + name + ".description";
            config_.store_string(model_id_key,    de.model_id,    "Use case model ID of daughter '" + name + "'");
            config_.store_string(description_key, de.description, "Description of daughter '" + name + "'");
          }
        }
      }

      return;
    }
  
    void uc_composition_description::configure(const datatools::properties & config_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Composition is locked!");

      if (!has_scheduling()) {
        if (config_.has_key("scheduling")) {
          std::string sched_label = config_.fetch_string("scheduling");
          vire::running::scheduling_type sched = vire::running::scheduling_from_label(sched_label);
          DT_THROW_IF(sched == vire::running::SCHED_UNDEF,
                      std::logic_error, "Invalid composition scheduling label '" << sched_label << "'!");
          this->set_scheduling(sched);
        }
      }

      if (is_scheduling_repeat() && !has_repeat_number_of_repetitions()) {
        if (config_.has_key("number_of_repetitions")) {
          std::size_t nr = config_.fetch_positive_integer("number_of_repetitions");
          this->set_repeat_number_of_repetitions(nr);
        }
      }

      _load_(config_);
      
      if (config_.has_key("locked")) {
        bool locked = config_.fetch_boolean("locked");
        if (locked) {
          this->lock();
        }
      }
      
      return;
    }

    bool uc_composition_description::can_merge(vire::running::scheduling_type scheduling_) const
    {
      if (is_locked()) return false;
      if (has_scheduling() && _scheduling_ != scheduling_) return false;
      return true;
    }
                        
    void uc_composition_description::merge(const uc_composition_description & other_,
                                           bool from_setup_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Composition is locked!");
      if (has_scheduling()) {
        DT_THROW_IF(other_.has_scheduling() && other_.get_scheduling() != this->get_scheduling(),
                    std::logic_error, "Non matching scheduling mode at composition merging!");
      } else {
        if (other_.has_scheduling()) {
          set_scheduling(other_.get_scheduling());
        }
      }
      std::vector<std::string> other_names;
      other_.build_daughter_names(other_names);
      for (std::size_t i = 0; i < other_names.size(); i++) {
        const daughter_entry & other_de = other_.get_daughter(other_names[i]);
        add_daughter(other_names[i], other_de.model_id, other_de.description, from_setup_);
      }
      return;
    }

    void uc_composition_description::_load_(const datatools::properties & config_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Composition is locked!");

      // Ordered list of daughter names:
      std::vector<std::string> daughter_names;
      if (config_.has_key("names")) {
        config_.fetch_unique_ordered("names", daughter_names);
      }

      for (const std::string & name : daughter_names) {
        std::string model_id;
        std::string description;
        {
          std::ostringstream model_id_key_ss;
          model_id_key_ss << name << ".model_id";
          if (config_.has_key(model_id_key_ss.str())) {
            model_id = config_.fetch_string(model_id_key_ss.str());
          }
        }
        {
          std::ostringstream description_key_ss;
          description_key_ss << name << ".description";
          if (config_.has_key(description_key_ss.str())) {
            description = config_.fetch_string(description_key_ss.str());
          }
        }
        add_daughter(name, model_id, description); 
      }
      
      return;
    }

    bool uc_composition_description::is_empty() const
    {
      return get_number_of_daughters() == 0;
    }

    void uc_composition_description::reset()
    {
      _locked_ = false;
      _daughters_.clear();
      return;
    }

    std::size_t uc_composition_description::get_number_of_daughters() const
    {
      return _daughters_.size();
    }
    
    bool uc_composition_description::has_daughters() const
    {
      return _daughters_.size() > 0;
    }

    void uc_composition_description::build_daughter_names(std::vector<std::string> & names_) const
    {
      names_.clear();
      if (is_scheduling_serial()) {
        build_serial_daughter_names(names_);
      } else {
        std::set<std::string> names;
        build_daughter_names(names);
        for (const auto & n : names) {
          names_.push_back(n);
        }
      }
      return;
    }

    void uc_composition_description::build_serial_daughter_names(std::vector<std::string> & names_) const
    {
      DT_THROW_IF(!is_scheduling_serial(), std::logic_error,
                  "Cannot order daughter use cases in '" << vire::running::scheduling_to_label(_scheduling_) << "' scheduling mode!");
      
      std::map<int, std::string> ordered;
      for (const auto & p : _daughters_) {
        ordered[p.second.serial_rank] = p.first;
      }
      names_.clear();
      for (const auto & p : ordered) {
        names_.push_back(p.second);
      }
      return;
    }

    void uc_composition_description::build_daughter_names(std::set<std::string> & names_) const
    {
      names_.clear();
      for (const auto & p : _daughters_) {
        names_.insert(p.first);
      }
      return;
    }
    
    bool uc_composition_description::has_daughter(const std::string & name_) const
    {
      return _daughters_.count(name_);
    }

    void uc_composition_description::add_daughter(const std::string & name_,
                                                  const std::string & model_id_,
                                                  const std::string & description_,
                                                  const bool from_setup_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Locked composition!");
      if (!has_scheduling()) {
        // Force parallel scheduling model:
        set_scheduling(DEFAULT_SCHEDULING);
      }
      DT_THROW_IF(is_scheduling_repeat() && _daughters_.size(),
                  std::logic_error, "Cannot add another daughter use case in 'repeat' scheduling mode!")   ;       
      std::string name = name_;
      if (name.empty()) {
        // Set a default name:
        name = scheduling_to_label(_scheduling_);
        if (is_scheduling_serial() || is_scheduling_parallel()) {
          name += "_" + std::to_string(_daughters_.size());
        }
      }
      static const uint32_t nvflags
        = datatools::NV_NO_COLON | datatools::NV_NO_HYPHEN | datatools::NV_LEADING_DIGIT;
      DT_THROW_IF(!datatools::name_validation(name, nvflags),
                  std::logic_error,
                  "Attempt to add the description of a daughter use case with an invalid name '" << name << "'!");
      DT_THROW_IF(has_daughter(name), std::logic_error,
                  "Description of a daughter use case named '" << name << "' already exists!");
      int rank = _daughters_.size();
      {
        daughter_entry e;
        _daughters_[name] = e;
      }
      daughter_entry & re = _daughters_.find(name)->second;
      re.model_id = model_id_;
      re.description = description_;
      if (is_scheduling_serial()) {
        re.serial_rank = rank;
      }
      re.from_setup = from_setup_;
      return;
    }

    const uc_composition_description::daughter_entry &
    uc_composition_description::get_daughter(const std::string & name_) const
    {
      daughters_coll_type::const_iterator found = _daughters_.find(name_);
      DT_THROW_IF(found == _daughters_.end(), std::logic_error,
                  "No daughter use case named '" << name_ << "'!");
      return found->second;
    }
                                       
    void uc_composition_description::remove_daughter(const std::string & name_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Locked composition!");
      daughters_coll_type::iterator found = _daughters_.find(name_);
      DT_THROW_IF(found == _daughters_.end(), std::logic_error,
                  "No daughter use case named '" << name_ << "'!");
      _daughters_.erase(found);
      return;
    }

    bool uc_composition_description::is_locked() const
    {
      return _locked_;
    }

    void uc_composition_description::lock()
    {
      _locked_ = true;
      return;
    }
    
    void uc_composition_description::_at_lock_()
    {
      // 
      return;
    }
   
    // virtual
    void uc_composition_description::print_tree(std::ostream & out_,
                                                const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }
     
      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Scheduling : '" << vire::running::scheduling_to_label(_scheduling_) << "'"
           << std::endl;

      if (is_scheduling_repeat()) {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Repeat # repetitions : " << _number_of_repetitions_
             << std::endl;
      }
      
      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Daughters : " << _daughters_.size()
           << std::endl;
      if (_daughters_.size()) {
        std::size_t counter = 0;
        for (const auto & p : _daughters_) {
          const daughter_entry & re = p.second;
          out_ << popts.indent << datatools::i_tree_dumpable::skip_tag;
          if (++counter == _daughters_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Daughter ";
          if (is_scheduling_serial()) {
            out_ << "[serial rank #" << re.serial_rank << "]";
          }
          out_ << " : '" << p.first << "' as '" << re.model_id << "'";
          if (!re.description.empty()) {
            out_ << " (" << re.description << ")";
          }
          out_ << std::endl;
        }
      }
      
      out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Locked : " << std::boolalpha << is_locked()
           << std::endl;
  
      return;
    }
    
  } // namespace cmsserver

} // namespace vire
