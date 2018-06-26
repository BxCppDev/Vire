// vire/resource/resource.cc - Implementation of Vire resource
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
#include <vire/resource/resource.h>

// Standard Library:
#include <iostream>
#include <fstream>
#include <cstdlib>

// Third Party:
// - Boost:
#include <boost/filesystem.hpp>
// - datatools:
#include <datatools/exception.h>
#include <datatools/logger.h>
#include <datatools/utils.h>

// This Project:
#include <vire/version.h>
#include <vire/utility/path.h>

namespace vire {

  namespace resource {

    DATATOOLS_SERIALIZATION_IMPLEMENTATION_ADVANCED(resource, "vire::resource::resource")

    // static
    const int32_t  resource::INVALID_ID;
    const int32_t  resource::MIN_ID;
    const uint32_t resource::MAX_TOKENS_INVALID;
    const uint32_t resource::MAX_TOKENS_SINGLETON;
    const uint32_t resource::MAX_TOKENS_UNLIMITED;

    // virtual
    bool resource::is_name_valid(const std::string & name_) const
    {
      return ::datatools::name_validation(name_, datatools::NV_INSTANCE);
    }

    void resource::_copy_from_(const resource & resource_)
    {
      this->datatools::enriched_base::operator=(resource_);
      _initialized_ = resource_._initialized_;
      _id_ = resource_._id_;
      _path_ = resource_._path_;
      _access_ = resource_._access_;
      _max_number_of_tokens_ = resource_._max_number_of_tokens_;
      _allowed_subscription_ = resource_._allowed_subscription_;
      _allowed_non_blocking_ = resource_._allowed_non_blocking_;
      _resource_instance_ = resource_._resource_instance_;
      _dependees_ = resource_._dependees_;
      _dependers_ = resource_._dependers_;
      if (_cached_dependee_ids_.get() != nullptr) {
        _cached_dependee_ids_.reset(new std::set<int32_t>(*_cached_dependee_ids_.get()));
      }
      if (_cached_depender_ids_.get() != nullptr) {
        _cached_depender_ids_.reset(new std::set<int32_t>(*_cached_depender_ids_.get()));
      }
      return;
    }

    resource::resource()
    {
      _initialized_ = false;
      _set_defaults();
      return;
    }

    resource::resource(const resource & res_) : ::datatools::enriched_base(res_)
    {
      this->resource::_copy_from_(res_);
      return;
    }

    resource::~resource()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    resource & resource::operator=(const resource & res_)
    {
      if (this == &res_) return *this;
      this->resource::_copy_from_(res_);
      return *this;
    }

    bool resource::has_id() const
    {
      return _id_ != INVALID_ID;
    }

    void resource::_set_id(int32_t id_)
    {
      _id_ = id_;
      return;
    }

    void resource::set_id(int32_t id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource is already initialized with ID=[" << _id_ << "]!");
      DT_THROW_IF(id_ <= INVALID_ID,
                  std::logic_error,
                  "Invalid resource ID [" << id_ << "]!");
      // if (has_id()) {
      //   DT_THROW_IF(is_initialized(), std::logic_error,
      //               "Resource is already initialized with ID=[" << _id_ << "]!");
      // }
      _set_id(id_);
      return;
    }

    int32_t resource::get_id() const
    {
      return _id_;
    }

    bool resource::operator<(const resource & res_) const
    {
      return _id_ < res_._id_;
    }

    bool resource::has_path() const
    {
      return !_path_.empty();
    }

    void resource::set_path(const std::string & path_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Resource is already initialized!");
      DT_THROW_IF(!vire::utility::path::validate_path(path_),
                  std::logic_error,
                  "Invalid resource path '" << path_ << "'!");
      _path_ = path_;
      return;
    }

    const std::string & resource::get_path() const
    {
      return _path_;
    }

    void resource::set_max_number_of_tokens(uint32_t mnh_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource is already initialized!");
      if (mnh_ > MAX_TOKENS_UNLIMITED) {
        _max_number_of_tokens_ = MAX_TOKENS_UNLIMITED;
      } else {
        _max_number_of_tokens_ = mnh_;
      }
      return;
    }

    bool resource::has_max_number_of_tokens() const
    {
      return _max_number_of_tokens_ != MAX_TOKENS_INVALID;
    }

    uint32_t resource::get_max_number_of_tokens() const
    {
      return _max_number_of_tokens_;
    }

    bool resource::is_number_of_tokens_unlimited() const
    {
      return _max_number_of_tokens_ == MAX_TOKENS_UNLIMITED;
    }

    bool resource::is_number_of_tokens_singleton() const
    {
      return _max_number_of_tokens_ == MAX_TOKENS_SINGLETON;
    }

    bool resource::is_number_of_tokens_limited() const
    {
      return has_max_number_of_tokens() && !is_number_of_tokens_unlimited();
    }

    bool resource::has_access() const
    {
      return _access_ != vire::utility::RW_INVALID;
    }

    void resource::set_access(vire::utility::rw_access_type a_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource is already initialized!");
      _access_ = a_;
      return;
    }

    vire::utility::rw_access_type resource::get_access() const
    {
      return _access_;
    }

    bool resource::is_readable() const
    {
      return vire::utility::is_readable(_access_);
    }

    bool resource::is_writable() const
    {
      return vire::utility::is_writable(_access_);
    }

    bool resource::has_resource_instance() const
    {
      return _resource_instance_.get() != 0;
    }

    // void resource::set_resource_instance(base_resource_instance * ri_)
    // {
    //   DT_THROW_IF(is_initialized(), std::logic_error,
    //               "Resource is already initialized!");
    //   _resource_instance_.reset(ri_);
    //   return;
    // }

    void resource::set_resource_instance(const std::shared_ptr<base_resource_instance> & shri_)
    {
      _resource_instance_ = shri_;
      return;
    }

    const base_resource_instance & resource::get_resource_instance() const
    {
      DT_THROW_IF(!has_resource_instance(),
                  std::logic_error,
                  "No resource instance is set in resource '" << get_path() << "'!");
      return *_resource_instance_.get();
    }

    base_resource_instance & resource::grab_resource_instance()
    {
      DT_THROW_IF(!has_resource_instance(),
                  std::logic_error,
                  "No resource instance is set in resource '" << get_path() << "'!");
      return *_resource_instance_.get();
    }

    bool resource::has_dependers() const
    {
      return _dependers_.size() > 0;
    }

    bool resource::has_dependees() const
    {
      return _dependees_.size() > 0;
    }

    std::size_t resource::get_number_of_dependers() const
    {
      return _dependers_.size();
    }

    std::size_t resource::get_number_of_dependees() const
    {
      return _dependees_.size();
    }

    bool resource::has_depender_with_path(const std::string & path_) const
    {
      for (const auto & r : _dependers_) {
        if (r->get_path() == path_) return true;
      }
      return false;
    }

    bool resource::has_depender_with_id(int id_) const
    {
      for(const auto & r : _dependers_) {
        if (r->get_id() == id_) return true;
      }
      return false;
    }

    bool resource::has_dependee_with_path(const std::string & path_) const
    {
      for (const auto & r : _dependees_) {
        if (r->get_path() == path_) return true;
      }
      return false;
    }

    bool resource::has_dependee_with_id(int id_) const
    {
      for(const auto & r : _dependees_) {
        if (r->get_id() == id_) return true;
      }
      return false;
    }

    const resource & resource::get_depender_by_path(const std::string & path_) const
    {
      std::set<const resource *>::const_iterator found = _dependers_.end();
      for (std::set<const resource *>::const_iterator i = _dependers_.begin();
           i != _dependers_.end();
           i++) {
        if ((*i)->get_path() == path_) i = found;
      }
      DT_THROW_IF(found == _dependers_.end(), std::logic_error,
                  "No depender with path '" << path_ << "' in resource '" << get_path() << "'!");
      return **found;
    }

    const resource & resource::get_depender_by_id(int id_) const
    {
      std::set<const resource *>::const_iterator found = _dependers_.end();
      for (std::set<const resource *>::const_iterator i = _dependers_.begin();
           i != _dependers_.end();
           i++) {
        if ((*i)->get_id() == id_) i = found;
      }
      DT_THROW_IF(found == _dependers_.end(), std::logic_error,
                  "No depender with identifier '" << id_ << "' in resource '" << get_path() << "'!");
      return **found;
    }

    const resource & resource::get_dependee_by_path(const std::string & path_) const
    {
      std::set<const resource *>::const_iterator found = _dependees_.end();
      for (std::set<const resource *>::const_iterator i = _dependees_.begin();
           i != _dependees_.end();
           i++) {
        if ((*i)->get_path() == path_) i = found;
      }
      DT_THROW_IF(found == _dependees_.end(), std::logic_error,
                  "No dependee with path '" << path_ << "' in resource '" << get_path() << "'!");
      return **found;
    }

    const resource & resource::get_dependee_by_id(int id_) const
    {
      std::set<const resource *>::const_iterator found = _dependees_.end();
      for (std::set<const resource *>::const_iterator i = _dependees_.begin();
           i != _dependees_.end();
           i++) {
        if ((*i)->get_id() == id_) i = found;
      }
      DT_THROW_IF(found == _dependees_.end(), std::logic_error,
                  "No dependee with identifier '" << id_ << "' in resource '" << get_path() << "'!");
      return **found;
    }

    void resource::_add_depender(const resource & deper_, bool update_cache_)
    {
      _dependers_.insert(&deper_);
      if (_cached_depender_ids_.get() != nullptr) {
        if (update_cache_) {
          build_cached_depender_resource_ids();
        } else {
          _cached_depender_ids_.reset();
        }
      }
      return;
    }

    void resource::add_dependee(const resource & depee_, bool update_cache_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource '" << _path_ << "' is already initialized!");
      _dependees_.insert(&depee_);
      {
        resource & mutable_depee = const_cast<resource &>(depee_);
        mutable_depee._add_depender(*this);
      }
      if (_cached_dependee_ids_.get() != nullptr) {
        if (update_cache_) {
          build_cached_dependee_resource_ids();
        } else {
          _cached_dependee_ids_.reset();
        }
      }
      return;
    }

    void resource::remove_dependee_with_id(int id_, bool update_cache_)
    {
      std::set<const resource *>::iterator found = _dependees_.end();
      for (std::set<const resource *>::iterator idep = _dependees_.begin();
           idep != _dependees_.end();
           idep++) {
        const resource * res_ptr = *idep;
        if (res_ptr->get_id() == id_) {
          found = idep;
          break;
        }
      }
      DT_THROW_IF(found == _dependees_.end(),
                  std::logic_error,
                  "Resource '" << get_path() << "' has no dependee with identifier [" << id_ << "]!");
      // std::cerr << "DEVEL: resource::remove_dependee_with_id: Resource '" << get_name() << "' ("
      //           << get_path() << ") removes dependee [" << id_ << "]" << std::endl;
      {
        resource & depee = const_cast<resource &>(**found);
        depee._remove_depender_with_id(this->get_id(), update_cache_);
      }
      _dependees_.erase(found);
      if (_cached_dependee_ids_.get() != nullptr) {
        if (update_cache_) {
          // std::cerr << "DEVEL: resource::remove_dependee_with_id: Resource '" << get_name() << "' ("
          //           << get_path() << ") updates dependee cache..." << std::endl;
          build_cached_dependee_resource_ids();
        } else {
          _cached_dependee_ids_.reset();
        }
      }
      return;
    }

    void resource::remove_dependee_with_path(const std::string & path_, bool update_cache_)
    {
      std::set<const resource *>::iterator found = _dependees_.end();
      for (std::set<const resource *>::iterator idep = _dependees_.begin();
           idep != _dependees_.end();
           idep++) {
        const resource * res_ptr = *idep;
        if (res_ptr->get_path() == path_) {
          found = idep;
          break;
        }
      }
      DT_THROW_IF(found == _dependees_.end(),
                  std::logic_error,
                  "Resource '" << get_path() << "' has no dependee with path '" << path_ << "'!");
      // std::cerr << "DEVEL: resource::remove_dependee_with_id: Resource '" << get_name() << "' ("
      //           << get_path() << ") removes dependee '" << path_ << "'" << std::endl;
      {
        resource & depee = const_cast<resource &>(**found);
        depee._remove_depender_with_id(this->get_id());
      }
      _dependees_.erase(found);
      if (_cached_dependee_ids_.get() != nullptr) {
        if (update_cache_) {
          // std::cerr << "DEVEL: resource::remove_dependee_with_id: Resource '" << get_name() << "' ("
          //           << get_path() << ") updates dependee cache..." << std::endl;
          build_cached_dependee_resource_ids();
        } else {
          _cached_dependee_ids_.reset();
        }
      }
      return;
    }

    void resource::_remove_depender_with_path(const std::string & path_, bool update_cache_)
    {
      std::set<const resource *>::iterator found = _dependers_.end();
      for (std::set<const resource *>::iterator idep = _dependers_.begin();
           idep != _dependers_.end();
           idep++) {
        const resource * res_ptr = *idep;
        if (res_ptr->get_path() == path_) {
          found = idep;
          break;
        }
      }
      DT_THROW_IF(found == _dependers_.end(),
                  std::logic_error,
                  "Resource '" << get_path() << "' has no depender with path '" << path_ << "'!");
      _dependers_.erase(found);
      if (_cached_depender_ids_.get() != nullptr) {
        if (update_cache_) {
          // std::cerr << "DEVEL: resource::_remove_depender_with_path: Resource '" << get_name() << "' ("
          //           << get_path() << ") updates depender cache..." << std::endl;
          build_cached_depender_resource_ids();
        } else {
          _cached_depender_ids_.reset();
        }
      }
      return;
    }

    void resource::_remove_depender_with_id(int id_, bool update_cache_)
    {
      std::set<const resource *>::iterator found = _dependers_.end();
      for (std::set<const resource *>::iterator idep = _dependers_.begin();
           idep != _dependers_.end();
           idep++) {
        const resource * res_ptr = *idep;
        if (res_ptr->get_id() == id_) {
          found = idep;
          break;
        }
      }
      DT_THROW_IF(found == _dependers_.end(),
                  std::logic_error,
                  "Resource '" << get_path() << "' has no depender with identifier [" << id_ << "]!");
      // std::cerr << "DEVEL: resource::_remove_depender_with_id: Resource '" << get_name() << "' ("
      //           << get_path() << ") removes depender [" << id_ << "]" << std::endl;
      _dependers_.erase(found);
      if (_cached_depender_ids_.get() != nullptr) {
        if (update_cache_) {
          // std::cerr << "DEVEL: resource::_remove_depender_with_id: Resource '" << get_name() << "' ("
          //           << get_path() << ") updates depender cache..." << std::endl;
          build_cached_depender_resource_ids();
        } else {
          _cached_depender_ids_.reset();
        }
      }
      return;
    }

    const std::set<const resource *> & resource::get_dependers() const
    {
      return _dependers_;
    }

    const std::set<const resource *> & resource::get_dependees() const
    {
      return _dependees_;
    }

    void resource::_set_defaults()
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _id_ = INVALID_ID;
      _access_ = ::vire::utility::RW_INVALID;
      _max_number_of_tokens_ = MAX_TOKENS_INVALID;
      _allowed_subscription_ = true;
      _allowed_non_blocking_ = false;
      // _owner_device_ = nullptr;
      return;
    }

    // bool resource::has_owner_device() const
    // {
    //   return _owner_device_ != nullptr;
    // }

    // void resource::set_owner_device(const device & dev_)
    // {
    //   _owner_device_ = &dev_;
    //   return;
    // }

    // const device & resource::get_owner_device() const
    // {
    //   DT_THROW_IF(!has_owner_device(), std::logic_error,
    //            "Resource '" << get_path() << "' has no owner device!");
    //   return &_owner_device_;
    // }

    void resource::set_allowed_subscription(bool as_)
    {
      _allowed_subscription_ = as_;
      return;
    }

    bool resource::is_allowed_subscription() const
    {
      return _allowed_subscription_;
    }

    void resource::set_allowed_non_blocking(bool anb_)
    {
      _allowed_non_blocking_ = anb_;
      return;
    }

    bool resource::is_allowed_non_blocking() const
    {
      return _allowed_non_blocking_;
    }

    void resource::build_cached_dependee_resource_ids()
    {
      if (_cached_dependee_ids_.get() == nullptr) {
        _cached_dependee_ids_.reset(new std::set<int32_t>);
        for (auto r : _dependees_) {
          _cached_dependee_ids_->insert(r->get_id());
        }
      }
      return;
    }

    void resource::build_cached_depender_resource_ids()
    {
      if (_cached_depender_ids_.get() == nullptr) {
        _cached_depender_ids_.reset(new std::set<int32_t>);
        for (auto r : _dependers_) {
          _cached_depender_ids_->insert(r->get_id());
        }
      }
      return;
    }

    const std::set<int32_t> & resource::get_dependers_ids() const
    {
      bool build_it = false;
      if (_cached_depender_ids_.get() == nullptr) {
        build_it = true;
      }
      if (build_it) {
        resource * mutable_this = const_cast<resource*>(this);
        mutable_this->build_cached_depender_resource_ids();
      }
      return *_cached_depender_ids_.get();
    }

    const std::set<int32_t> & resource::get_dependees_ids() const
    {
      bool build_it = false;
      if (_cached_dependee_ids_.get() == nullptr) {
        build_it = true;
      }
      if (build_it) {
        resource * mutable_this = const_cast<resource*>(this);
        mutable_this->build_cached_dependee_resource_ids();
      }
      return *_cached_dependee_ids_.get();
    }

    bool resource::is_initialized() const
    {
      return _initialized_;
    }

    void resource::initialize_simple()
    {
      datatools::properties dummy;
      initialize(dummy);
      return;
    }

    void resource::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource is already initialized!");
      this->datatools::enriched_base::initialize(config_);

      if (! has_id()) {
        if (config_.has_key("id")) {
          set_id(config_.fetch_integer("id"));
        }
      }

      if (! has_path()) {
        if (config_.has_key("path")) {
          set_path(config_.fetch_string("path"));
        }
      }

      if (! has_access()) {
        if (config_.has_key("access")) {
          const std::string & rw_access_label = config_.fetch_string("access");
          ::vire::utility::rw_access_type rw_access;
          DT_THROW_IF(!::vire::utility::from_string(rw_access_label,rw_access),
                      std::logic_error,
                      "Invalid R/W access label '" << rw_access_label << "'!");
          set_access(rw_access);
        }
      }

      if (! has_max_number_of_tokens()) {
        if (config_.has_key("max_number_of_tokens")) {
          set_max_number_of_tokens(config_.fetch_integer("max_number_of_tokens"));
        }
      }

      // TODO: Support for dependees

      DT_THROW_IF(! has_id(), std::logic_error, "Missing resource identifier (RID)!");
      DT_THROW_IF(! has_path(), std::logic_error, "Missing resource path!");
      DT_THROW_IF(! has_access(), std::logic_error, "Missing resource R/W access mode!");

      if (! has_max_number_of_tokens()) {
        // Default max cardinality depending on the resource access mode:
        if (::vire::utility::is_writable(_access_)) {
          set_max_number_of_tokens(MAX_TOKENS_SINGLETON);
        } else if (::vire::utility::is_readable(_access_)) {
          set_max_number_of_tokens(MAX_TOKENS_UNLIMITED);
        } else {
          set_max_number_of_tokens(MAX_TOKENS_INVALID);
        }
      }

      DT_THROW_IF(! is_valid(), std::logic_error, "Invalid resource description!");

      _initialized_ = true;
      return;
    }

    void resource::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Resource is not initialized!");
      _initialized_ = false;
      _cached_dependee_ids_.reset();
      _cached_depender_ids_.reset();
      for (auto depee : _dependees_) {
        remove_dependee_with_id(depee->get_id(), true);
      }
      _dependees_.clear();
      if (has_dependers()) {
        // DT_THROW(std::logic_error, "Resource '" << get_path() << "' has dependers!");
        DT_LOG_WARNING(datatools::logger::PRIO_WARNING,
                       "Resource '" << get_path() << "' has dependers!");
        for (auto deper : _dependers_) {
          resource * mutable_deper = const_cast<resource *>(deper);
          DT_LOG_WARNING(datatools::logger::PRIO_WARNING,
                         "Force depender '" << deper->get_path() << "' to remove dependee resource '" << get_path() << "'!");
          mutable_deper->remove_dependee_with_path(this->get_path(), true);
        }
        _dependers_.clear();
      }
      _resource_instance_.reset();
      _path_.clear();
      _set_defaults();
      this->datatools::enriched_base::clear();
      return;
    }

    bool resource::is_valid() const
    {
      if (!has_id()) return false;
      if (!has_path()) return false;
      if (!has_access()) return false;
      if (!has_max_number_of_tokens()) return false;
      return true;
    }

    void resource::tree_dump(std::ostream & out_,
                             const std::string & title_,
                             const std::string & indent_,
                             bool inherit_) const
    {
      this->datatools::enriched_base::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Resource ID : ";
      if (has_id()) {
        out_ << '[' << _id_ << ']';
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Path : ";
      if (has_path()) {
        out_ << "'" << _path_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Access : ";
      if (has_access()) {
        out_ << "'" << ::vire::utility::to_string(_access_) << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Max number of tokens : ";
      if (has_max_number_of_tokens()) {
        if (is_number_of_tokens_unlimited()) {
          out_ << "<unlimited>";
        } else if (is_number_of_tokens_singleton()) {
          out_ << "<singleton>";
        } else if (is_number_of_tokens_limited()) {
          out_ << "limited to [" << _max_number_of_tokens_ << ']';
        }
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Allowed subscription : " << _allowed_subscription_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Allowed non blocking : " << _allowed_non_blocking_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Resource instance : ";
      if (has_resource_instance()) {
        out_ << "'" << resource_category_to_label(get_resource_instance().get_resource_category()) << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Dependers : ";
      if (_dependers_.size() == 0) {
        out_ << "<none>" << std::endl;
      } else {
        out_ << '[' << _dependers_.size() << ']' << std::endl;
      }
      for (std::set<const resource *>::const_iterator ideper = _dependers_.begin();
           ideper != _dependers_.end();
           ideper++) {
        out_ << indent_ << i_tree_dumpable::skip_tag;
        std::set<const resource *>::const_iterator jdeper = ideper;
        jdeper++;
        if (jdeper == _dependers_.end()) {
          out_ << i_tree_dumpable::last_tag;
        } else {
          out_ << i_tree_dumpable::tag;
        }
        out_ << "Depender : '" << (*ideper)->get_path() << "'" << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Dependees : ";
      if (_dependees_.size() == 0) {
        out_ << "<none>";
      } else {
        out_ << '[' << _dependees_.size() << ']';
      }
      out_ << std::endl;
      for (std::set<const resource *>::const_iterator idepee = _dependees_.begin();
           idepee != _dependees_.end();
           idepee++) {
        out_ << indent_ << i_tree_dumpable::skip_tag;
        std::set<const resource *>::const_iterator jdepee = idepee;
        jdepee++;
        if (jdepee == _dependees_.end()) {
          out_ << i_tree_dumpable::last_tag;
        } else {
          out_ << i_tree_dumpable::tag;
        }
        out_ << "Dependee : '" << (*idepee)->get_path() << "'" << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Cached dependees ids: ";
      if (_cached_dependee_ids_.get() == nullptr) {
        out_ << "<no>" << std::endl;
      } else {
        out_ << "<yes>" << std::endl;
        for (std::set<int32_t>::const_iterator idepee = _cached_dependee_ids_.get()->begin();
             idepee != _cached_dependee_ids_.get()->end();
             idepee++) {
          out_ << indent_ << i_tree_dumpable::skip_tag;
          std::set<int32_t>::const_iterator jdepee = idepee;
          jdepee++;
          if (jdepee == _cached_dependee_ids_.get()->end()) {
            out_ << i_tree_dumpable::last_tag;
          } else {
            out_ << i_tree_dumpable::tag;
          }
          out_ << "Dependee : [" << *idepee << "]" << std::endl;
        }
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Cached dependers ids: ";
      if (_cached_depender_ids_.get() == nullptr) {
        out_ << "<no>" << std::endl;
      } else {
        out_ << "<yes>" << std::endl;
        for (std::set<int32_t>::const_iterator ideper = _cached_depender_ids_.get()->begin();
             ideper != _cached_depender_ids_.get()->end();
             ideper++) {
          out_ << indent_ << i_tree_dumpable::skip_tag;
          std::set<int32_t>::const_iterator jdeper = ideper;
          jdeper++;
          if (jdeper == _cached_depender_ids_.get()->end()) {
            out_ << i_tree_dumpable::last_tag;
          } else {
            out_ << i_tree_dumpable::tag;
          }
          out_ << "Depender : [" << *ideper << "]" << std::endl;
        }
      }

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << is_valid() << std::endl;

      return;
    }

  } // namespace resource

} // namespace vire
