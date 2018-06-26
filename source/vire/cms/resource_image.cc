// vire/cms/resource_image.cc - Resource CMS image
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
#include <vire/cms/resource_image.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>
//#include <datatools/logger.h>

namespace vire {

  namespace cms {
   
    resource_image::resource_image(const vire::resource::resource & resource_)
      : _res_(&resource_)
    {
      if (!can_store_value()) {
        grab_monitoring().disallow_value();
      }
      return;
    }
    
    resource_image::~resource_image()
    {
      return;
    }

    void resource_image::reset()
    {
      _res_ = nullptr;
      base_image::reset();
      return;
    }

    const std::string & resource_image::get_path() const
    {
      return _res_->get_path();
    }

    bool resource_image::is_resource() const 
    {
      return true;
    }

    bool resource_image::can_store_value() const
    {
      vire::utility::rw_access_type rw_access = _res_->get_access();
      return vire::utility::is_readable(rw_access);
    }

    const vire::resource::resource & resource_image::get_resource() const
    {
      return *_res_;
    }

    void resource_image::print_tree(std::ostream & out_,
                                    const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      base_image::print_tree(out_, base_print_options::force_inheritance(options_));
      
      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Resource : " << "'" << _res_->get_name() << "'"
           << std::endl;
       
      return;
    }

  } // namespace cms

} // namespace vire
