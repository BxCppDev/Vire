// Ourselves:
#include <vire/rabbitmq/transport_driver.h>

// Third party:
// - BxRabbitMQ:
#include <bayeux/rabbitmq/rabbit_mgr.h>
#include <bayeux/rabbitmq/parameters.h>
#include <bayeux/rabbitmq/connection.h>
#include <bayeux/rabbitmq/channel.h>
#include <bayeux/rabbitmq/management.h>

// - Bayeux:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace rabbitmq {

    VIRE_COM_TRANSPORT_DRIVER_REGISTRATION_IMPLEMENT(transport_driver,
                                                     "vire::rabbitmq::transport_driver")

    struct transport_driver::pimpl_type
    {
      ::rabbitmq::connection_parameters con_params;
      std::unique_ptr<::rabbitmq::connection> con;
      ::rabbitmq::channel * channel_handle = nullptr;
    };

    transport_driver::transport_driver()
    {
      return;
    }

    transport_driver::transport_driver(vire::com::domain & domain_)
      : vire::com::i_transport_driver(domain_)
    {
      return;
    }

    transport_driver::~transport_driver()
    {
      return;
    }

    void transport_driver::_initialize_impl_(const datatools::properties & config_)
    {
      std::string server_host;
      uint16_t    server_port = -1;
      std::string user_login;
      std::string user_passwd;

      if (config_.has_key("host")) {
        server_host = config_.fetch_string("host");
      }

      if (config_.has_key("port")) {
        server_port = config_.fetch_positive_integer("port");
      }

      if (config_.has_key("login")) {
        user_login = config_.fetch_string("login");
      }

      if (config_.has_key("passwd")) {
        user_passwd = config_.fetch_string("passwd");
      }

      if (server_port < 0) {
        server_port = ::rabbitmq::rabbit_mgr::SSL_PORT;
      }

      DT_THROW_IF(server_host.empty(), std::logic_error, "Missing RabbitMQ server host!");
      // DT_THROW_IF(server_port < 0, std::logic_error, "Missing RabbitMQ server host!");
      DT_THROW_IF(user_login.empty(), std::logic_error, "Missing RabbitMQ user login!");
      DT_THROW_IF(user_passwd.empty(), std::logic_error, "Missing RabbitMQ user password!");

      _pimpl_.reset(new pimpl_type);
      ::rabbitmq::connection_parameters & con_params = _pimpl_->con_params;
      con_params.host   = server_host;
      con_params.port   = server_port;
      con_params.vhost  = get_domain().get_name();
      con_params.login  = user_login;
      con_params.passwd = user_passwd;

      _pimpl_->con.reset(new ::rabbitmq::connection(con_params));
      DT_THROW_IF(!_pimpl_->con->is_ok(), std::logic_error, "Failed to connect the RabbitMQ server!");
       _pimpl_->channel_handle = &_pimpl_->con->grab_channel();

      return;
    }

    void transport_driver::_reset_impl_()
    {
      _pimpl_->channel_handle = nullptr;
      _pimpl_->con.reset();
      _pimpl_.reset();
      return;
    }

    std::string transport_driver::_new_private_mailbox_address_impl_(const vire::com::mailbox::mode_type mode_)
    {
      std::string address;
      ::rabbitmq::vhost dom_vhost;
      dom_vhost.name = get_domain().get_name();
      if (mode_ == vire::com::mailbox::MODE_SERVICE) {
      }
      if (mode_ == vire::com::mailbox::MODE_EVENT) {
      }

      ::rabbitmq::queue_parameters queue_params;
      queue_params.name = "";
      queue_params.auto_delete = true;
       ::rabbitmq::error_response err;
      bool success = true;
      try {
        _pimpl_->channel_handle->queue_declare(queue_params);
        address = queue_params.name;
      } catch (std::exception & x) {
        DT_THROW(std::logic_error,
                 "RabbitMQ error: " << "Failed to declare a new queue!");
      }
      return address;
    }

  } // namespace rabbitmq

} // namespace vire
