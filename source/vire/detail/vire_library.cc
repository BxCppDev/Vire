// vire/detail/vire_library.cc - Implementation of Vire system singleton
//

// Ourselves:
#include <vire/detail/vire_library.h>

// Standard library
#include <cstdlib>
#include <memory>
#include <string>

// Third party:
// Boost:
#include <boost/filesystem.hpp>
// Bayeux:
#include <bayeux/datatools/kernel.h>
#include <bayeux/datatools/library_info.h>
#include <bayeux/datatools/urn_to_path_resolver_service.h>
#include <bayeux/datatools/urn_db_service.h>

// This project;
#include "vire/version.h"
#include "vire/resource_files.h"
#include "vire_binreloc.h"

namespace {
  //! Convert BrInitError code to a string describing the error
  //! \todo add errno to returned string
  std::string BRErrorAsString(const BrInitError& err) {
    std::string errMsg;
    switch (err) {
    case BR_INIT_ERROR_NOMEM:
      errMsg = "Unable to open /proc/self/maps";
    case BR_INIT_ERROR_OPEN_MAPS:
      errMsg = "Unable to read from /proc/self/maps";
    case BR_INIT_ERROR_READ_MAPS:
      errMsg = "The file format of /proc/self/maps";
    case BR_INIT_ERROR_INVALID_MAPS:
      errMsg = "The file format of /proc/self/maps is invalid";
    case BR_INIT_ERROR_DISABLED:
      errMsg = "Binary relocation disabled";
    default:
      BOOST_ASSERT_MSG(1,"Invalid BrInitError");
    }
    return errMsg;
  }
}

namespace vire {

  namespace detail {

    directories_initialization_exception::directories_initialization_exception(const std::string& msg)
      : std::runtime_error(msg)
    {
    }

    void init_directories() {
      BrInitError err;
      int initSuccessful = br_init_lib(&err);
      DT_THROW_IF(initSuccessful != 1,
                  vire::detail::directories_initialization_exception,
                  "Initialization of Vire library's directories failed : "
                  << err
                  << " ("
                  << BRErrorAsString(err)
                  << ")");
    }

    // static
    const std::string & vire_library::setup_db_name()
    {
      static const std::string _n("vireSystemSetupDb");
      return _n;
    }

    // static
    const std::string & vire_library::resource_resolver_name()
    {
      static const std::string _n("vireSystemResourceResolver");
      return _n;
    }

    datatools::logger::priority vire_library::process_logging_env()
    {
      datatools::logger::priority logging = datatools::logger::PRIO_FATAL;
      char * l = getenv("VIRE_LIBRARY_LOGGING");
      if (l) {
        std::string level_label(l);
        ::datatools::logger::priority prio
            = ::datatools::logger::get_priority(level_label);
        if (prio != ::datatools::logger::PRIO_UNDEFINED) {
          logging = prio;
        }
      }
      return logging;
    }

    // static
    vire_library * vire_library::_instance_ = nullptr;

    vire_library::vire_library()
    {
      _logging_ = vire_library::process_logging_env();
      if (_logging_ == ::datatools::logger::PRIO_UNDEFINED) {
        DT_LOG_WARNING(::datatools::logger::PRIO_WARNING,
                       "Ignoring invalid VIRE_LIBRARY_LOGGING=\"" << getenv("VIRE_LIBRARY_LOGGING") << "\" environment!");
        _logging_ = ::datatools::logger::PRIO_FATAL;
      }
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_LOG_TRACE(_logging_, "Initializing Vire library's resource files directories...");
      vire::detail::init_directories();

      DT_THROW_IF(vire_library::_instance_ != nullptr,
                  std::logic_error,
                  "Vire library system singleton is already set!");
      vire_library::_instance_ = this;
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    vire_library::~vire_library()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      if (is_initialized()) {
        DT_LOG_TRACE(_logging_, "Shuting down Vire library system singleton...");
        shutdown();
      }
      vire_library::_instance_ = nullptr;
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    datatools::logger::priority vire_library::get_logging() const
    {
      return _logging_;
    }

    bool vire_library::is_initialized() const
    {
      return _initialized_;
    }

    void vire_library::initialize()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(is_initialized(), std::logic_error, "Vire library system singleton is already initialized!");

      // Register library informations in the Bayeux/datatools' kernel:
      _libinfo_registration_();

      // Setup services:
      DT_LOG_TRACE(_logging_, "Vire library system singleton services...");
      _services_.set_name("vireLibrarySystemServices");
      _services_.set_description("Vire Library System Singleton Services");
      _services_.set_allow_dynamic_services(true);
      _services_.initialize();

      // Start URN services:
      _initialize_urn_services_();

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void vire_library::shutdown()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(!is_initialized(), std::logic_error, "Vire library system singleton is not initialized!");
      _initialized_ = false;

      // Terminate services:
      if (_services_.is_initialized()) {
        _shutdown_urn_services_();
        DT_LOG_TRACE(_logging_, "Terminating Vire library system singleton services...");
        _services_.reset();
      }

      // Deregister library informations from the Bayeux/datatools' kernel:
      _libinfo_deregistration_();

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    datatools::service_manager & vire_library::grab_services()
    {
      return _services_;
    }

    const datatools::service_manager & vire_library::get_services() const
    {
      return _services_;
    }

    // static
    bool vire_library::is_instantiated()
    {
      return _instance_ != nullptr;
    }

    // static
    vire_library & vire_library::instance()
    {
      return *_instance_;
    }

    // static
    const vire_library & vire_library::const_instance()
    {
      return *_instance_;
    }

    // static
    vire_library & vire_library::instantiate()
    {
      if (! vire_library::is_instantiated()) {
        static std::unique_ptr<vire_library> _flsys_handler;
        if (! _flsys_handler) {
          // Allocate the Vire sys library singleton and initialize it:
          _flsys_handler.reset(new vire_library);
        }
      }
      return vire_library::instance();
    }

    void vire_library::_libinfo_registration_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      DT_THROW_IF(!datatools::kernel::is_instantiated(),
                  std::runtime_error,
                  "The Bayeux/datatools' kernel is not instantiated !");
      datatools::kernel & krnl = datatools::kernel::instance();

      // Populate the library info register, basically dumb if we don't
      // have it so assume it exists and hope for an exception if
      // it doesn't
      datatools::library_info & lib_info_reg = krnl.grab_library_info_register();

      // Bundled submodules:
      {
        DT_LOG_TRACE(_logging_, "Registration of Vire library in the Bayeux/datatools' kernel...");
        // Vire itself:
        DT_THROW_IF (lib_info_reg.has("vire"),
                     std::logic_error,
                     "Vire is already registered !");
        datatools::properties & vire_lib_infos
          = lib_info_reg.registration("vire",
                                      "Vire provides a offline and real-time software environment for the description, control,"
                                      "monitoring and running of an experiment.",
                                      vire::version::get_version()
                                      );
        DT_LOG_TRACE(_logging_, "Vire library entry is now registered in the Bayeux/datatools' kernel.");


        // Register the Vire resource path in the datatools' kernel:
        vire_lib_infos.store_string(datatools::library_info::keys::install_resource_dir(),
                                    vire::get_resource_files_dir());
        DT_LOG_TRACE(_logging_, "Vire resource files dir is documented.");

        // If the 'VIRE_RESOURCE_DIR' environment variable is set, it will supersede
        // the official registered resource path above through the 'datatools::fetch_path_with_env'
        // function:
        vire_lib_infos.store_string(datatools::library_info::keys::env_resource_dir(),
                                    "VIRE_RESOURCE_FILES_DIR");
        DT_LOG_TRACE(_logging_, "Vire resource files dir env is documented.");

#if VIRE_WITH_PLUGINS == 1
        // Register the Vire plugin path in the datatools' kernel:
        vire_lib_infos.store_string(datatools::library_info::keys::install_plugin_lib_dir(),
                                    vire::get_plugin_dir());
        DT_LOG_TRACE(_logging_, "Vire plugin resource files dir is documented.");

        // If the 'VIRE_PLUGIN_LIB_DIR' environment variable is set, it will supersed
        // the official registered plugin path above through the 'datatools::library_loader'
        // function:
        vire_lib_infos.store_string(datatools::library_info::keys::env_plugin_lib_dir(),
                                    "VIRE_PLUGIN_LIB_DIR"
                                    );
        DT_LOG_TRACE(_logging_, "Vire plugin resource files dir env is documented.");
#endif // VIRE_WITH_PLUGINS == 1

        DT_LOG_TRACE(_logging_, "Vire library has been registered in the Bayeux/datatools' kernel.");
      }

      // SuperNEMO Sandbox:
      {
        DT_LOG_TRACE(_logging_, "Registration of Vire SuperNEMO sandbox library in the Bayeux/datatools' kernel...");
        datatools::properties & vire_snemo_lib_infos
          = lib_info_reg.registration("snemo",
                                      "Vire SuperNEMO sandbox",
                                      vire::version::get_version()
                                      );

        // Register the Vire resource path in the datatools' kernel:
        boost::filesystem::path p = vire::get_resource_files_dir();
        p /= "sandbox";
        p /= "snemo";
        vire_snemo_lib_infos.store_string(datatools::library_info::keys::install_resource_dir(),
                                            p.string());
        DT_LOG_TRACE(_logging_, "Vire SuperNEMO sandbox resource files dir is documented.");
        DT_LOG_TRACE(_logging_, "Vire SuperNEMO sandbox library entry is now registered in the Bayeux/datatools' kernel.");
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void vire_library::_libinfo_deregistration_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      if (datatools::kernel::is_instantiated()) {
        datatools::kernel & krnl = datatools::kernel::instance();
        if (krnl.has_library_info_register()) {
          // Access to the datatools kernel library info register:
          datatools::library_info & lib_info_reg = krnl.grab_library_info_register();

          // Unregistration of all registered submodules from the kernel's
          // library info register:
          if (lib_info_reg.has("snemo")) {
            DT_LOG_TRACE(_logging_, "Deregistration of the Vire SuperNEMO sandbox library from the Bayeux/datatools' kernel...");
            lib_info_reg.unregistration("snemo");
            DT_LOG_TRACE(_logging_, "Vire SuperNEMO sandbox library has been deregistered from the Bayeux/datatools' kernel.");
          }

          if (lib_info_reg.has("vire")) {
            DT_LOG_TRACE(_logging_, "Deregistration of the Vire library from the Bayeux/datatools' kernel...");
            lib_info_reg.unregistration("vire");
            DT_LOG_TRACE(_logging_, "Vire library has been deregistered from the Bayeux/datatools' kernel.");
          }
        }
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void vire_library::_initialize_urn_services_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      if (_services_.is_initialized()) {

        // Activate an URN info DB service:
        {
          datatools::urn_db_service & urnSetupDb =
            dynamic_cast<datatools::urn_db_service &>(_services_.load_no_init("vireSandboxSNemoSystemDb",
                                                                              "datatools::urn_db_service"));
          urnSetupDb.set_logging_priority(_logging_);
          std::string urn_db_conf_file = "@snemo:urn/db/service.conf";
          datatools::fetch_path_with_env(urn_db_conf_file);
          datatools::properties urn_db_conf;
          urn_db_conf.read_configuration(urn_db_conf_file);
          urnSetupDb.initialize_standalone(urn_db_conf);
          if (datatools::logger::is_debug(_logging_)) {
            urnSetupDb.tree_dump(std::cerr, urnSetupDb.get_name() + ": ", "[debug] ");
          }
          DT_LOG_TRACE(_logging_, "Publishing the URN info DB '" << urnSetupDb.get_name() << "' to the Bayeux/datatools' kernel...");
          urnSetupDb.kernel_push();
          DT_LOG_TRACE(_logging_, "URN info DB has been plugged in the Bayeux/datatools' kernel.");
        }

        // Activate an URN resolver service:
        {
          datatools::urn_to_path_resolver_service & urnResourceResolver =
            dynamic_cast<datatools::urn_to_path_resolver_service &>(_services_.load_no_init("vireSandboxSNemoSystemPathResolver",
                                                                                            "datatools::urn_to_path_resolver_service"));
          urnResourceResolver.set_logging_priority(_logging_);
          std::string urn_resolver_conf_file = "@snemo:urn/resolvers/service.conf";
          datatools::fetch_path_with_env(urn_resolver_conf_file);
          datatools::properties urn_resolver_conf;
          urn_resolver_conf.read_configuration(urn_resolver_conf_file);
          urnResourceResolver.initialize_standalone(urn_resolver_conf);
          if (datatools::logger::is_debug(_logging_)) {
            urnResourceResolver.tree_dump(std::cerr, urnResourceResolver.get_name() + ": ", "[debug] ");
          }
          DT_LOG_TRACE(_logging_, "Publishing the URN path resolver '" << urnResourceResolver.get_name() << "' to the Bayeux/datatools' kernel...");
          urnResourceResolver.kernel_push();
          DT_LOG_TRACE(_logging_, "URN path resolver has been plugged in the Bayeux/datatools' kernel.");
        }
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void vire_library::_shutdown_urn_services_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      if (_services_.is_initialized()) {

        // Deactivate the URN resolver:
        if (_services_.has(resource_resolver_name())) {
          DT_LOG_TRACE(_logging_, "Accessing URN path resolver...");
          datatools::urn_to_path_resolver_service & urnResourceResolver =
            _services_.grab<datatools::urn_to_path_resolver_service &>("vireSandboxSNemoSystemPathResolver");
          DT_LOG_TRACE(_logging_, "Removing URN path resolver '" << urnResourceResolver.get_name() << "' from the  Bayeux/datatools's kernel...");
          urnResourceResolver.kernel_pop();
          DT_LOG_TRACE(_logging_, "URN path resolver has been removed from the  Bayeux/datatools kernel.");
          urnResourceResolver.reset();
        }

        // DeActivate the URN info setup DB:
        if (_services_.has(setup_db_name())){
          DT_LOG_TRACE(_logging_, "Accessing URN info setup DB...");
          datatools::urn_db_service & urnSetupDb =
            _services_.grab<datatools::urn_db_service &>("vireSandboxSNemoSystemDb");
          DT_LOG_TRACE(_logging_, "Removing URN info setup DB '" << urnSetupDb.get_name() << "' from the  Bayeux/datatools's kernel...");
          urnSetupDb.kernel_pop();
          DT_LOG_TRACE(_logging_, "URN info setup DB has been removed from the  Bayeux/datatools kernel.");
          urnSetupDb.reset();
        }
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

  } // end of namespace detail

} // namespace vire
