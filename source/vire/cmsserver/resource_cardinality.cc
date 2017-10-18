//! \file vire/cmsserver/resource_cardinality.cc
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
#include <vire/cmsserver/resource_cardinality.h>

// Third party:
// - Boost:
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>
#include <vire/cmsserver/utils.h>

namespace vire {

  namespace cmsserver {

    cardinalities_request_type::cardinalities_request_type()
    {
      return;
    }

    cardinalities_request_type::~cardinalities_request_type()
    {
      return;
    }

    bool cardinalities_request_type::has_resource(int32_t resource_id_) const
    {
      return this->count(resource_id_) != 0;
    }

    void cardinalities_request_type::set_unlimited_resource(int32_t resource_id_)
    {
      (*this)[resource_id_] = vire::resource::resource::MAX_TOKENS_UNLIMITED;
      return;
    }

    bool cardinalities_request_type::has_unlimited_resource(int32_t resource_id_) const
    {
      if (this->has_resource(resource_id_)
          && this->at(resource_id_) == vire::resource::resource::MAX_TOKENS_UNLIMITED) {
        return true;
      }
      return false;
    }

    void cardinalities_request_type::unset_resource(int32_t resource_id_)
    {
      (*this)[resource_id_] = 0;
      return;
    }

    bool cardinalities_request_type::has_unset_resource(int32_t resource_id_) const
    {
      if (this->has_resource(resource_id_) && this->at(resource_id_) == 0) return true;
      return false;
    }

    void cardinalities_request_type::set_limited_resource(int32_t resource_id_,
                                                          std::size_t cardinality_)
    {
      (*this)[resource_id_] = cardinality_;
      return;
    }

    bool cardinalities_request_type::has_limited_resource(int32_t resource_id_) const
    {
      if (this->has_resource(resource_id_)) {
        std::size_t cardinality = this->at(resource_id_);
        if (cardinality != 0
            && cardinality != vire::resource::resource::MAX_TOKENS_UNLIMITED) {
          return true;
        }
      }
      return false;
    }

    void cardinalities_request_type::print(std::ostream & out_) const
    {
      out_ << "#@number_of_cardinalities=" << this->size() << std::endl;
      for (const_iterator i = this->begin(); i != this->end(); i++) {
        out_ << i->first << " " << i->second << std::endl;
      }
      out_ << std::endl;
      return;
    }

    void cardinalities_request_type::initialize(const datatools::properties & config_,
                                                const vire::resource::manager & resource_mgr_)
    {
      initialize(config_, &resource_mgr_);
      return;
    }

    void cardinalities_request_type::initialize(const datatools::properties & config_,
                                                const vire::resource::manager * resource_mgr_)
    {
      // Reset the map:
      clear();

      {
        // Unset resources:
        std::set<std::string> unset;
        if (config_.has_key("unset")) {
          config_.fetch("unset", unset, false);
        }
        for (const auto & resource_str : unset) {
          int32_t resource_id = vire::resource::resource::INVALID_ID;
          if (resource_mgr_ != nullptr) {
            DT_THROW_IF(! resource_mgr_->fetch_resource_id(resource_str, resource_id),
                        std::logic_error,
                        "Cannot resolve unset resource identifier from '" << resource_str << "'!");
          } else {
            try {
              // Attempt to parse the resource integer identifier:
              resource_id = boost::lexical_cast<int32_t>(resource_str);
            } catch (boost::bad_lexical_cast &) {
              DT_THROW(std::logic_error, "Invalid resource identifier format!");
            }
          }
          DT_THROW_IF(resource_id < vire::resource::resource::MIN_ID, std::logic_error,
                      "Invalid resource identifier!");
          unset_resource(resource_id);
        }
      }

      {
        // Unlimited resources:
        std::set<std::string> unlimited;
        if (config_.has_key("unlimited")) {
          config_.fetch("unlimited", unlimited, false);
        }
        for (const auto & resource_str : unlimited) {
          int32_t resource_id = vire::resource::resource::INVALID_ID;
          if (resource_mgr_ != nullptr) {
            DT_THROW_IF(! resource_mgr_->fetch_resource_id(resource_str, resource_id),
                        std::logic_error,
                        "Cannot resolve unlimited resource identifier from '" << resource_str << "'!");
            DT_THROW_IF(!resource_mgr_->get_resource_by_id(resource_id).is_number_of_tokens_unlimited(),
                        std::logic_error,
                        "Resource '" << resource_str << "' is not unlimited!");
          } else {
            try {
              // Attempt to parse the resource integer identifier:
              resource_id = boost::lexical_cast<int32_t>(resource_str);
            } catch (boost::bad_lexical_cast &) {
              DT_THROW(std::logic_error, "Invalid resource identifier format!");
            }
          }
          DT_THROW_IF(resource_id < vire::resource::resource::MIN_ID, std::logic_error,
                      "Invalid resource identifier!");
          DT_THROW_IF(this->count(resource_id) == 1, std::logic_error,
                      "Resource '" << resource_str << "' is already registered !");
          set_unlimited_resource(resource_id);
        }
      }

      {
        // Limited resources:
        std::set<std::string> limited;
        if (config_.has_key("limited")) {
          config_.fetch("limited", limited, false);
        }
        for (const auto & resource_card_str : limited) {
          std::vector<std::string> tokens;
          boost::split(tokens, resource_card_str,boost::is_any_of("="));
          std::size_t cardinality = 1;
          DT_THROW_IF(tokens.size() > 2 || tokens.size() < 1, std::logic_error,
                      "Invalid limited resource cardinality format '" << resource_card_str << "'!");
          if (tokens.size() == 2) {
            try {
              cardinality = boost::lexical_cast<std::size_t>(tokens[1]);
            } catch (boost::bad_lexical_cast &) {
              DT_THROW(std::logic_error, "Invalid limited resource cardinality format '" << resource_card_str << "'!");
            }
          }
          int32_t resource_id = vire::resource::resource::INVALID_ID;
          if (resource_mgr_ != nullptr) {
            DT_THROW_IF(! resource_mgr_->fetch_resource_id(tokens[0], resource_id),
                        std::logic_error,
                        "Cannot resolve limited resource identifier from '" << tokens[0] << "'!");
            DT_THROW_IF(!resource_mgr_->get_resource_by_id(resource_id).is_number_of_tokens_limited(),
                        std::logic_error,
                        "Resource '" << tokens[0] << "' is not limited!");
            std::size_t max_cardinality
              = resource_mgr_->get_resource_by_id(resource_id).get_max_number_of_tokens();
            DT_THROW_IF(cardinality > max_cardinality,
                        std::logic_error,
                        "Resource '" << tokens[0] << "' cannot provide more than '" << max_cardinality << "' tokens!");
          } else {
            try {
              // Attempt to parse the resource integer identifier:
              resource_id = boost::lexical_cast<int32_t>(tokens[0]);
            } catch (boost::bad_lexical_cast &) {
              DT_THROW(std::logic_error, "Invalid resource identifier format!");
            }
          }
          DT_THROW_IF(resource_id < vire::resource::resource::MIN_ID, std::logic_error,
                      "Invalid resource identifier!");
          DT_THROW_IF(this->count(resource_id) == 1, std::logic_error,
                      "Resource '" << tokens[0] << "' is already registered !");
          set_limited_resource(resource_id, cardinality);
        }
      }

      return;
    }

    void cardinalities_request_type::reset()
    {
      this->clear();
      return;
    }

    void cardinalities_request_type::build_from_role(const vire::resource::manager & resource_mgr_,
                                                     const vire::resource::role & r_,
                                                     vire::resource::role::resource_set_flag flag_,
                                                     cardinalities_request_type special_)
    {
      DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());
      // Reset the map:
      this->clear();

      const std::set<int32_t> * res_pool_ptr = nullptr;
      if (flag_ & ::vire::resource::role::RESOURCE_SET_FUNCTIONAL) {
        if (r_.has_functional_resource_selector()) {
          res_pool_ptr = &r_.get_set_of_functional_resource_ids();
        }
      }
      if (flag_ & ::vire::resource::role::RESOURCE_SET_DISTRIBUTABLE) {
        if (r_.has_distributable_resource_selector()) {
          res_pool_ptr = &r_.get_set_of_distributable_resource_ids();
        }
      }

      if (res_pool_ptr == nullptr) {
        DT_LOG_TRACE(vire::cmsserver::logging(), "No resources available from the role [" << r_.get_id() << "]");
        DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
        return;
      }

      // Scan all resources from the role and build the request from their
      // nature (limited/unlimited). Apply special request if any.
      for (std::set<int32_t>::const_iterator ires = res_pool_ptr->begin();
           ires != res_pool_ptr->end();
           ires++) {
        int32_t resource_id = *ires;
        if (special_.has_unset_resource(resource_id)) {
          // Explicitely skip this resource from the requested role:
          continue;
        }
        // Check unlimited status:
        bool unlimited = resource_mgr_.get_resource_by_id(resource_id).is_number_of_tokens_unlimited();
        if (unlimited) {
          DT_THROW_IF(special_.has_limited_resource(resource_id),
                      std::logic_error,
                      "Resource with ID=[" << resource_id << "] is not limited!");
          this->set_unlimited_resource(resource_id);
        } else {
          std::size_t cardinality = 1;
          DT_THROW_IF(special_.has_unlimited_resource(resource_id),
                      std::logic_error,
                      "Resource with ID=[" << resource_id << "] is limited!");
          if (special_.has_limited_resource(resource_id)) {
            cardinality = special_[resource_id];
          }
          this->set_limited_resource(resource_id, cardinality);
        }
      }

      if (vire::cmsserver::logging() >= datatools::logger::PRIO_TRACE) {
        DT_LOG_TRACE(vire::cmsserver::logging(), "Build request: ");
        print(std::cerr);
        DT_LOG_TRACE(vire::cmsserver::logging(), "End.");
      }

      DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
      return;
    }

  } // namespace cmsserver

} // namespace vire
