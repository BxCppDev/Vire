//! \file  vire/base_object_protobuf.h
//! \brief handle inheritance for Protobufable classes
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

#ifndef VIRE_BASE_OBJECT_PROTOBUF_H
#define VIRE_BASE_OBJECT_PROTOBUF_H

// - Bayeux/BxProtobuftools:
#include <bayeux/protobuftools/node.h>
#include <bayeux/protobuftools/protobufable_converter.h>

#define VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(BaseType,Node)        \
  {                                                                     \
    BaseType & _base = static_cast<BaseType &>(*this);                  \
    Node["_base"].operator%<BaseType>(_base);                           \
  }                                                                     \
  /**/

#endif //VIRE_BASE_OBJECT_PROTOBUF_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
