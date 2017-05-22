// vire/cms/image.cc - Device/resource CMS image
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cms/image.h>

// Third Party:
// - Boost:
#include <boost/filesystem/path.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/logger.h>

// This project:
#include <vire/cms/utils.h>
#include <vire/utility/path.h>
#include <vire/device/base_datapoint_model.h>

namespace vire {

  namespace cms {

    void image::resource_info::reset()
    {
      res = nullptr;
      return;
    }

    bool image::resource_info::is_valid() const
    {
      return res != nullptr;
    }

    void image::device_info::reset()
    {
      path.clear();
      log = nullptr;
      return;
    }

    bool image::device_info::is_valid() const
    {
      if (path.empty()) return false;
      if (log == nullptr) return false;
      return true;
    }

    image::image()
    {
      _parent_    = nullptr;
      _timestamp_ = vire::time::invalid_time();
      return;
    }

    image::image(const vire::resource::resource & r_)
      : image()
    {
      _resource_.res = &r_;
      std::string setup;
      std::string dirs;
      std::string leaf;
      vire::utility::path::extract(r_.get_path(), setup, dirs, leaf);
      _set_leaf_name_(leaf);
      return;
    }

    image::image(const std::string & device_path_,
                 const vire::device::logical_device & device_log_)
      : image()
    {
      _device_.path = device_path_;
      _device_.log = & device_log_;
      std::string setup;
      std::string dirs;
      std::string leaf;
      vire::utility::path::extract(_device_.path, setup, dirs, leaf);
      _set_leaf_name_(leaf);
      return;
    }

    image::~image()
    {
      reset();
      return;
    }

    bool image::is_valid() const
    {
      return _device_.is_valid() || _resource_.is_valid();
    }

    bool image::has_path() const
    {
      return !_device_.path.empty() || _resource_.res->has_path();
    }

    const std::string & image::get_path() const
    {
      if (_device_.is_valid()) return _device_.path;
      if (_resource_.res->has_path()) return _resource_.res->get_path();
    }

    const std::string & image::get_leaf_name() const
    {
      return _leaf_name_;
    }

    void image::_set_leaf_name_(const std::string & ln_)
    {
      _leaf_name_ = ln_;
      return;
    }

    bool image::has_group() const
    {
      return !_group_.empty();
    }

    const std::string & image::get_group() const
    {
      return _group_;
    }

    void image::set_group(const std::string & group_)
    {
      _group_ = group_;
      return;
    }

    bool image::is_device() const
    {
      return _device_.is_valid();
    }

    void image::reset_device()
    {
      _device_.reset();
      return;
    }

    bool image::is_resource() const
    {
      return _resource_.is_valid();
    }

    void image::reset_resource()
    {
      _resource_.reset();
      return;
    }

    const vire::device::logical_device & image::get_log_device() const
    {
      DT_THROW_IF(!is_device(), std::logic_error, "No device info!");
      return *_device_.log;
    }

    const vire::resource::resource & image::get_resource() const
    {
      DT_THROW_IF(!is_resource(), std::logic_error, "No resource info!");
      return *_resource_.res;
    }

    void image::add_daughter(image & img_)
    {
      DT_THROW_IF(is_resource(), std::logic_error, "Resource image '" << get_path() << "' cannot have daughters!");
      DT_THROW_IF(img_.has_parent(), std::logic_error, "Image '" << img_.get_path() << "' already has a parent!");
      _daughters_.insert(std::pair<std::string, image *>(img_.get_leaf_name(), &img_));
      return;
    }

    bool image::has_parent() const
    {
      return _parent_ != nullptr;
    }

    void image::set_parent(image & parent_, const std::string & daughter_name_)
    {
      _parent_ = &parent_;
      std::string dn = daughter_name_;
      daughter_dict_type & pdaughters = parent_.grab_daughters();
      //  const_cast<daughter_dict_type &>(parent_.grab_daughters());
      pdaughters[dn] = this;
      return;
    }

    const image & image::get_parent() const
    {
      DT_THROW_IF(!has_parent(), std::logic_error, "No parent image!");
      return *_parent_;
    }

    image & image::grab_parent()
    {
      DT_THROW_IF(!has_parent(), std::logic_error, "No parent image!");
      return *_parent_;
    }

    image::daughter_dict_type & image::grab_daughters()
    {
      return _daughters_;
    }

    const image::daughter_dict_type & image::get_daughters() const
    {
      return _daughters_;
    }

    void image::reset()
    {
      _values_.clear();
      reset_pubsub();
      reset_missing();
      reset_disabled();
      reset_pending();
      reset_failed();
      vire::time::invalidate_time(_timestamp_);
      _daughters_.clear();
      _parent_ = nullptr;
      if (is_resource()) {
        reset_resource();
      }
      if (is_device()) {
        reset_device();
      }
      _leaf_name_.clear();
      return;
    }

    bool image::has_timestamp() const
    {
      return vire::time::is_valid(_timestamp_);
    }

    void image::set_timestamp(const boost::posix_time::ptime & t_)
    {
      _timestamp_ = t_;
      return;
    }

    const boost::posix_time::ptime & image::get_timestamp() const
    {
      return _timestamp_;
    }

    bool image::has_missing() const
    {
      return _missing_;
    }

    bool image::is_present() const
    {
      if (!_missing_) {
        return true;
      }
      return false;
    }

    bool image::is_missing() const
    {
      if (_missing_) {
        return true;
      }
      return false;
    }

    void image::set_missing(bool m_)
    {
      _missing_ = m_;
      return;
    }

    void image::reset_missing()
    {
      _missing_ = boost::logic::indeterminate;
      return;
    }

    bool image::is_enabled() const
    {
      if (!_disabled_) {
        return true;
      }
      return false;
    }

    bool image::is_disabled() const
    {
      if (_disabled_) {
        return true;
      }
      return false;
    }

    void image::set_disabled(bool d_)
    {
      _disabled_ = d_;
      return;
    }

    void image::reset_disabled()
    {
      _disabled_ = boost::logic::indeterminate;
      return;
    }

    bool image::is_idle() const
    {
      if (!_pending_) {
        return true;
      }
      return false;
    }

    bool image::is_pending() const
    {
      if (_pending_) {
        return true;
      }
      return false;
    }

    void image::set_pending(bool p_)
    {
      _pending_ = p_;
      return;
    }

    void image::reset_pending()
    {
      _pending_ = boost::logic::indeterminate;
      return;
    }

    bool image::is_success() const
    {
      if (!_failed_) {
        return true;
      }
      return false;
    }

    bool image::is_failed() const
    {
      if (_failed_) {
        return true;
      }
      return false;
    }

    void image::set_failed(bool e_)
    {
      _failed_ = e_;
      return;
    }

    void image::reset_failed()
    {
      _failed_ = boost::logic::indeterminate;
      return;
    }

    bool image::is_pubsub() const
    {
      if (_pubsub_) {
        return true;
      }
      return false;
    }

    void image::set_pubsub(bool e_)
    {
      _pubsub_ = e_;
      return;
    }

    void image::reset_pubsub()
    {
      _pubsub_ = boost::logic::indeterminate;
      return;
    }

    void image::indeterminate_status()
    {
      reset_missing();
      reset_disabled();
      reset_pending();
      reset_failed();
      return;
    }

    bool image::can_value() const
    {
      if (!is_resource()) return false;
      if (get_leaf_name() == vire::device::base_datapoint_model::read_method_name()) return true;
      return false;
    }

    bool image::has_value(std::size_t index_) const
    {
      if (!can_value()) return false;
      return (_values_.count(index_) == 1);
    }

    void image::reset_value(std::size_t index_)
    {
      _values_.erase(index_);
      return;
    }

    void image::reset_values()
    {
      _values_.clear();
      return;
    }

    std::size_t image::get_values_size() const
    {
      return 1;
    }

    std::size_t image::get_number_of_set_values() const
    {
      return _values_.size();
    }

    void image::set_boolean_value(const bool value_, std::size_t index_)
    {
      DT_THROW_IF(!can_value(), std::logic_error,
                  "Can not host value!")
      _values_[index_] = value_;
    }

    bool image::get_boolean_value(std::size_t index_) const
    {
      DT_THROW_IF(index_ >= _values_.size(), std::logic_error,
                  "No value at index '" << index_ << "'!");
      return boost::get<bool>(_values_.find(index_)->second);
    }

    int32_t image::get_integer_value(std::size_t index_) const
    {
      DT_THROW_IF(index_ >= _values_.size(), std::logic_error,
                  "No value at index '" << index_ << "'!");
      return boost::get<int32_t>(_values_.find(index_)->second);
    }

    double image::get_real_value(std::size_t index_) const
    {
      DT_THROW_IF(index_ >= _values_.size(), std::logic_error,
                  "No value at index '" << index_ << "'!");
      return boost::get<double>(_values_.find(index_)->second);
    }

    const std::string & image::get_string_value(std::size_t index_) const
    {
      DT_THROW_IF(index_ >= _values_.size(), std::logic_error,
                  "No value at index '" << index_ << "'!");
      return boost::get<std::string>(_values_.find(index_)->second);
    }


    void image::update(const resource_status_record & record_)
    {
      DT_THROW_IF(record_.get_path() != get_path(),
                  std::logic_error,
                  "Resource status record's path '"
                  << record_.get_path()
                  << "' does not match the image's path '"
                  << get_path() << "'");
      set_missing(record_.is_missing());
      set_disabled(record_.is_disabled());
      set_pending(record_.is_pending());
      set_failed(record_.is_failed());
      return;
    }

    void image::start()
    {
      _at_start_();
      return;
    }

    void image::_at_start_()
    {
      if (can_value()) ;
      return;
    }

    void image::tree_dump(std::ostream & out_,
                          const std::string & title_,
                          const std::string & indent_,
                          bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Path      : ";
      if (has_path()) {
        out_ << "'" << get_path() << "'";
        if (is_device()) {
          out_ << " (device)";
        } else if (is_resource()) {
          out_ << " (resource)";
        }
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Validity  : " << std::boolalpha << is_valid()
           << std::endl;

      if (is_device()) {
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Group     : ";
      if (has_group()) {
        out_ << "'" << _group_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Timestamp : " << '[' << boost::posix_time::to_iso_string(_timestamp_) << ']'
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Missing   : " << repr(_missing_)
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Disabled  : " << repr(_disabled_)
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Pending   : " << repr(_pending_)
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Failed    : " << repr(_failed_)
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Pub/Sub   : " << repr(_pubsub_)
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Value     : ";
      if (has_value()) {
        out_ << "(size=" << get_values_size() << ")";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace cms

} // namespace vire
