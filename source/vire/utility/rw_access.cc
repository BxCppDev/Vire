// vire/utility/rw_access.cc
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

// Ourselves:
#include <vire/utility/rw_access.h>

// Third party:
// - Camp:
#include <camp/enum.hpp>
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>

namespace vire {

  namespace utility {

    const std::string & to_string(const rw_access_type t_)
    {
      const camp::Enum & metaAccessTypeEnum = camp::enumByType<rw_access_type>();
      return metaAccessTypeEnum.name(t_);
    }

    bool from_string(const std::string & label_, rw_access_type & type_)
    {
      type_ = RW_INVALID;
      const camp::Enum & metaAccessTypeEnum = camp::enumByType<rw_access_type>();
      if (metaAccessTypeEnum.hasName(label_)) {
        type_= static_cast<rw_access_type>(metaAccessTypeEnum.value(label_));
        return true;
      }
      return false;
    }

    bool is_valid(const rw_access_type t_)
    {
      return t_ != RW_INVALID;
    }

    bool is_readable(const rw_access_type t_)
    {
      return t_ == RW_READABLE || t_ == RW_READABLE_WRITABLE;
    }

    bool is_writable(const rw_access_type t_)
    {
      return t_ == RW_WRITABLE || t_ == RW_READABLE_WRITABLE;
    }

  } // namespace utility

} // namespace vire
