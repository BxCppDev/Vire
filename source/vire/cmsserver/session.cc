//! \file vire/cmsserver/session.cc
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/cmsserver/session.h>

// Standard Library:
#include <thread>
#include <future>
#include <memory>
#include <chrono>
#include <unistd.h>

// This project:
#include <vire/time/utils.h>
#include <vire/resource/role.h>
#include <vire/resource/manager.h>
#include <vire/cmsserver/utils.h>
#include <vire/cmsserver/base_use_case.h>
#include <vire/cmsserver/client_connection.h>
#include <vire/user/simple_password_generator.h>

/*
namespace
{

  class session_user_manager
  {
  public:

    session_user_manager()
    {
      _init_();
      return;
    }

    ~session_user_manager()
    {
      return;
    }

    std::pair<std::string,std::string> new_user_password()
    {
      std::pair<std::string,std::string> p;
      _login_gen_->generate_password(p.first, 8);
      _password_gen_->generate_password(p.second, 10);
      return p;
    }

  private:

    void _init_()
    {
      std::hash<std::thread::id> hasher;
      unsigned long i1 = hasher(std::this_thread::get_id());
      unsigned long i2 = hasherstatic_cast<unsigned long>(getpid());
      unsigned long i3 = std::chrono::system_clock::now().time_since_epoch().count();
      unsigned int seed_name = std::abs(i3 + i2 - i1);
      unsigned int seed_password = i3 + i2 + i1;
      _login_gen_.reset(new vire::user::simple_password_generator(vire::user::simple_password_generator::CHARSET_ALPHANUM,
                                                                  seed_name));
      _password_gen_.reset(new vire::user::simple_password_generator(vire::user::simple_password_generator::CHARSET_ALPHANUM,
                                                                     seed_password));
      return;
    }

    void _reset_()
    {
      _login_gen_.reset();
      _password_gen_.reset();
      return;
    }

    std::unique_ptr<vire::user::simple_password_generator> _login_gen_;
    std::unique_ptr<vire::user::simple_password_generator> _password_gen_;
  };

  session_user_manager & session_users()
  {
    static std::unique_ptr<session_user_manager> _sum;
    if (_sum.get() == nullptr) {
      _sum.reset(new session_user_manager);
    }
    return *_sum;
  }

}
*/

namespace vire {

  namespace cmsserver {

    // static
    const int32_t session::INVALID_ID;
    const int32_t session::ROOT_ID;

    session::session_entry::~session_entry()
    {
      if (instance != nullptr) {
        if (instance->is_initialized()) {
          // Other tests and ops may be needed...
          instance->terminate();
        }
        delete instance;
        instance = nullptr;
      }
      return;
    }

    session::session()
      : _when_(vire::time::invalid_time_interval())
    {
      return;
    }

    session::~session()
    {
      return;
    }

    void session::set_logging_priority(datatools::logger::priority lp_)
    {
      _logging_ = lp_;
      return;
    }

    datatools::logger::priority session::get_logging_priority() const
    {
      return _logging_;
    }

    bool session::has_id() const
    {
      return _id_ >= 0;
    }

    void session::set_id(const int32_t id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Session is initialized and locked!");
      _id_ = id_ < 0 ? INVALID_ID : id_;
      return;
    }

    int32_t session::get_id() const
    {
      return _id_;
    }

    bool session::has_parent() const
    {
      return _parent_ != nullptr;
    }

    void session::set_parent(const session & parent_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Session is initialized and locked!");
      _parent_ = &parent_;
      return;
    }

    void session::reset_parent()
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Session is initialized and locked!");
      _parent_ = nullptr;
      return;
    }

    const session & session::get_parent() const
    {
      DT_THROW_IF(has_parent(), std::logic_error, "No parent is set!");
      return *_parent_;
    }

    bool session::is_root() const
    {
      return ! has_parent();
    }

    bool session::has_use_case() const
    {
      return _use_case_ != nullptr;
    }

    void session::set_use_case(base_use_case & uc_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Session is initialized and locked!");

      // XXX Should we test here if the use case object if initialized ?
      // DT_THROW_IF(!uc_->is_initialized(), std::logic_error, "Use case is not initialized!");
      _use_case_ = &uc_;
      return;
    }

    const base_use_case & session::get_use_case() const
    {
      DT_THROW_IF(!has_use_case(), std::logic_error, "Use case is not set!");
      return *_use_case_;
    }

    base_use_case & session::grab_use_case()
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Session is initialized and locked!");
      DT_THROW_IF(!has_use_case(), std::logic_error, "Use case is not set!");
      return *_use_case_;
    }

    bool session::has_when() const
    {
      return vire::time::is_valid(_when_);
    }

    const boost::posix_time::time_period & session::get_when() const
    {
      return _when_;
    }

    void session::set_when(const boost::posix_time::time_period & when_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Session is initialized and locked!");
      _when_ = when_;
      return;
    }

    const resource_pool & session::get_functional() const
    {
      return _functional_;
    }

    resource_pool & session::grab_functional()
    {
      return _functional_;
    }

    void session::set_functional(const resource_pool & functional_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Session is initialized and locked!");
      _functional_ = functional_;
      return;
    }

    const resource_pool & session::get_distributable() const
    {
      return _distributable_;
    }

    resource_pool & session::grab_distributable()
    {
      return _distributable_;
    }

    void session::set_distributable(const resource_pool & distributable_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Session is initialized and locked!");
      _distributable_ = distributable_;
      return;
    }

    void session::_check_use_case_()
    {
      DT_THROW_IF(!has_use_case(), std::logic_error, "Session has no use case!");

      // Check resource and time constraints forced by the use case

      return;
    }

    // void session::_at_init_(const session_ptr_type & parent_,
    //                          const vire::resource::manager & rmgr_,
    //                          const session_info & sinfo_,
    //                          uint32_t flags_)
    // {
    //   DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());

    //   if (parent_) {
    //      _parent_ = parent_;
    //   }

    //   // Hosted use_case/process:
    //   _use_case_ = ;

    //   _initialize_pools_(rmgr_, sinfo_);

    //   // // Plug the session in the list of subsessions of its parent session:
    //   // if (has_parent()) {
    //   //   _parent_->_sys_add_subsession_(*this);
    //   // }

    //   DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
    //   return;
    // }

    void session::_destroy_use_case_()
    {
      if (_use_case_ != nullptr) {

        // Some checks and ops needed...

        delete _use_case_;
        _use_case_ = nullptr;
      }
     return;
    }

    void session::_at_terminate_()
    {
      DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());

      // Unplug the session from the list of subsessions of its parent session:
      // if (has_parent()) {
      //   _parent_->_sys_remove_subsession_(*this);
      // }

      _functional_.reset();
      _distributable_.reset();
      _parent_  = nullptr;
      _id_ = INVALID_ID;
      _destroy_use_case_();

      DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
      return;
    }

    // void session::_sys_add_subsession_(session & s_)
    // {
    //   DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());
    //   DT_THROW_IF(s_._parent_ != this, std::logic_error,
    //               "Only subsession can be added in their parent session!");
    //   _subsessions_.insert(&s_);
    //   for (auto s : _subsessions_) {
    //     std::cerr << "[trace:vire::cmsserver::session::_sys_add_subsession_] "
    //               << "  -> subsession @" << s << std::endl;
    //   }
    //   DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
    //   return;
    // }

    // void session::_sys_remove_subsession_(session & s_)
    // {
    //   DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());
    //   DT_LOG_TRACE(vire::cmsserver::logging(), "this = @" << this);
    //   DT_LOG_TRACE(vire::cmsserver::logging(), "s    = @" << &s_);
    //   DT_LOG_TRACE(vire::cmsserver::logging(),
    //                "Current subsessions (before) : " << _subsessions_.size());
    //   for (auto s : _subsessions_) {
    //     std::cerr << "[trace]  -> subsession @" << s << std::endl;
    //   }
    //   DT_THROW_IF(_subsessions_.count(&s_) == 0,
    //               std::logic_error,
    //               "Only subsession can be removed from their parent session!");
    //   _subsessions_.erase(&s_);
    //   DT_LOG_TRACE(vire::cmsserver::logging(),
    //                "Current subsessions (after) : " << _subsessions_.size());
    //   for (auto s : _subsessions_) {
    //     std::cerr << "[trace]  -> subsession @" << s << std::endl;
    //   }
    //   DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
    //   return;
    // }

    /*
    void session::_initialize_pools_(const vire::resource::manager & rmgr_,
                                     const session_info & sinfo_)
    {
      DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());
      // Special init operations:
      DT_LOG_TRACE(vire::cmsserver::logging(), "Special init operations...");

      // Extract information from the session info and build the session functional
      // and distributable pool of resource from it:
      // int32_t role_id = sinfo_.get_role_identifier();
      std::string role_name = sinfo_.get_role_identifier();
      DT_LOG_TRACE(vire::cmsserver::logging(), "role_name='" << role_name << "'");
      const vire::resource::role & r = rmgr_.get_role_by_name(role_name);
      DT_LOG_TRACE(vire::cmsserver::logging(), "Fetched role from the manager");

      // Build functional resources from the role description:
      if (r.has_functional_resource_selector()) {
        DT_LOG_TRACE(vire::cmsserver::logging(), "Building functional session...");
        cardinalities_request_type functional_request;
        resource_cardinality rc(functional_request);
        rc.build_from_role(rmgr_,
                           r,
                           ::vire::resource::role::RESOURCE_SET_FUNCTIONAL,
                           sinfo_.get_special_functional_cardinalities());
        if (is_root()) {
          resource_pool::init_root(this->_functional_,
                                   rmgr_,
                                   functional_request,
                                   resource_pool::CARD_ALL);
        } else {
          resource_pool::init_daughter_from_parent(_parent_->_distributable_,
                                                   this->_functional_,
                                                   rmgr_,
                                                   functional_request);
        }
      } else {
        DT_LOG_TRACE(vire::cmsserver::logging(), "No functional resources.");
      }

      // Build distributable resources from the role description:
      if (r.has_distributable_resource_selector()) {
        DT_LOG_TRACE(vire::cmsserver::logging(), "Building distributable session...");
        cardinalities_request_type distributable_request;
        resource_cardinality rc(distributable_request);
        rc.build_from_role(rmgr_,
                           r,
                           ::vire::resource::role::RESOURCE_SET_DISTRIBUTABLE,
                           sinfo_.get_special_distributable_cardinalities());
        if (is_root()) {
          resource_pool::init_root(this->_distributable_,
                                   rmgr_,
                                   distributable_request,
                                   resource_pool::CARD_ALL);
        } else {
          resource_pool::init_daughter_from_parent(_parent_->_distributable_,
                                                   this->_distributable_,
                                                   rmgr_,
                                                   distributable_request);
        }
      } else {
        DT_LOG_TRACE(vire::cmsserver::logging(), "No distributable resources.");
      }

      // // Generate unique key and register from the root process:
      // _process_.grab_root().register_session(*this);

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
      return;
    }
    */

    void session::_at_init_()
    {
      DT_THROW_IF(!has_id(), std::logic_error, "Session has no ID!");
      _check_use_case_();

      return;
    }

    void session::initialize_simple()
    {
      DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());
      DT_THROW_IF(is_initialized(), std::logic_error, "Session is already initialized!");

      _at_init_();

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
      return;
    }

    // void session::initialize(const vire::resource::manager & rmgr_,
    //                          const session_info & sinfo_)
    // {
    //   DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());
    //   DT_THROW_IF(is_initialized(), std::logic_error, "Session is already initialized!");



    //   _post_init_();
    //   _initialized_ = true;
    //   DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
    //   return;
    // }

    void session::terminate()
    {
      DT_LOG_TRACE_ENTERING(vire::cmsserver::logging());
      DT_THROW_IF(!is_initialized(), std::logic_error, "Session is not initialized!");
      _initialized_ = false;

      /*
      for (session_dict_type::iterator isession = _subsessions_.begin();
           isession != _subsessions_.end();
           isession++) {
        session * s = isession->second->get();
        if (s != nullptr) {
          delete s;
        }
      }
      */
      // _subsessions_.clear();

      _at_terminate_();

      DT_LOG_TRACE_EXITING(vire::cmsserver::logging());
      return;
    }

    bool session::is_initialized() const
    {
      return _initialized_;
    }

    bool session::has_subsessions() const
    {
      return _subsessions_.size() > 0;
    }

    const session::session_dict_type & session::get_subsessions() const
    {
      return _subsessions_;
    }

    void session::build_subsession_names(std::set<std::string> & names_) const
    {
      names_.clear();
      for (const auto & s : _subsessions_) {
        names_.insert(s.first);
      }
      return;
    }

    // session_set_type & session::grab_subsessions()
    // {
    //   return _subsessions_;
    // }

    // void session::list_subsessions(std::ostream & out_,
    //                                const std::string & title_,
    //                                const std::string & indent_,
    //                                uint32_t flags_) const
    // {
    //   if (!title_.empty()) {
    //     out_ << indent_ << title_ << std::endl;
    //   }

    //   for (session_set_type::const_iterator isess = _subsessions_.begin();
    //        isess != _subsessions_.end();
    //        isess++) {
    //     const session * sess = *isess;
    //     out_ << indent_;
    //     session_set_type::const_iterator jsess = isess;
    //     jsess++;
    //     if (jsess == _subsessions_.end()) {
    //       out_ << i_tree_dumpable::last_tag;
    //     } else {
    //       out_ << i_tree_dumpable::tag;
    //     }
    //     out_ << "Subsession: ";
    //     out_ << "key='" << sess->get_key() << "' ";
    //     out_ << "pid=[" << sess->get_process().get_pid() << ']';
    //     out_ << ' ';
    //     out_ << "(address=@" << sess << ')';
    //     out_ << std::endl;
    //   }

    //   return;
    // }

    void session::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Identifier : ";
      out_ << _id_;
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Parent     : ";
      if (has_parent()) {
        out_ << "[@" << _parent_ << "]";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Functional : ";
      out_ << std::endl;
      {
        std::ostringstream indentoss;
        indentoss << indent_ << i_tree_dumpable::skip_tag;
        _functional_.tree_dump(out_, "", indentoss.str());
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Distributable : ";
      out_ << std::endl;
      {
        std::ostringstream indentoss;
        indentoss << indent_ << i_tree_dumpable::skip_tag;
        _distributable_.tree_dump(out_, "", indentoss.str());
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Use case : ";
      if (has_use_case()) {
        out_ << "[@" << _use_case_ << "]";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      // out_ << indent_ << i_tree_dumpable::tag
      //      << "Subsessions : ";
      // if (has_subsessions()) {
      //   out_ << "[" << _subsessions_.size() << "]";
      // } else {
      //   out_ << "<none>";
      // }
      // out_ << std::endl;
      // list_subsessions(out_, "", indent_ + "|   ");

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Initialized : " << std::boolalpha << is_initialized() << std::endl;

      return;
    }

    void session::set_check_only(bool check_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Session is already initialized!");
      _check_only_ = check_;
      return;
    }

    bool session::is_check_only() const
    {
      return _check_only_;
    }

    bool session::can_run() const
    {
      return !_check_only_ && is_initialized();
    }

    bool session::is_running() const
    {
      return _running_;
    }

    void session::run()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Session is not initialized!");
      DT_THROW_IF(is_check_only(), std::logic_error,
                  "Session is in check-only mode and is not configured for running!");
      DT_THROW_IF(is_running(), std::logic_error,
                  "Session is already running!");
      _running_ = true;
      DT_LOG_DEBUG(_logging_, "Session is running...");
      // _use_case_->run_distributable_up();
      // std::packaged_task<work_report_type()> use_case_task(std::bind(&base_use_case::run_functional, _use_case_));
      // std::future<work_report_type> work_report = use_case_task.get_future();
      // std::thread use_case_thread(std::move(use_case_task));
      // _at_run_();
      // use_case_thread.join();
      // _use_case_->run_distributable_down();
      _running_ = false;
      return;
    }

    /* start                                                               stop
     * --[-----------+--------------------------------------------+----------]----> time
     *   <--up-->....<-----------work---------------------->......<--down-->..
     *    up_max                                                   down_max
     *
     *
     */
    void session::_at_run_()
    {
      //std::this_thread::sleep_for(std::chrono::seconds(10));
      return;
    }

    /*
    void session::create_connection()
    {
      DT_THROW_IF(!is_ready_for_connection(), std::logic_error, "Session cannot be connected!");

      _conn_.reset(new client_connection);
      _conn_->set_mother_session(*this);
      _conn_->set_id(++_last_connection_id_);
      {
        std::pair<std::string,std::string> p = ::session_users().new_user_password();
        _conn_->set_user_login(p.first);
        _conn_->set_user_password(p.second);
      }
      {
        boost::posix_time:ptime start = vire::time::now();
        boost::posix_time:ptime stop = _when_.end();
        if (_use_case_->has_down_max_duration()) {
          stop -= _use_case_->get_down_max_duration();
        }
        if ( _use_case_->has_stop_macro()) {
          // substract the stop macro max time...
        }
        boost::posix_time::time_period ti(start, stop);
        _conn_->set_time_interval(ti);
      }

      // register user in rabbit...

      return;
    }

    void session::destroy_connection()
    {
      if (_conn_.get() == nullptr) return;

      if (_conn_->is_initialized()) {
        // unregister user from rabbit...
        _conn_->reset();
      }

      _conn_.reset();
      return;
    }
    */

  } // namespace cmsserver

} // namespace vire
