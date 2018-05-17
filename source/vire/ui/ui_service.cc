// ui_service.cc

// Ourselves:
#include <vire/ui/ui_service.h>

namespace vire {

  namespace ui {

    /** Auto-registration of this service class in a central service Db */
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(ui_service, "vire::ui::ui_service")

    ui_service::ui_service()
    {
      return;
    }

    ui_service::~ui_service()
    {
      if (this->is_initialized()) this->ui_service::reset();
      return;
    }

    bool ui_service::is_initialized() const
    {
      return _initialized_;
    }

    int ui_service::initialize(const datatools::properties & config_,
                               datatools::service_dict_type & services_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Service is already initialized!");
      base_service::_common_initialize(config_);
      _init_(config_);
      _initialized_ = true;
      return datatools::SUCCESS;
    }

    int ui_service::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Service is not initialized!");
      _initialized_ = false;
      _reset_();
      return datatools::SUCCESS;
    }

    void ui_service::_init_(const datatools::properties & config_)
    {
      if (config_.has_key("interactive")) {
        bool inter = config_.fetch_boolean("interactive");
        set_interactive(inter);
      }

      return;
    }

    void ui_service::_reset_()
    {
      _interactive_ = false;
      return;
    }

    void ui_service::set_interactive(bool i_)
    {
      _interactive_ = i_;
      return;
    }

    bool ui_service::is_interactive() const
    {
      return _interactive_;
    }

    bool ui_service::is_batch() const
    {
      return !_interactive_;
    }

    void ui_service::print_tree(std::ostream & out_,
                                const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      out_ << popts.indent
           << datatools::i_tree_dumpable::tag
           << "Interactive : "
           << std::boolalpha << _interactive_ << std::endl;

      out_ << popts.indent
           << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Initialized : "
           << std::boolalpha << _initialized_ << std::endl;

      return;
    }

  } // namespace ui

} // namespace vire
