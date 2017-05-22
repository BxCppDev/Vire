/// \file vire/device/slot-reflect.h

#ifndef VIRE_DEVICE_SLOT_REFLECT_H
#define VIRE_DEVICE_SLOT_REFLECT_H

// Ourselves:
#include <vire/device/slot.h>

// Third party:
// - Camp:
#include <camp/arraymapper.hpp>
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// Export the introspection code for the 'vire::device::slot' class:
DR_CLASS_EXPORT(::vire::device::slot, "vire::device::slot")

// Mandatory namespace is datatools::detail::reflection :
namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::device::slot, /*tag_*/)
      {
        // Declare and populate the reflection metaclass for the 'vire::device::slot' class:
        DR_CLASS_DECLARE (::vire::device::slot)
          .DR_CLASS_BASE(::datatools::i_tree_dumpable)
          .DR_CLASS_CONSTRUCTOR()
          .DR_CLASS_CONSTRUCTOR4(::vire::device::space::dimension_type,
                                 uint32_t, uint32_t, uint32_t
                                 )
          .DR_CLASS_PROPERTY_GETTER_SETTER("dimension",
                                           ::vire::device::slot::get_dimension,
                                           ::vire::device::slot::set_dimension)
          .DR_CLASS_PROPERTY_GETTER_SETTER("dimension",
                                           ::vire::device::slot::get_dimension,
                                           ::vire::device::slot::set_dimension)
          .DR_CLASS_METHOD_CONST("has_dimension",
                                 ::vire::device::slot,
                                 ::vire::device::slot::has_dimension,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_dimensionless",
                                 ::vire::device::slot,
                                 ::vire::device::slot::is_dimensionless,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_dimension_0",
                                 ::vire::device::slot,
                                 ::vire::device::slot::is_dimension_0,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_dimension_1",
                                 ::vire::device::slot,
                                 ::vire::device::slot::is_dimension_1,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_dimension_2",
                                 ::vire::device::slot,
                                 ::vire::device::slot::is_dimension_2,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_dimension_3",
                                 ::vire::device::slot,
                                 ::vire::device::slot::is_dimension_3,
                                 bool)
          .DR_CLASS_METHOD_CONST("get_max_coordinate_index",
                                 ::vire::device::slot,
                                 ::vire::device::slot::get_max_coordinate_index,
                                 int32_t)
          .DR_CLASS_METHOD1_CONST("has_valid_coordinate",
                                  ::vire::device::slot,
                                  ::vire::device::slot::has_valid_coordinate,
                                  bool,
                                  unsigned int)
          .DR_CLASS_METHOD1_CONST("get_coordinate",
                                  ::vire::device::slot,
                                  ::vire::device::slot::get_coordinate,
                                  uint32_t,
                                  unsigned int)
          .DR_CLASS_VOID_METHOD2("set_coordinate",
                                 ::vire::device::slot,
                                 ::vire::device::slot::set_coordinate,
                                 unsigned int,
                                 uint32_t)
          .DR_CLASS_VOID_METHOD("reset_coordinates",
                                 ::vire::device::slot,
                                 ::vire::device::slot::reset_coordinates)
          // None of the following works:
          // .DR_CLASS_METHOD_CONST("gc",
          //                        ::vire::device::slot,
          //                        ::vire::device::slot::gc,
          //                        const std::vector<uint32_t> &)
          // .DR_CLASS_METHOD_CONST("gc",
          //                        ::vire::device::slot,
          //                        ::vire::device::slot::gc,
          //                        ::vire::device::slot::coordinates_array)
          // .DR_CLASS_METHOD_CONST("get_coordinates",
          //                       ::vire::device::slot,
          //                       ::vire::device::slot::get_coordinates,
          //                       std::vector<uint32_t>)
          // .DR_CLASS_VOID_METHOD2_CONST("fetch_coordinates",
          //                              ::vire::device::slot,
          //                              ::vire::device::slot::fetch_coordinates,
          //                              std::vector<uint32_t> &,
          //                              bool)
          .DR_CLASS_VOID_METHOD4("set",
                                 ::vire::device::slot,
                                 ::vire::device::slot::set,
                                 ::vire::device::space::dimension_type,
                                 uint32_t,
                                 uint32_t,
                                 uint32_t
                                 )
          .DR_CLASS_METHOD_CONST("is_valid",
                                 ::vire::device::slot,
                                 ::vire::device::slot::is_valid,
                                 bool)
          .DR_CLASS_VOID_METHOD("reset",
                                ::vire::device::slot,
                                ::vire::device::slot::reset)
          .DR_CLASS_VOID_METHOD1("make_dimension_0",
                                 ::vire::device::slot,
                                 ::vire::device::slot::make_dimension_0,
                                 bool)
          .DR_CLASS_VOID_METHOD1("make_dimension_1",
                                 ::vire::device::slot,
                                 ::vire::device::slot::make_dimension_1,
                                 uint32_t)
          .DR_CLASS_VOID_METHOD2("make_dimension_2",
                                 ::vire::device::slot,
                                 ::vire::device::slot::make_dimension_2,
                                 uint32_t,
                                 uint32_t)
          .DR_CLASS_VOID_METHOD3("make_dimension_3",
                                 ::vire::device::slot,
                                 ::vire::device::slot::make_dimension_3,
                                 uint32_t,
                                 uint32_t,
                                 uint32_t)
          .DR_CLASS_METHOD1("from_string",
                            ::vire::device::slot,
                            ::vire::device::slot::from_string,
                            bool,
                            std::string)
          .DR_CLASS_METHOD_CONST("str",
                                 ::vire::device::slot,
                                 ::vire::device::slot::str,
                                 std::string)
          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_DEVICE_SLOT_REFLECT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
