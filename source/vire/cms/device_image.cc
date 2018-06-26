// vire/cms/device_image.cc - Device CMS image
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
#include <vire/cms/device_image.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>
//#include <datatools/logger.h>

namespace vire {

  namespace cms {
   
    device_image::device_image(const std::string & path_,
                               const vire::device::logical_device & logical_)
      : _path_(path_)
      , _logical_(&logical_)
    {
      grab_monitoring().disallow_value();
      return;
    }
    
    device_image::~device_image()
    {
      return;
    }

    void device_image::reset()
    {
      _logical_ = nullptr;
      _path_.clear();
      base_image::reset();
     return;
    }
 
    bool device_image::is_device() const 
    {
      return true;
    }
 
    bool device_image::is_resource() const 
    {
      return false;
    }

    bool device_image::can_store_value() const
    {
      return false;
    }

    const std::string & device_image::get_path() const 
    {
      return _path_;
    }

    const vire::device::logical_device & device_image::get_logical() const
    {
      return *_logical_;
    }

    void device_image::print_tree(std::ostream & out_,
                                  const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      base_image::print_tree(out_, base_print_options::force_inheritance(options_));
      
      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Device logical : " << "'" << _logical_->get_name() << "'"
           << std::endl;
       
      return;
    }

  } // namespace cms

} // namespace vire
