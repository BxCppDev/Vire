//! \file vire/com/json_encoding_driver.cc
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
#include <vire/com/json_encoding_driver.h>

// Third party:
// - Boost:
#include <boost/math/special_functions/nonfinite_num_facets.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/archive/codecvt_null.hpp>
// - BxJsontools:
#include <jsontools/io.h>

namespace vire {

  namespace com {

    VIRE_COM_ENCODING_DRIVER_REGISTRATION_IMPLEMENTATION(json_encoding_driver,
                                                         "vire::com::json_encoding_driver")

    json_encoding_driver::json_encoding_driver()
    {
      return;
    }

    json_encoding_driver::~json_encoding_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    std::string json_encoding_driver::_class_guid_() const
    {
      return json_encoding_driver::system_factory_auto_registration_id();
    }


    void json_encoding_driver::_initialize_impl_(const datatools::properties & config_)
    {
      _default_locale_ = new std::locale(std::locale::classic(),
                                         new boost::archive::codecvt_null<char>);
      _out_locale_ = new std::locale(*_default_locale_,
                                     new boost::math::nonfinite_num_put<char>);
      _in_locale_ = new std::locale(*_default_locale_,
                                    new boost::math::nonfinite_num_get<char>);
      return;
    }

    void json_encoding_driver::_reset_impl_()
    {
      if (_out_locale_) {
        delete _out_locale_;
        _out_locale_ = nullptr;
      }
      if (_in_locale_) {
        delete _in_locale_;
        _in_locale_ = nullptr;
      }
      if (_default_locale_) {
        delete _default_locale_;
        _default_locale_ = nullptr;
      }
      return;
    }

    int json_encoding_driver::_encode_impl_(const vire::message::message & msg_,
                                            raw_message_type & raw_msg_) const
    {
      boost::iostreams::stream<boost::iostreams::back_insert_device<std::vector<char> > > out_stream(raw_msg_.buffer);
      out_stream.imbue(*_out_locale_);
      jsontools::store(out_stream, msg_);
      out_stream << std::flush;
      return 0;
    }

    int json_encoding_driver::_decode_impl_(const raw_message_type & raw_msg_,
                                            vire::message::message & msg_) const
    {
      boost::iostreams::basic_array_source<char> in_source(&raw_msg_.buffer[0], raw_msg_.buffer.size());
      boost::iostreams::stream<boost::iostreams::basic_array_source<char> > in_stream(in_source);
      in_stream.imbue(*_in_locale_);
      jsontools::load(in_stream, msg_);
      return 0;
    }

  } // namespace com

} // namespace vire
