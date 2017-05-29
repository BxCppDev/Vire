//! \file  vire/message/message-serial.ipp
//! \brief Message serialization
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

#ifndef VIRE_MESSAGE_MESSAGE_SERIAL_IPP
#define VIRE_MESSAGE_MESSAGE_SERIAL_IPP

// Ourselves:
#include <vire/message/message.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>

namespace vire {

  namespace message {

    template<class Archive>
    void message::serialize(Archive & archive_,
                            const unsigned int /* version_*/)
    {
      archive_ & DATATOOLS_SERIALIZATION_I_SERIALIZABLE_BASE_OBJECT_NVP;
      archive_ & boost::serialization::make_nvp("header", _header_);
      archive_ & boost::serialization::make_nvp("body",   _body_);
      return;
    }

  } // end of namespace message

} // end of namespace vire

#endif // VIRE_MESSAGE_MESSAGE_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
