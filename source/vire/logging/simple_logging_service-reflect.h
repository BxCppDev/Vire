//! \file vire/logging/simple_logging_service-reflection.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/logging/simple_logging_service.h>

// This project:
#include <datatools/compiler_macros.h>

// This project:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// Export the introspection code for the 'datatools::base_service' class:
DR_CLASS_EXPORT(::vire::logging::simple_logging_service, "vire::logging::simple_logging_service")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::logging::simple_logging_service, DT_UNUSED(tag_) )
      {
        // Declare and populate the reflection metaclass :
        DR_CLASS_DECLARE(::vire::logging::simple_logging_service)
          .DR_CLASS_BASE(::datatools::base_service)
          .DR_CLASS_VOID_METHOD1("set_severity",
                                 ::vire::logging::simple_logging_service,
                                 ::vire::logging::simple_logging_service::set_severity_label,
                                 const std::string &)
          .DR_CLASS_METHOD_CONST("get_severity",
                                 ::vire::logging::simple_logging_service,
                                 ::vire::logging::simple_logging_service::get_severity_label,
                                 std::string)
          .DR_CLASS_METHOD_CONST("is_trace",
                                 ::vire::logging::simple_logging_service,
                                 ::vire::logging::simple_logging_service::is_trace,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_debug",
                                 ::vire::logging::simple_logging_service,
                                 ::vire::logging::simple_logging_service::is_debug,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_info",
                                 ::vire::logging::simple_logging_service,
                                 ::vire::logging::simple_logging_service::is_info,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_warning",
                                 ::vire::logging::simple_logging_service,
                                 ::vire::logging::simple_logging_service::is_warning,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_error",
                                 ::vire::logging::simple_logging_service,
                                 ::vire::logging::simple_logging_service::is_error,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_fatal",
                                 ::vire::logging::simple_logging_service,
                                 ::vire::logging::simple_logging_service::is_fatal,
                                 bool)
          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools
