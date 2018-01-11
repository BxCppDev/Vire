// vire/user/general_expression_resource_selector.cc - Implementation of a selector of resources built on some general expression
//
// Copyright (c) 2015 by Francois Mauger <mauger@lpccaen.in2p3.fr>
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves
#include <vire/resource/general_expression_resource_selector.h>

// Third party:
// - Boost:
// #include <boost/general_expression.hpp>

// This project:
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>
#include <vire/resource/detail/gers_tools.h>

namespace vire {

  namespace resource {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(general_expression_resource_selector, "vire::resource::general_expression_resource_selector");

    struct general_expression_resource_selector::work
    {
      cuts::cut_handle_type top;
    };

    general_expression_resource_selector::general_expression_resource_selector(datatools::logger::priority logger_priority_)
      : base_resource_selector(logger_priority_)
    {
      return;
    }

    general_expression_resource_selector::~general_expression_resource_selector()
    {
      if (is_initialized()) {
        this->general_expression_resource_selector::reset();
      }
      return;
    }

    int general_expression_resource_selector::_accept()
    {
      int result = cuts::SELECTION_ACCEPTED;
      DT_LOG_TRACE(_logging, "Entering...");
      if (is_user_data<std::string>()) {
        const std::string & rpath = get_user_data<std::string>();
        if (!has_path(rpath)) {
          result = cuts::SELECTION_REJECTED;
        }
      } else if (is_user_data<resource>()) {
        const resource & r = get_user_data<resource>();
        if (!has_path(r.get_path())) {
          result = cuts::SELECTION_REJECTED;
        }
      }
      DT_LOG_TRACE(_logging, "Exiting.");
      return result;
    }

    void general_expression_resource_selector::initialize(const datatools::properties & config_,
                                                          datatools::service_manager & /* service_manager_ */,
                                                          cuts::cut_handle_dict_type & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource selector '" << get_name() << "' is already initialized !");

      this->base_resource_selector::_base_init(config_);

      if (!has_resource_mgr()) {
        /*
        // Not implemented:
        vire::resource_service * resourcev_ptr = 0;
        if (service_manager_.is_a<vire::resource::service>("Resources")) {
        vire::resource::service & resourcev = service_manager_.grab<vire::resource::service>("Resources");
        set_resource_mgr(resourcev->get_resource_manager());
        }
        */
      }

      // if (has_resource_mgr()) {
      //   if (_rids_.empty()) {
      //     if (config_.has_key("resources.general_expression")) {
      //       std::string general_expressionp;
      //       config_.fetch("resources.general_expression", general_expressionp);
      //       _resource_mgr_->build_set_of_rids_from_path_general_expressionp(general_expressionp, _rids_);
      //     }
      //   }
      // }

      if (has_resource_mgr()) {
        if (config_.has_key("expression")) {
          std::string expression = config_.fetch_string("expression");
          set_expression(expression);
        }
      }

      // Initialize internal data:
      _work_.reset(new work);
      // _work_->hgeneral_expression.reset(new boost::general_expression(_general_expression_paths_));

      _init();

      _set_initialized(true);
      return;
    }

    void general_expression_resource_selector::reset()
    {
      _set_initialized(false);
      _work_->top.reset();
      _work_.reset();
      _expression_.clear();
      this->base_resource_selector::_base_reset();
      return;
    }

    void general_expression_resource_selector::_init()
    {
      detail::general_expression_parsing::expression_ast ast;

      {
        datatools::command::returned_info rinfo = detail::general_expression_parsing::parse(_expression_, ast);
        DT_THROW_IF(rinfo.is_failure(), std::logic_error,
                    "Invalid expression! " << rinfo.get_error_message());
      }

      {
        datatools::command::returned_info rinfo_built = detail::general_expression_parsing::build_cut(ast, get_resource_mgr(), _work_->top);
        DT_THROW_IF(rinfo_built.is_failure(), std::logic_error,
                    "Building of the top cut handle failed! " << rinfo_built.get_error_message());
      }

      return;
    }

    bool general_expression_resource_selector::has_path(const std::string & rpath_) const
    {
      if (!_work_->top.has_data()) return false;
      _work_->top.grab().set_user_data(rpath_);
      return _work_->top.grab().process() == cuts::SELECTION_ACCEPTED;
    }

    bool general_expression_resource_selector::has_expression() const
    {
      return ! _expression_.empty();
    }

    void general_expression_resource_selector::set_expression(const std::string & general_expression_)
    {
      _expression_= general_expression_;
      return;
    }

    const std::string & general_expression_resource_selector::get_expression() const
    {
      return _expression_;
    }

    void general_expression_resource_selector::export_to_config(datatools::properties & config_,
                                                                uint32_t flags_,
                                                                const std::string & prefix_) const
    {
      this->cuts::i_cut::export_to_config(config_, flags_, prefix_);

      config_.store(prefix_ + "expression", _expression_, "The general expression on resource");

      return;
    }

    // virtual
    void general_expression_resource_selector::tree_dump(std::ostream & out_,
                                                         const std::string & title_,
                                                         const std::string & indent_,
                                                         bool inherit_) const
    {
      this->base_resource_selector::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "General expression : ";
      if (has_expression()) {
        out_ << "'" << _expression_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Top selector : ";
      if (_work_.get() == 0 || ! _work_->top.has_data()) {
        out_ << "<none>";
      } else {
        out_ << "'" << _work_->top.get().get_name() << "'";
      }
      out_ << std::endl;

      return;
    }

  } // namespace resource

} // namespace vire
