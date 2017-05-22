// vire/message/body_layout.cc
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

// Ourselves:
#include <vire/message/body_layout.h>

// Standard library:
#include <memory>

namespace vire {

  namespace message {

    // static
    const std::string & body_layout::name()
    {
      static const std::string _bfn("vire::message::body_format::typed_payload");
      return _bfn;
    }

    // static
    const datatools::version_id & body_layout::current_version()
    {
      static std::unique_ptr<datatools::version_id> _vid;
      if (_vid.get() == nullptr) {
        _vid.reset(new datatools::version_id(1,0));
      }
      return *_vid.get();
    }

  } // namespace message

} // namespace vire
