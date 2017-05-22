//! \file vire/com/boost_serialization_encoding_driver.cc
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
#include <vire/com/boost_serialization_encoding_driver.h>

// Third party:
// - Boost:
#include <boost/archive/codecvt_null.hpp>
#include <boost/math/special_functions/nonfinite_num_facets.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/eos/portable_oarchive.hpp>
#include <bayeux/datatools/eos/portable_iarchive.hpp>

namespace vire {

  namespace com {

    VIRE_COM_ENCODING_DRIVER_REGISTRATION_IMPLEMENTATION(boost_serialization_encoding_driver,
                                                         "vire::com::boost_serialization_encoding_driver")

    boost_serialization_encoding_driver::boost_serialization_encoding_driver()
    {
      return;
    }

    boost_serialization_encoding_driver::~boost_serialization_encoding_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void boost_serialization_encoding_driver::set_archive_format(const archive_format_type af_)
    {
      _archive_format_ = af_;
      return;
    }

    boost_serialization_encoding_driver::archive_format_type
    boost_serialization_encoding_driver::get_archive_format() const
    {
      return _archive_format_;
    }

    void boost_serialization_encoding_driver::_initialize_impl_(const datatools::properties & config_)
    {
      if (config_.has_key("archive_format")) {
        const std::string & af_label = config_.fetch_string("archive_format");
        if (af_label == "xml") {
          set_archive_format(AR_XML);
        } else if (af_label == "text") {
          set_archive_format(AR_TEXT);
        } else {
          DT_THROW(std::logic_error, "Invalid archive format '" << af_label << "'!");
        }
      }
      _default_locale_ = new std::locale(std::locale::classic(),
                                         new boost::archive::codecvt_null<char>);
      _out_locale_ = new std::locale(*_default_locale_,
                                     new boost::math::nonfinite_num_put<char>);
      _in_locale_ = new std::locale(*_default_locale_,
                                     new boost::math::nonfinite_num_get<char>);
      return;
    }

    void boost_serialization_encoding_driver::_reset_impl_()
    {
      set_archive_format(AR_XML);
      if (_out_locale_) {
        delete _out_locale_;
        _out_locale_ = 0;
      }
      if (_in_locale_) {
        delete _in_locale_;
        _in_locale_ = 0;
      }
      if (_default_locale_) {
        delete _default_locale_;
        _default_locale_ = 0;
      }
      return;
    }

    int boost_serialization_encoding_driver::_encode_impl_(const vire::message::message & msg_,
                                                          std::vector<char> & buffer_)
    {
      boost::iostreams::stream<boost::iostreams::back_insert_device<std::vector<char> > > out_stream(buffer_);
      out_stream.imbue(*_out_locale_);
      if (_archive_format_ == AR_XML) {
        boost::archive::xml_oarchive oxar(out_stream, boost::archive::no_codecvt);
        oxar << boost::serialization::make_nvp("message", msg_);
      } else if (_archive_format_ == AR_TEXT) {
        boost::archive::text_oarchive otar(out_stream, boost::archive::no_codecvt);
        otar << msg_;
      } else if (_archive_format_ == AR_PBA) {
        eos::portable_oarchive obar(out_stream);
        obar << msg_;
      }
      out_stream << std::flush;
      return 0;
    }

    int boost_serialization_encoding_driver::_decode_impl_(const std::vector<char> & buffer_,
                                                           vire::message::message & msg_)
    {
      boost::iostreams::basic_array_source<char> in_source(&buffer_[0], buffer_.size());
      boost::iostreams::stream<boost::iostreams::basic_array_source<char> > in_stream(in_source);
      in_stream.imbue(*_in_locale_);
      if (_archive_format_ == AR_XML) {
        boost::archive::xml_iarchive ixar(in_stream, boost::archive::no_codecvt);
        ixar >> boost::serialization::make_nvp("message", msg_);
      } else if (_archive_format_ == AR_TEXT) {
        boost::archive::text_iarchive itar(in_stream, boost::archive::no_codecvt);
        itar >> msg_;
      } else if (_archive_format_ == AR_PBA) {
        eos::portable_iarchive ibar(in_stream);
        ibar >> msg_;
      }
      return 0;
    }

  } // namespace com

} // namespace vire
