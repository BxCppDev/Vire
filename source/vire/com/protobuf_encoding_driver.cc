//! \file vire/com/protobuf_encoding_driver.cc
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
#include <vire/com/protobuf_encoding_driver.h>

// Third party:
// - Boost:
#include <boost/math/special_functions/nonfinite_num_facets.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/archive/codecvt_null.hpp>
// - Bxprotobuftools:
#include <protobuftools/io.h>

namespace vire {

  namespace com {

    VIRE_COM_ENCODING_DRIVER_REGISTRATION_IMPLEMENTATION(protobuf_encoding_driver,
                                                         "vire::com::protobuf_encoding_driver")

    protobuf_encoding_driver::protobuf_encoding_driver()
    {
      return;
    }

    protobuf_encoding_driver::~protobuf_encoding_driver()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    std::string protobuf_encoding_driver::_class_guid_() const
    {
      return protobuf_encoding_driver::system_factory_auto_registration_id();
    }

    void protobuf_encoding_driver::_initialize_impl_(const datatools::properties & config_)
    {
      return;
    }

    void protobuf_encoding_driver::_reset_impl_()
    {
      return;
    }

    int protobuf_encoding_driver::_encode_impl_(const vire::message::message & msg_,
                                                raw_message_type & raw_msg_)
    {
      boost::iostreams::stream<boost::iostreams::back_insert_device<std::vector<char>>> out(raw_msg_.buffer);
      uint32_t io_flags = 0;
      if (datatools::logger::is_debug(get_logging())) {
        io_flags |= protobuftools::IO_DEBUG;
      }
      protobuftools::store(out, msg_, io_flags);
      out << std::flush;
      return 0;
    }

    int protobuf_encoding_driver::_decode_impl_(const raw_message_type & raw_msg_,
                                                vire::message::message & msg_)
    {
      boost::iostreams::basic_array_source<char> in_source(&raw_msg_.buffer[0], raw_msg_.buffer.size());
      boost::iostreams::stream<boost::iostreams::basic_array_source<char> > in_stream(in_source);
      uint32_t io_flags = 0;
      if (datatools::logger::is_debug(get_logging())) {
        io_flags |= protobuftools::IO_DEBUG;
      }
      protobuftools::load(in_stream, msg_, io_flags);
      return 0;
    }

  } // namespace com

} // namespace vire
