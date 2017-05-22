/// \file vire/device/space-reflect.h

#ifndef VIRE_DEVICE_SPACE_REFLECT_H
#define VIRE_DEVICE_SPACE_REFLECT_H

// Ourselves:
#include <vire/device/space.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// Export the introspection code for the 'vire::device::space::dimension_type' enum:
DR_CLASS_EXPORT(::vire::device::space, "vire::device::space")
DR_TYPE_EXPORT(::vire::device::space::dimension_type,
               "vire::device::space::dimension_type")

// Mandatory namespace is datatools::detail::reflection :
namespace datatools {
  namespace detail {
    namespace reflection {

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::device::space, /*tag_*/)
      {
        DR_CLASS_DECLARE(::vire::device::space)
          ;
        return;
      }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::device::space::dimension_type, /*tag_*/)
      {
        DR_ENUM_DECLARE(::vire::device::space::dimension_type)
          .value("vire::device::space::DIM_INVALID", ::vire::device::space::DIM_INVALID)
          .value("vire::device::space::DIM_0",       ::vire::device::space::DIM_0)
          .value("vire::device::space::DIM_1",       ::vire::device::space::DIM_1)
          .value("vire::device::space::DIM_2",       ::vire::device::space::DIM_2)
          .value("vire::device::space::DIM_3",       ::vire::device::space::DIM_3)
          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_DEVICE_SPACE_REFLECT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
