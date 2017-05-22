// vire/user/base_resource_selector.cc - Implementation of a base selector of resources
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

// Ourselves
#include <vire/resource/base_resource_selector.h>

// This project:
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace resource {

    void base_resource_selector::_base_init(const datatools::properties & config_,
                                            datatools::service_manager * /* srvc_mgr_ */,
                                            cuts::cut_handle_dict_type * /* cuts_ */)
    {
      this->cuts::i_cut::_common_initialize(config_);

      return;
    }

    void base_resource_selector::_base_reset()
    {
      // _cached_rids_.reset();
      reset_resource_mgr();
      _set_defaults();
      this->cuts::i_cut::_reset();
      return;
    }

    void base_resource_selector::_set_defaults()
    {
      return;
    }

    base_resource_selector::base_resource_selector(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _resource_mgr_ = nullptr;
      _set_defaults();
      return;
    }

    base_resource_selector::~base_resource_selector()
    {
      DT_LOG_TRACE(_logging, "Destruction.");
      _base_reset();
      return;
    }

    bool base_resource_selector::has_resource_mgr() const
    {
      return _resource_mgr_ != 0;
    }

    void base_resource_selector::set_resource_mgr(const manager & umgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource selector '" << get_name() << "' is initialized and locked!");
      // DT_THROW_IF(!umgr_.is_initialized(), std::logic_error,
      //             "Resource manager for selector '" << get_name() << "' is not initialized!");
      _resource_mgr_ = &umgr_;
      return;
    }

    void base_resource_selector::reset_resource_mgr()
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource selector '" << get_name() << "' is initialized and locked!");
      _resource_mgr_ = 0;
      return;
    }

    const manager & base_resource_selector::get_resource_mgr() const
    {
      DT_THROW_IF(!has_resource_mgr(), std::logic_error,
                  "Resource selector '" << get_name() << "' has no resource manager!");
      return *_resource_mgr_;
    }

    // bool base_resource_selector::has_cached_rids() const
    // {
    //   return _cached_rids_.get() != 0;
    // }

    // void base_resource_selector::build_cached_rids()
    // {
    //   if (_cached_rids_.get() == 0) {
    //     _cached_rids_.reset(new std::set<int32_t>);
    //     const std::set<int32_t> & universe = _resource_mgr_->get_set_of_rids();
    //     bool ac_restore = is_activated_counters();
    //     this->set_activated_counters(false);
    //     for (auto rid : universe) {
    //       const resource & r = _resource_mgr_->get_resource_by_rid(rid);
    //       this->set_user_data(r);
    //       int res = this->process();
    //       if (res == cuts::SELECTION_ACCEPTED) {
    //         _cached_rids_->insert(r.get_rid());
    //       }
    //     }
    //     this->set_activated_counters(ac_restore);
    //   }
    //   return;
    // }

    // const std::set<int32_t> & base_resource_selector::get_set_of_rids() const
    // {
    //   DT_THROW_IF(!is_initialized(), std::logic_error,
    //               "Resource selector '" << get_name() << "' is not initialized!");
    //   bool build_it = false;
    //   if (!has_cached_rids()) {
    //     build_it = true;
    //   }
    //   if (build_it) {
    //     base_resource_selector * mutable_this = const_cast<base_resource_selector*>(this);
    //     mutable_this->build_cached_rids();
    //   }
    //   return *_cached_rids_;
    // }

    // virtual
    void base_resource_selector::tree_dump(std::ostream & out_,
                                           const std::string & title_,
                                           const std::string & indent_,
                                           bool inherit_) const
    {
      this->cuts::i_cut::tree_dump(out_, title_, indent_, true);

      // out_ << indent_ << i_tree_dumpable::tag
      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Resource manager : ";
      if (has_resource_mgr()) {
        out_ << _resource_mgr_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      /*
      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Cached RIDs : ";
      if (_cached_rids_.get() != 0) {
        if (_cached_rids_->size()) {
          out_ << _cached_rids_->size() << " RIDs";
        } else {
          out_ << "<no RIDs>";
        }
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      */

      return;
    }

  } // namespace resource

} // namespace vire
