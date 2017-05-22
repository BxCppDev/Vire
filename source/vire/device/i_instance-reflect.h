/// \file vire/device/i_instance-reflect.h

#ifndef VIRE_DEVICE_I_INSTANCE_REFLECT_H
#define VIRE_DEVICE_I_INSTANCE_REFLECT_H

// Ourselve:
#include <vire/device/i_instance.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// This project:
#include <vire/device/single_instance.h>

// Export the introspection code for the 'vire::device::i_instance' class:
DR_CLASS_EXPORT(::vire::device::i_instance, "vire::device::i_instance")

// Mandatory namespace is datatools::detail::reflection :
namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::device::i_instance, /*tag_*/)
      {
        // Declare and populate the reflection metaclass for the 'vire::device::i_instance' class:
        DR_CLASS_DECLARE(::vire::device::i_instance)
          // .DR_CLASS_BASE(::datatools::i_serializable)
          .DR_CLASS_BASE(::datatools::i_tree_dumpable)
          .DR_CLASS_METHOD_CONST("is_valid",
                                 ::vire::device::i_instance,
                                 ::vire::device::i_instance::is_valid,
                                 bool)
          .DR_CLASS_VOID_METHOD("reset",
                                ::vire::device::i_instance,
                                ::vire::device::i_instance::reset
                                )
          .DR_CLASS_METHOD_CONST("get_dimension",
                                 ::vire::device::i_instance,
                                 ::vire::device::i_instance::get_dimension,
                                 size_t)
          .DR_CLASS_METHOD_CONST("get_number_of_items",
                                 ::vire::device::i_instance,
                                 ::vire::device::i_instance::get_number_of_items,
                                 size_t)
          .DR_CLASS_METHOD_CONST("is_multiple",
                                 ::vire::device::i_instance,
                                 ::vire::device::i_instance::is_multiple,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_replica",
                                 ::vire::device::i_instance,
                                 ::vire::device::i_instance::is_replica,
                                 bool)
          .DR_CLASS_VOID_METHOD2_CONST("get_instance",
                                       ::vire::device::i_instance,
                                       ::vire::device::i_instance::get_instance,
                                       int,
                                       ::vire::device::single_instance
                                       )

          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_DEVICE_I_INSTANCE_REFLECT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
