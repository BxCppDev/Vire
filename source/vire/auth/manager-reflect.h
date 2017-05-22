
// Ourselves:
#include <vire/auth/manager.h>

// This project:
#include <datatools/compiler_macros.h>

// This project:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// Export the introspection code for the 'datatools::base_service' class:
DR_CLASS_EXPORT(::vire::auth::manager, "vire::auth::manager")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::auth::manager, DT_UNUSED(tag_) )
        {
          // Declare and populate the reflection metaclass :
          DR_CLASS_DECLARE(::vire::auth::manager)
            .DR_CLASS_BASE(::datatools::base_service)

            .DR_CLASS_METHOD_CONST("is_initialized",
                                   ::vire::auth::manager,
                                   ::vire::auth::manager::is_initialized,
                                   bool)
            .DR_CLASS_TAG_FLAG("builder")
            .DR_CLASS_TAG_FLAG("const")
            .DR_CLASS_TAG("description", "Check the initialization status")
            .DR_CLASS_TAG("returned.name", "initialized")
            .DR_CLASS_TAG("returned.description", "the initialization flag")

            .DR_CLASS_METHOD("reset",
                             ::vire::auth::manager,
                             ::vire::auth::manager::reset,
                             int)
            .DR_CLASS_TAG_FLAG("builder")
            .DR_CLASS_TAG("description", "Reset the authentication manager")
            .DR_CLASS_TAG("returned.name", "error_code")
            .DR_CLASS_TAG("returned.description", "The status of the operation")

            .DR_CLASS_METHOD2_CONST("authenticate",
                                    ::vire::auth::manager,
                                    ::vire::auth::manager::authenticate,
                                    ::vire::auth::manager::auth_status,
                                    std::string,
                                    std::string)
            .DR_CLASS_TAG_FLAG("builder")
            .DR_CLASS_TAG("description", "Check authentication for a given user/password pair")
            .DR_CLASS_TAG_FLAG("const")
            .DR_CLASS_TAG("arg[0].name", "login")
            .DR_CLASS_TAG("arg[0].description", "The user's login name")
            .DR_CLASS_TAG("arg[1].name", "password")
            .DR_CLASS_TAG("arg[1].description", "The user's current password")
            .DR_CLASS_TAG("returned.name", "status")
            .DR_CLASS_TAG("returned.type", "vire::auth::manager::auth_status")
            .DR_CLASS_TAG("returned.description", "The authentication status")

            ;
          return;
        }

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::auth::manager::auth_status, DT_UNUSED(tag_))
        {
          DR_ENUM_DECLARE(::vire::auth::manager::auth_status)
            .value("success",          ::vire::auth::manager::AUTH_SUCCESS)
            .value("invalid_login",    ::vire::auth::manager::AUTH_INVALID_LOGIN)
            .value("locked",           ::vire::auth::manager::AUTH_LOCKED)
            .value("expired",          ::vire::auth::manager::AUTH_EXPIRED)
            .value("invalid_password", ::vire::auth::manager::AUTH_INVALID_PASSWORD)
            ;
          return;
        }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
