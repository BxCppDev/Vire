// file:///data/sw/Cadfael/Binary/Cadfael-trunk/Build-Linux-x86_64/ports/xerces-c/xerces-c-prefix/src/xerces-c/doc/html/program-dom-3.html

// Standard Library:
#include <string>
#include <iostream>

// Third Party:
// - Boost++:
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/scoped_ptr.hpp>
// - Xerces-C++:
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/exception.h>
#include <datatools/utils.h>

// This project:
#include <vire/mos/vire_mos_config.h>
#include <vire/mos/vire_mos.h>
#include <vire/mos/version.h>
#include <vire/mos/opcua_ast.h>
#include <vire/mos/opcua_dom.h>
#include <vire/mos/opcua_export_vire.h>

//! Print application usage (supported options and arguments)
void app_usage(std::ostream & out_,
               const boost::program_options::options_description & desc_);

//! Print application version
void app_version(std::ostream & out_);

//! Raw scanning of the OPCUA server (developers only):
void scan_opcua_server(const vire::mos::OPCUA & server_);

//! \brief Application configuration parameters
struct app_config_params {
  //! Default constructor
  app_config_params();
  datatools::logger::priority logging; //!< Logging priority threshold
  std::string input_xml_filename; //!< Name of the input XML MOS server file
  std::string no_namespace_schema_loc;
  std::string output_vire_model_dir; //!< Name of the directory where to store Vire model definition files
  std::string server_model_name; //!< Name of the top level server model
  bool export_internals; //!< Flag to export non public (protected/internal) components
};

int main(int argc_, char * argv_[])
{
  vire::initialize(argc_, argv_, 0);
  int error_code = EXIT_SUCCESS;
   try {
    xercesc::XMLPlatformUtils::Initialize();

    // The configurarion parameter sets:
    app_config_params cfg;

    // Describe command line arguments :
    namespace po = boost::program_options;
    po::options_description opts("Allowed options");
    opts.add_options()

      ("help,h", "produce help message")

      ("version,v", "print version")

      ("logging-priority,L",
       po::value<std::string>()
       ->value_name("level"),
       "set the logging priority")

      ("input-xml-file,i",
       po::value<std::string>(&cfg.input_xml_filename)
       ->value_name("file"),
       "set the name of the input XML MOS server file")

      ("output-dir,d",
       po::value<std::string>(&cfg.output_vire_model_dir)
       ->value_name("dir"),
       "set the name of the output directory where to store device model definition files")

      ("server-model-name,s",
       po::value<std::string>(&cfg.server_model_name)
       ->value_name("name"),
       "set the name of the top level server model")

      ("no-namespace-schema,n",
       po::value<std::string>(&cfg.no_namespace_schema_loc)
       ->value_name("file"),
       "set the name of schema file")

      ("export-internals,I", "export the internal components")

      ; // end of options description

    po::variables_map vm;
    try {
      po::positional_options_description args;
      args.add("input-xml-file", 1);
      args.add("output-dir", 1);
      po::store(po::command_line_parser(argc_, argv_)
                .options(opts)
                .positional(args)
                .run(), vm);
      po::notify(vm);

    }
    catch (std::exception & po_error) {
      app_usage(std::cerr, opts);
      throw;
    }

    // Use command line arguments :

    if (vm.count("help")) {
      app_usage(std::cout, opts);
      return(error_code);
    }

    if (vm.count("version")) {
      app_version(std::cout);
      return(error_code);
    }

    // Fetch the verbosity level:
    if (vm.count("logging-priority")) {
      const std::string & logging_label = vm["logging-priority"].as<std::string>();
      cfg.logging = datatools::logger::get_priority(logging_label);
      DT_THROW_IF(cfg.logging == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                  "Invalid logging priority label '" << logging_label << "' !");
    }

    if (vm.count("export-internals")) {
      cfg.export_internals = true;
    }

    // Checks:

    DT_THROW_IF(cfg.input_xml_filename.empty(), std::logic_error, "Missing input XML filename!");
    DT_LOG_NOTICE(datatools::logger::PRIO_ALWAYS,
                  "Input XML file : '" << cfg.input_xml_filename << "'.");
    datatools::fetch_path_with_env(cfg.input_xml_filename);
    DT_THROW_IF(! boost::filesystem::exists(cfg.input_xml_filename),
                std::runtime_error,
                "Missing input XML file '" << cfg.input_xml_filename << "'!");

    if (cfg.no_namespace_schema_loc.empty()) {
      cfg.no_namespace_schema_loc = "@viremos:data/mos/xml/2.0.1/MOS_OPCUA.xsd";
    }
    datatools::fetch_path_with_env(cfg.no_namespace_schema_loc);
    DT_THROW_IF(! boost::filesystem::exists(cfg.no_namespace_schema_loc),
                std::runtime_error,
                "Missing no namespace schema file '" << cfg.no_namespace_schema_loc << "'!");

    if (cfg.output_vire_model_dir.empty()) {
      cfg.output_vire_model_dir = "/tmp/${USER}";
    }
    datatools::fetch_path_with_env(cfg.output_vire_model_dir);
    if (! boost::filesystem::exists(cfg.output_vire_model_dir)) {
      if (!boost::filesystem::create_directories(cfg.output_vire_model_dir)) {
        DT_THROW(std::runtime_error,
                 "Cannot create output directory '" << cfg.output_vire_model_dir << "'!");
      }
    }

    // --------------------------------------------------
    // Xerces-C++ XML parse:
    boost::scoped_ptr<xercesc::XercesDOMParser> parser;
    parser.reset(new xercesc::XercesDOMParser);
    parser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
    // Optional:
    parser->setExitOnFirstFatalError(true);
    // parser->setDoNamespaces(false);
    // parser->setCreateEntityReferenceNodes(true);
    // parser->setValidationSchemaFullChecking(true);
    parser->setExternalNoNamespaceSchemaLocation(cfg.no_namespace_schema_loc.c_str());
    boost::scoped_ptr<xercesc::ErrorHandler> errHandler;
    errHandler.reset(dynamic_cast<xercesc::ErrorHandler*>(new xercesc::HandlerBase()));
    parser->setErrorHandler(errHandler.get());

    // Parse the document:
    parser->parse(cfg.input_xml_filename.c_str());
    xercesc::DOMDocument * document = parser->getDocument();
    const xercesc::DOMElement * root_node = document->getDocumentElement();

    // OPCUA server model:
    vire::mos::OPCUA server;
    vire::mos::opcua_builder builder(server, datatools::logger::PRIO_FATAL);
    builder(root_node);
    server.tree_dump(std::clog, "OPCUA: ");
    std::clog << std::endl;
    if (cfg.logging >= datatools::logger::PRIO_DEBUG) {
      scan_opcua_server(server);
    }

    // Terminate XML parser:
    parser->resetDocumentPool();

    // OPCUA export to Vire:
    vire::mos::opcua_export_vire to_vire_export;
    to_vire_export.set_export_path(cfg.output_vire_model_dir);
    to_vire_export.set_model_name(cfg.server_model_name);
    to_vire_export.set_logging(cfg.logging);
    to_vire_export.set_export_only_public_components(! cfg.export_internals);
    to_vire_export.process(server);

  } catch (const xercesc::XMLException & error) {
    char * error_message = xercesc::XMLString::transcode(error.getMessage());
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS, error_message);
    xercesc::XMLString::release(&error_message);
    return EXIT_FAILURE;
  } catch (const xercesc::DOMException & error) {
    char * error_message = xercesc::XMLString::transcode(error.msg);
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS, error_message);
    xercesc::XMLString::release(&error_message);
    return EXIT_FAILURE;
  } catch (const std::exception & error) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS, error.what());
    return EXIT_FAILURE;
  } catch (...) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS, "Unexpected Exception !");
    return EXIT_FAILURE;
  }

  xercesc::XMLPlatformUtils::Terminate();
  vire::terminate();
  return EXIT_SUCCESS;
}

// Definitions:
void app_version(std::ostream & out_)
{
  out_ << "Vire_MOS " << vire::mos::version::get_version() << std::endl;
  return;
}

void app_usage(std::ostream & out_,
               const boost::program_options::options_description & opts_)
{
  out_ << "Usage : " << std::endl;
  out_ << opts_ << std::endl;
  return;
}

app_config_params::app_config_params()
{
  logging = datatools::logger::PRIO_FATAL;
  export_internals = false;
  return;
}

void scan_opcua_server(const vire::mos::OPCUA & server_)
{
  if (server_.simple_devices.size()) {
    std::clog << "Details about the embedded devices: " << std::endl;
    for (const auto & sdevice : server_.simple_devices) {
      sdevice.tree_dump(std::clog, "Embedded simple device: ", "");
      if (sdevice.compound_datapoints.size()) {
        std::clog << "  Details about the " << sdevice.compound_datapoints.size() << " embedded compound datapoints: " << std::endl;
        // for (const auto & cdatapoint : sdevice.compound_datapoints) {
        for (int i = 0; i < sdevice.compound_datapoints.size(); i++) {
          const auto & cdatapoint = sdevice.compound_datapoints[i];
          cdatapoint.tree_dump(std::clog, "Embedded compound datapoint: ", "  ");
          if (cdatapoint.simple_datapoints.size()) {
            std::clog << "    Details about the embedded simple datapoints: " << std::endl;
            for (const auto & sdatapoint : cdatapoint.simple_datapoints) {
              sdatapoint.tree_dump(std::clog, "Embedded simple datapoint: ", "    ");
            }
          }
          if (cdatapoint.compound_datapoints.size()) {
            std::clog << "    Details about the embedded compound datapoints: " << std::endl;
            for (const auto & cdatapoint2 : cdatapoint.compound_datapoints) {
              cdatapoint2.tree_dump(std::clog, "Embedded compound datapoint: ", "    ");
            }
          }
        }
      }
    }
  }

  return;
}
