// vire/cms/base_image.cc - Device/resource CMS base image
//
// Copyright (c) 2017-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cms/base_image.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

namespace vire {

  namespace cms {
    
    base_image_signal_emitter::base_image_signal_emitter(const base_image & image_)
      : _image_(image_)
    {
      return;
    }
    
    void base_image_signal_emitter::emit_value_change()
    {
      emit sig_value_changed();
      return;
    }
   
    void base_image_signal_emitter::emit_status_change()
    {
      emit sig_status_changed();
      return;
    }
    
    const base_image & base_image_signal_emitter::get_image() const
    {
      return _image_;
    }
  
    base_image_signal_emitter & base_image::grab_emitter()
    {
      if (!_emitter_) {
        _emitter_.reset(new base_image_signal_emitter(*this));
      }
      return *_emitter_.get();
    }

    const base_image_signal_emitter & base_image::get_emitter() const
    {
      base_image * mutable_this = const_cast<base_image*>(this);
      return mutable_this->grab_emitter();
    }

    void base_image::on_value_change()
    {
      grab_emitter().emit_value_change();
      return;
    }

    void base_image::on_status_change()
    {
      grab_emitter().emit_status_change();
      return;
    } 

    base_image::base_image()
    {
      return;
    }
    
    base_image::~base_image()
    {
      return;
    }

    // virtual
    bool base_image::is_device() const
    {
      return false;
    }

    // virtual
    bool base_image::is_resource() const 
    {
      return false;
    }
    
    // virtual
    bool base_image::can_store_value() const
    {
      return true;
    }

    // virtual
    void base_image::reset()
    {
      reset_monitoring();
      clear_metadata();
      return;
    }

    // virtual
    const std::string & base_image::get_path() const
    {
      static std::string _p;
      return _p;
    }

    const monitoring_record & base_image::get_monitoring() const
    {
      return _monitoring_;
    }
    
    monitoring_record & base_image::grab_monitoring()
    {
      return _monitoring_;
    }
    
    void base_image::reset_monitoring()
    {
      _monitoring_.reset();
      return;
    }
 
    const image_status & base_image::get_status() const
    {
      return _monitoring_.get_status();
    }

    image_status & base_image::grab_status()
    {
      return _monitoring_.grab_status();
    }

    void base_image::update_status(const resource_status_record & record_)
    {
      DT_THROW_IF(record_.get_path() != get_path(),
                  std::logic_error,
                  "Resource status record's path '"
                  << record_.get_path()
                  << "' does not match the image's path '"
                  << get_path() << "'");
      _monitoring_.grab_status().set_timestamp(record_.get_timestamp());
      _monitoring_.grab_status().set_missing(record_.is_missing());
      _monitoring_.grab_status().set_disabled(record_.is_disabled());
      _monitoring_.grab_status().set_pending(record_.is_pending());
      _monitoring_.grab_status().set_failed(record_.is_failed());

      return;
    }

    bool base_image::has_metadata(const std::string & key_) const
    {
      return _metadata_.count(key_) > 0;
    }

    void base_image::set_metadata(const std::string & key_,
                                  const std::string & value_)
    {
      _metadata_[key_] = value_;
      return;
    }

    void base_image::remove_metadata(const std::string & key_)
    {
      _metadata_.erase(key_);
      return;
    }

    const std::string & base_image::get_metadata(const std::string & key_) const
    {
      metadata_col_type::const_iterator found = _metadata_.find(key_);
      DT_THROW_IF(found == _metadata_.end(),
                  std::logic_error,
                  "No metadata with key '" << key_ << "' in image '" << get_path() << "'!");
      return found->second;
    }
    
    const base_image::metadata_col_type & base_image::get_metadata() const
    {
      return _metadata_;
    }

    void base_image::clear_metadata()
    {
      _metadata_.clear();
      return;
    }

    void base_image::print_tree(std::ostream & out_,
                                const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      const std::string & indent = popts.indent;
      if (! popts.title.empty ()) {
        out_ << indent << popts.title << std::endl;
      }

      out_ << indent << tag
           << "Path      : '" << get_path() << "'"
           << std::endl;
  
      out_ << indent << tag
           << "Device    : " << std::boolalpha << this->is_device()
           << std::endl;

      out_ << indent << tag
           << "Resource  : " << std::boolalpha << this->is_resource()
           << std::endl;

      out_ << indent << tag
           << "Can store value : " << std::boolalpha << this->can_store_value()
           << std::endl;
     
      out_ << indent << tag
           << "Monitoring record : " << std::endl;
      {
        std::ostringstream indent2ss;
        indent2ss << indent << skip_tag;
        boost::property_tree::ptree options2;
        options2.put(base_print_options::indent_key(),
                     indent2ss.str());
        _monitoring_.print_tree(out_, options2);
      }
       
      out_ << indent << inherit_tag(popts.inherit)
           << "Metadata  : " << _metadata_.size()
           << std::endl;
      {
        std::size_t count = 0;
        for (metadata_col_type::const_iterator i = _metadata_.begin();
             i !=  _metadata_.end();
             i++) {
          out_ << indent << inherit_skip_tag(popts.inherit);
          if (++count == _metadata_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Key='" << i->first << "' : Value='" << i->second << "'";
          out_ << std::endl;
        }
      }

      return;
    }
 
  } // namespace cms

} // namespace vire
