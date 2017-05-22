// vire/utility/json_converters.cc
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
#include <vire/utility/json_converters.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
// - BxJsontools:
#include <bayeux/jsontools/exception.h>

namespace jsontools {

  void converter<datatools::version_id>::jsonize(node & node_,
                                                 datatools::version_id & vid_)
  {
    std::string tstr = vid_.to_string();
    tstr = vid_.to_string();
    node_.grab_value() = tstr;
    return;
  }

  void converter<datatools::version_id>::dejsonize(node & node_,
                                                   datatools::version_id & vid_)
  {
    if (not node_.get_value().isString()) {
      throw jsontools::wrong_type(node_.get_value(), "expected string");
    }
    std::string tstr = node_.get_value().asString();
    bool parsed = vid_.from_string(tstr);
    DT_THROW_IF(!parsed, std::logic_error,
                "Cannot parse a JSON-ized 'datatools::version_id' object!");
    return;
  }

} // end of namespace jsontools
