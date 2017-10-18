//! \file vire/cmsserver/resource_pool.cc
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

// Ourselves:
#include <vire/cmsserver/resource_pool.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/resource/manager.h>
#include <vire/resource/resource.h>
#include <vire/cmsserver/utils.h>

namespace vire {

  namespace cmsserver {

    resource_pool::resource_pool()
    {
      _initialized_ = false;
      return;
    }

    resource_pool::~resource_pool()
    {
      return;
    }

    // static
    void resource_pool::load_daughter_from_parent(resource_pool & parent_,
                                                  resource_pool & daughter_,
                                                  const vire::resource::manager & resource_mgr_,
                                                  const cardinalities_request_type & token_cardinalities_)
    {
      DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());
      DT_THROW_IF(!parent_.is_initialized(), std::logic_error,
                  "Source parent resource pool is not initialized!");
      DT_THROW_IF(daughter_.is_initialized(), std::logic_error,
                  "Target daughter resource pool is already initialized!");
      daughter_._limited_tokens_.clear();
      daughter_._unlimited_.clear();
      for (std::map<int32_t, std::size_t>::const_iterator i = token_cardinalities_.begin();
           i != token_cardinalities_.end();
           i++) {
        // The ID of the resource with limited cardinality:
        int32_t rid = i->first;
        const vire::resource::resource & r = resource_mgr_.get_resource_by_id(rid);
        // The requested cardinality associated to the resource:
        std::size_t cardinality = i->second;
        DT_THROW_IF(!parent_.has_resource(rid), std::logic_error,
                    "Source parent resource pool has no resource with ID=[" << rid << "]!");
        if (cardinality == 0) {
          // No need for this resource in the daughter pool, we pass:
          continue;
        }
        if (r.is_number_of_tokens_unlimited()) {
          // We ignore cardinality directive for resource with unlimited number of tokens:
          daughter_._unlimited_.insert(rid);
          // // 2017-10-03, FM: or should we throw ?
          // DT_THROW(std::logic_error,
          //       "Resource with ID=[" << rid << " is unlimited and cannot be constrained via cardinality]!");
        } else {
          DT_THROW_IF(cardinality > parent_.remaining_limited_tokens(rid),
                      std::logic_error,
                      "Source parent resource pool has not enough tokens left for resource with ID=[" << rid << "]!");
          // We transfer tokens from parent to daughter:
          parent_.decrement_limited_tokens(rid, cardinality);
          daughter_._limited_tokens_[rid] = cardinality;
        }
      }
      daughter_.initialize();
      DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
      return;
    }

    // static
    void resource_pool::restore_parent_from_daughter(resource_pool & parent_, resource_pool & daughter_)
    {
      DT_THROW_IF(!parent_.is_initialized(), std::logic_error,
                  "Parent resource pool is not initialized!");
      DT_THROW_IF(!daughter_.is_initialized(), std::logic_error,
                  "Daughter resource pool is not initialized!");
      for (std::map<int32_t, std::size_t>::const_iterator i = daughter_._limited_tokens_.begin();
           i != daughter_._limited_tokens_.end();
           i++) {
        int32_t rid = i->first;
        std::size_t cardinality = i->second;
        parent_.increment_limited_tokens(rid, cardinality);
      }
      daughter_.reset();
      return;
    }

    // static
    void resource_pool::init_root(resource_pool & root_,
                                  const vire::resource::manager & resource_mgr_,
                                  const cardinalities_request_type & requested_cardinalities_,
                                  cardinality_profile_type profile_)
    {
      DT_THROW_IF(root_.is_initialized(), std::logic_error,
                  "Target root resource pool is already initialized!");
      root_._limited_tokens_.clear();
      root_._unlimited_.clear();
      // Scan all available resources from the manager:
      const std::set<int32_t> & ids = resource_mgr_.get_set_of_resource_ids();
      for (std::set<int32_t>::const_iterator i = ids.begin(); i != ids.end(); i++) {
        int32_t rid = *i;
        const vire::resource::resource & r = resource_mgr_.get_resource_by_id(rid);
        // Default cardinality is set to some unusable reference value:
        std::size_t cardinality = vire::resource::resource::MAX_TOKENS_UNLIMITED;
        if (requested_cardinalities_.count(rid)) {
          // Read specific cardinality from the request:
          cardinality = requested_cardinalities_.at(rid);
        }
        if (cardinality == 0) {
          // Remove the (unlimited or limited) resource from the pool
          // if explicit ZERO value is set for the resource.
          continue;
        }
        if (r.is_number_of_tokens_unlimited()) {
          // Unlimited resource is push as is in the pool:
          root_._unlimited_.insert(rid);
        } else {
          std::size_t max_cardinality = r.get_max_number_of_tokens();
          if (cardinality == vire::resource::resource::MAX_TOKENS_UNLIMITED) {
            // If requested cardinalities does not contains the resource id,
            // we compute a default one depending of the profile:
            if (profile_ == CARD_ONLY_ONE) {
              cardinality = 1;
            } else if (profile_ == CARD_ALL) {
              cardinality = max_cardinality;
            } else if (profile_ == CARD_ALL_PLUS_ONE) {
              cardinality = max_cardinality + 1;
            }
          } else {
            // Requested cardinality overflows the maximum allowed for this resource:
            DT_THROW_IF(cardinality > max_cardinality, std::logic_error,
                        "Requested token cardinality [" << cardinality << "] is too large!");
          }
          root_._limited_tokens_[rid] = cardinality;
        }
      } // end of scan.
      root_.initialize();
      return;
    }

    bool resource_pool::is_initialized() const
    {
      return _initialized_;
    }

    void resource_pool::initialize()
    {
      _initialized_ = true;
      return;
    }

    void resource_pool::reset()
    {
      _initialized_ = false;
      _limited_tokens_.clear();
      _unlimited_.clear();
      return;
    }

    bool resource_pool::has_resource(int32_t rid_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      if (_unlimited_.count(rid_) > 0) {
        return true;
      }
      if (_limited_tokens_.find(rid_) != _limited_tokens_.end()) {
        return true;
      }
      return false;
    }

    bool resource_pool::is_limited(int32_t rid_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      return _limited_tokens_.find(rid_) != _limited_tokens_.end();
    }

    bool resource_pool::is_exhausted(int32_t rid_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      return is_limited(rid_) && remaining_limited_tokens(rid_) == 0;
    }

    bool resource_pool::is_unlimited(int32_t rid_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      return _unlimited_.count(rid_) > 0;
    }

    void resource_pool::add_unlimited(int32_t rid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      _unlimited_.insert(rid_);
      return;
    }

    void resource_pool::increment_limited_tokens(int32_t rid_, std::size_t step_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      std::map<int32_t, std::size_t>::iterator found = _limited_tokens_.find(rid_);
      DT_THROW_IF(found == _limited_tokens_.end(),
                  std::logic_error,
                  "Resource with identifier [" << rid_ << "] is not limited!");
      found->second += step_;
      return;
    }

    void resource_pool::decrement_limited_tokens(int32_t rid_, std::size_t step_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      std::map<int32_t, std::size_t>::iterator found = _limited_tokens_.find(rid_);
      DT_THROW_IF(found == _limited_tokens_.end(),
                  std::logic_error,
                  "Resource with identifier [" << rid_ << "] is not limited!");
      DT_THROW_IF(step_ > found->second, std::range_error,
                  "Resource with identifier [" << rid_ << "] has not enough remaining tokens!");
      found->second -= step_;
      return;
    }

    std::size_t resource_pool::remaining_limited_tokens(int32_t rid_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      std::map<int32_t, std::size_t>::const_iterator found
        = _limited_tokens_.find(rid_);
      DT_THROW_IF(found == _limited_tokens_.end(),
                  std::logic_error,
                  "Resource with identifier [" << rid_ << "] is not limited!");
      return found->second;
    }

    bool resource_pool::can_tranfer(int32_t rid_, std::size_t step_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      return step_ <= remaining_limited_tokens(rid_);
    }

    void resource_pool::build_all(std::set<int32_t> & all_, bool append_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      build_limited(all_, append_);
      build_unlimited(all_, true);
      return;
    }

    void resource_pool::build_limited(std::set<int32_t> & limited_, bool append_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      if (!append_) limited_.clear();
      for (std::map<int32_t, std::size_t>::const_iterator i = _limited_tokens_.begin();
           i != _limited_tokens_.end();
           i++) {
        limited_.insert(i->first);
      }
      return;
    }

    void resource_pool::build_unlimited(std::set<int32_t> & unlimited_, bool append_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Resource pool is not initialized!");
      if (!append_) unlimited_.clear();
      for (std::set<int32_t>::const_iterator i = _unlimited_.begin();
           i != _unlimited_.end();
           i++) {
        unlimited_.insert(*i);
      }
      return;
    }

    const std::map<int32_t, std::size_t> & resource_pool::get_limited_tokens() const
    {
      return _limited_tokens_;
    }

    const std::set<int32_t> & resource_pool::get_unlimited() const
    {
      return _unlimited_;
    }

    // bool resource_pool::contains(const resource_pool & rp_) const
    // {
    //   std::set<int32_t> all2;
    //   rp_.build_all(all2);
    //   for (std::set<int32_t>::const_iterator i = all2.begin();
    //        i != all2.end();
    //        i++) {
    //     if (! has_resource(*i)) {
    //       return false;
    //     }
    //   }
    //   return true;
    // }

    /*
    // friend
    resource_pool operator+(const resource_pool & rp1_, const resource_pool & rp2_)
    {
      resource_pool rp;
      for (std::set<int32_t>::const_iterator i1 = rp1_._unlimited_.begin();
           i1 != rp1_._unlimited_.end();
           i1++) {
        rp._unlimited_.insert(*i1);
      }
      for (std::set<int32_t>::const_iterator i2 = rp2_._unlimited_.begin();
           i2 != rp2_._unlimited_.end();
           i2++) {
        rp._unlimited_.insert(*i2);
      }
      for (std::map<int32_t, std::size_t>::const_iterator i1
             = rp1_._limited_tokens_.begin();
           i1 != rp1_._limited_tokens_.end();
           i1++) {
        rp._limited_tokens_[i1->first] = i1->second;
      }
      for (std::map<int32_t, std::size_t>::const_iterator i2
             = rp2_._limited_tokens_.begin();
           i2 != rp2_._limited_tokens_.end();
           i2++) {
        std::map<int32_t, std::size_t>::iterator found = rp._limited_tokens_.find(i2->first);
        if (found != rp._limited_tokens_.end()) {
          found->second += i2->second;
        } else {
          rp._limited_tokens_[i2->first] = i2->second;
        }
      }
      return rp;
    }

    // friend
    resource_pool operator-(const resource_pool & rp1_, const resource_pool & rp2_)
    {
      resource_pool rp;
      for (std::set<int32_t>::const_iterator i1 = rp1_._unlimited_.begin();
           i1 != rp1_._unlimited_.end();
           i1++) {
        rp._unlimited_.insert(*i1);
      }
      // XXX
      return rp;
    }
    */

    // // static
    // void and(const resource_pool & rp1_, const resource_pool & rp2_,
    //          resource_pool & rp3_)
    // {
    //   return;
    // }

    void resource_pool::print(std::ostream & out_, uint32_t flags_) const
    {
      if (flags_ | 0x1) {
        out_ << "#@initialized=" << is_initialized() << std::endl;
      }
      if (is_initialized()) {
        if (flags_ | 0x1) {
          out_ << "#@number_of_unlimited_resources=" << _unlimited_.size() << std::endl;
          out_ << "#@number_of_limited_resources=" << _limited_tokens_.size() << std::endl;
        }
        for (std::set<int32_t>::const_iterator i = _unlimited_.begin();
             i != _unlimited_.end();
             i++) {
          out_ << *i << ' ' << vire::resource::resource::MAX_TOKENS_UNLIMITED << std::endl;
        }
        for (std::map<int32_t, std::size_t>::const_iterator i = _limited_tokens_.begin();
             i != _limited_tokens_.end();
             i++) {
          out_ << i->first << ' ' << i->second << std::endl;
        }
      }
      out_ << std::endl;
      return;
    }

    void resource_pool::tree_dump(std::ostream & out_,
                                  const std::string & title_,
                                  const std::string & indent_,
                                  bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      if (is_initialized()) {
        out_ << indent_ << i_tree_dumpable::tag
             << "Unlimited resource IDs: " << _unlimited_.size() << std::endl;

        out_ << indent_ << i_tree_dumpable::tag
             << "Limited resource IDs: " << _limited_tokens_.size() << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Initialized: " << _initialized_ << std::endl;

      return;
    }

    bool resource_pool::operator!=(const resource_pool & pool_) const
    {
      return !operator==(pool_);
    }

    bool resource_pool::operator==(const resource_pool & pool_) const
    {
      if (_initialized_ != pool_._initialized_) {
        return false;
      }
      if (_limited_tokens_.size() != _limited_tokens_.size()) {
        return false;
      }
      if (_unlimited_.size() != _unlimited_.size()) {
        return false;
      }
      for (std::set<int32_t>::const_iterator i = _unlimited_.begin();
           i != _unlimited_.end();
           i++) {
        if (pool_._unlimited_.count(*i) == 0) {
          return false;
        }
      }
      for (std::map<int32_t, std::size_t>::const_iterator i = _limited_tokens_.begin();
           i != _limited_tokens_.end();
           i++) {
        int32_t rid = i->first;
        if (pool_._limited_tokens_.count(rid) == 0) {
          return false;
        }
        if (pool_._limited_tokens_.at(rid) != i->second) {
          return false;
        }
      }
      return true;
    }

  } // namespace cmsserver

} // namespace vire
