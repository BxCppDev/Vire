/// \file vire/device/single_instance-reflect.h

#ifndef VIRE_DEVICE_SINGLE_INSTANCE_REFLECT_H
#define VIRE_DEVICE_SINGLE_INSTANCE_REFLECT_H

// Ourselves:
#include <vire/device/single_instance.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// Export the introspection code for the 'vire::device::single_instance' class:
DR_CLASS_EXPORT(::vire::device::single_instance, "vire::device::single_instance")

// Mandatory namespace is datatools::detail::reflection :
namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::device::single_instance, /*tag_*/)
        {
          // Declare and populate the reflection metaclass for the 'vire::device::single_instance' class:
          DR_CLASS_DECLARE(::vire::device::single_instance)
            .DR_CLASS_BASE(::vire::device::i_instance)
            .DR_CLASS_CONSTRUCTOR()
            .DR_CLASS_PROPERTY_GETTER_SETTER("slot",
                                             ::vire::device::single_instance::get_slot,
                                             ::vire::device::single_instance::set_slot)
            .DR_CLASS_METHOD_CONST("has_slot",
                                   ::vire::device::single_instance::has_slot,
                                   bool)
            .DR_CLASS_VOID_METHOD1("set_slot",
                                   ::vire::device::single_instance::set_slot,
                                   const slot & slot_)
            .DR_CLASS_VOID_METHOD("reset_slot",
                                  ::vire::device::single_instance::reset_slot)
            // ...
            ;
          return;
        }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_DEVICE_SINGLE_INSTANCE_REFLECT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
