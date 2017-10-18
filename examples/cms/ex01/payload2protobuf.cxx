// Standard library:
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <cstdlib>

// Third party:
// - Boost
#include <boost/program_options.hpp>
// - BxProtobuftools:
#include <bayeux/protobuftools/iofile.h>

// Vire:
#include <vire/vire.h>
#include <vire/time/utils.h>
#include <vire/message/message.h>
#include <vire/message/message_header.h>
#include <vire/message/message_body.h>
#include <vire/message/body_layout.h>
#include <vire/utility/path.h>
#include <vire/cms/resource_exec.h>

struct app_params
{
  std::string mode;
  std::string setup;
  std::string leaf;
  std::string dirs;
  std::string file_prefix = "vire_message";
  std::string metadata_filename;
  std::string buffer_filename;
  std::string dump_filename;
  bool        async = false;
  std::string async_address;

  static void build_options(boost::program_options::options_description & opts_,
                            app_params & params_);
  static void print_usage(boost::program_options::options_description & opts_,
                          std::ostream & out_);

};

int main(int argc_, char ** argv_)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {

    bool process = true;
    app_params params;
    namespace po = boost::program_options;
    po::options_description opts("Allowed options ");
    app_params::build_options(opts, params);
    // Aggregate options:
    po::options_description optPublic;
    optPublic.add(opts);
    po::variables_map vm;
    po::parsed_options parsed =
      po::command_line_parser(argc_, argv_)
      .options(optPublic)
      .run();
    po::store(parsed, vm);
    po::notify(vm);

    // Fetch the opts/args :
    if (vm.count("help")) {
      if (vm["help"].as<bool>()) {
        app_params::print_usage(optPublic, std::cout);
        error_code = 1;
        process = false;
      }
    }

    if (process) {
      if (params.async) {
        params.mode = "async_resource_exec_request";
      }
      if (params.mode.empty()) {
        params.mode = "sync_resource_exec_request";
      }
      if (params.setup.empty()) {
        params.setup = "SuperNEMO";
      }
      if (params.dirs.empty()) {
        params.dirs = "/Demonstrator/CMS/Coil/Control/Voltage";
      }
      if (params.leaf.empty()) {
        params.leaf = "__dp_read__";
      }
      if (params.metadata_filename.empty()) {
        params.metadata_filename = params.file_prefix + ".meta";
      }
      if (params.buffer_filename.empty()) {
        params.buffer_filename = params.file_prefix + ".buf";
      }
      if (params.dump_filename.empty()) {
        params.dump_filename = params.file_prefix + ".dump";
      }

      if (params.mode == "async_resource_exec_request") {
        // Async return queue:
        params.async_address = "snemo.vire.client_0.Die0baxu.q";
      }

      std::string resource_path = vire::utility::path::build(params.setup, params.dirs, params.leaf);
      std::string routing_key   = vire::utility::path::to_address(resource_path);

      std::clog << "[info] Resource path = [" << resource_path << "]" << std::endl;
      std::clog << "[info] Routing key   = [" << routing_key << "]" << std::endl;
      std::clog << "[info] Async address = [" << params.async_address << "]" << std::endl;

      // Message:
      vire::message::message msg;

      // Header:
      vire::message::message_header & msg_header = msg.grab_header();
      vire::message::message_identifier msg_id("vire.server", 42);
      vire::utility::model_identifier body_layout_id;
      std::clog << "[info] Vire message body layout name    : " << vire::message::body_layout::name() << std::endl;
      body_layout_id.set_name(vire::message::body_layout::name());
      std::clog << "[info] Vire message body layout version : " << vire::message::body_layout::current_version() << std::endl;
      body_layout_id.set_version(vire::message::body_layout::current_version());
      msg_header.set_message_id(msg_id);
      msg_header.set_timestamp(vire::time::now());
      msg_header.set_category(vire::message::MESSAGE_REQUEST);
      msg_header.set_asynchronous(!params.async_address.empty());
      msg_header.set_async_address(params.async_address);
      msg_header.set_body_layout_id(body_layout_id);
      msg_header.add_metadata("user_correlation_id", routing_key);

      // Payload:
      vire::cms::resource_exec rer;
      rer.set_path(resource_path);
      if (params.leaf == "__dp_read__") {
        rer.add_input_argument("refresh", "false");
      }
      rer.tree_dump(std::clog, "Resource execution request: ");
      std::clog << std::endl;

      // Body:
      vire::message::message_body & msg_body = msg.grab_body();
      vire::utility::model_identifier payload_type_id;
      payload_type_id.set("beatles", "1.0");
      // msg_body.set_payload_type_id(payload_type_id);
      msg_body.set_payload(rer);

      msg.tree_dump(std::clog, "Message: ");

      // Dump message:
      std::ofstream fdump(params.dump_filename.c_str());
      msg.tree_dump(fdump, "Vire message: ");

      // Generate metadata:
      std::ofstream fmetadata(params.metadata_filename.c_str());
      fmetadata << "resource_path=" << resource_path << std::endl;
      fmetadata << "routing_key=" << routing_key << std::endl;
      if (params.async) {
        fmetadata << "async_address=" << params.async_address << std::endl;
      }
      // Generate protobufized message:
      std::ofstream fbufferdata(params.buffer_filename.c_str(), std::ios::binary);
      protobuftools::store(params.buffer_filename, msg, 0);

      std::clog << "[info] Message dump  : '" << params.dump_filename << "'" << std::endl;
      std::clog << "[info] Metadata file : '" << params.metadata_filename << "'" << std::endl;
      std::clog << "[info] Buffer file   : '" << params.buffer_filename << "'" << std::endl;
    }
  } catch (std::logic_error error) {
    std::cerr << "[error] " << error.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "[error] " << "Unexpected error!" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return error_code;
}

// static
void app_params::build_options(boost::program_options::options_description & opts_,
                               app_params & params_)
{

  namespace po = boost::program_options;
  opts_.add_options()

    ("help,h",
     po::value<bool>()
     ->zero_tokens()
     ->default_value(false),
     "Produce help message")

    ("file-prefix,p",
     po::value<std::string>(&params_.file_prefix)
     ->value_name("prefix"),
     "Set the output files prefix. \n"
     "Example :\n"
     " --file-prefix \"vire_message\""
     )

    ("async,a",
     po::value<bool>(&params_.async)
     ->zero_tokens()
     ->default_value(false),
     "Set the asynchronous flag. \n"
     "Example :\n"
     " --async "
     )

    ("dirs,d",
     po::value<std::string>(&params_.dirs)
     ->value_name("name"),
     "Set the resource dirs path. \n"
     "Example :\n"
     " --dirs \"/Demonstrator/CMS/Coil/Control/Voltage\" "
     )

    ("leaf,l",
     po::value<std::string>(&params_.leaf)
     ->value_name("name"),
     "Set the resource leaf name. \n"
     "Example :\n"
     " --leaf \"__dp_read__\" "
     )

    ; // end of options' description
  return;
}

void app_params::print_usage(boost::program_options::options_description & opts_,
                             std::ostream & out_)
{
  static const std::string APP_NAME = "payload2protobuf";
  out_ << "\n" << APP_NAME << " -- Payload to protobuf encoding" << std::endl;
  out_ << std::endl;
  out_ << "Usage : " << std::endl;
  out_ << std::endl;
  out_ << "  " << APP_NAME << " [OPTIONS] " << std::endl;
  out_ << opts_ << std::endl;
  return;
}
