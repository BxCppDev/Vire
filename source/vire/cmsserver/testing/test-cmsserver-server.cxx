// test-cmsserver-server.cxx

// Ourselves:
#include <vire/cmsserver/server.h>

// Standard library:
#include <unistd.h>
#include <chrono>
#include <thread>
#include <csignal>
#include <sstream>
#include <future>

// Third party:
// - Qt:
#include <QApplication>
#include <QVBoxLayout>
// - Bayeux:
#include <bayeux/datatools/library_loader.h>
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/datatools/service_manager.h>

// This project:
#include <vire/vire.h>
#include <vire/cmsserver/ui/server_panel.h>
#include <vire/resource/manager.h>

struct params_type
{
  bool use_gui = false;
  bool use_tui = false;
  bool use_stopper = true;
};

void test_server_1(const params_type &);

int main(int argc_, char ** argv_)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::server' class."
              << std::endl;

    params_type params;

    int iarg = 1;
    while (iarg < argc_) {
      std::string arg = argv_[iarg];
      if (arg == "--gui") {
        params.use_gui = true;
        params.use_stopper = false;
      }
      if (arg == "--tui") {
        params.use_tui = true;
        params.use_stopper = false;
      }
      if (arg == "--no-stopper") {
        params.use_stopper = false;
      }
      iarg++;
    }
     
    test_server_1(params);

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

namespace {
  static  vire::cmsserver::server * gCmsServerHandle = nullptr;
}

void cmsserver_signal_handler(int signum_)
{
  std::cerr << "Interrupt signal (" << signum_ << ") received.\n";
  if (signum_ == SIGTERM) {
    if (gCmsServerHandle && gCmsServerHandle->get_rc().is_running()) {
      std::cerr << "Stop the CMS server...\n";
      gCmsServerHandle->stop();
    }
  }
  if (signum_ == SIGABRT) {
    if (gCmsServerHandle && gCmsServerHandle->get_rc().is_running()) {
      std::cerr << "Abort the CMS server...\n";
      gCmsServerHandle->reset();
    }
    exit(signum_);
  }
  return;
}

void cmsserver_shell(vire::cmsserver::server & svr_)
{
  typedef std::pair<bool,std::string> result_type;
  std::cerr << "Entering shell...\n";
  bool stop_shell = false;
  while (!svr_.get_rc().is_stopped() && !stop_shell) {
    // Input command line:
    std::clog << "cmsserver> "; // prompt
    std::string command_line;

    /*
    // Lambda function:
    auto getline_lambda = [&] (const vire::cmsserver::server &) {
      std::cerr << "[devel] getline_lambda: Entering...\n";
      std::string cl;
      result_type p;
      p.first = false;
      p.second = "";
      if (std::cin.good() && svr_.is_running()) {
        std::cerr << " [devel] getline...\n";
        if (std::getline(std::cin, cl)) {
          p.first = true;
          p.second = cl;
          return p;
        } else {
          p.first = false;
          p.second = "";
        }
      }
      return p;
    };

    std::future<result_type> f = std::async(std::launch::async, getline_lambda, std::ref(svr_));
    std::cerr << "[devel] Waiting for the future, f.valid() == "
              << std::boolalpha << f.valid() << "\n";
    std::chrono::milliseconds span(500);
    result_type res;
    std::clog << "[devel] Input loop..." << std::endl;
    while (f.wait_for(span) != std::future_status::ready) {
      std::clog << "[devel] Still waiting..." << std::endl;
      if (!f.valid()) {
        std::cerr << "[devel] Future is not valid!" << "\n";
        stop_shell = true;
        break;
      }
      if (std::cin.eof()) {
        std::cerr << "[devel] Input is EOF!" << "\n";
        stop_shell = true;
        break;
      }
      if (!svr_.is_running()) {
        std::cerr << "[devel] CMS server is not running anymore!" << "\n";
        stop_shell = true;
        break;
      }
    }
    std::cerr << "[devel] Input loop is done" << "\n";
    if (! stop_shell) {
      if (f.valid()) {
        std::clog << "[devel] Fetching the command line from the future..." << std::endl;
        res = f.get();
        if (!res.first) {
          stop_shell = true;
        }
      }
    }
    if (stop_shell) return;
    command_line = res.second;
    */

    if (!std::getline(std::cin, command_line)) {
      break;
    }

    // Process command line:
    std::cerr << "[devel] Process command line:  '" << command_line << "'" << "\n";
    std::string command;
    {
      std::istringstream command_iss(command_line);
      command_iss >> command;
    }
    if (command.empty() || command[0] == '#') {
      continue;
    }
    if (command == "stop") {
      if (svr_.get_rc().is_running()) {
        svr_.stop();
        break;
      } else {
        std::cerr << "[error] server is not running." << std::endl;
      }
    } else if (command == "uptime") {
      if (svr_.get_rc().is_running()) {
        vire::time::system_duration uptime;
        svr_.get_rc().compute_uptime(uptime);
        double uptime_unit = vire::time::to_real_with_unit(uptime);
        std::cout << uptime_unit / CLHEP::second << " s" << std::endl;
      } else {
        std::cerr << "[error] server is not running." << std::endl;
      }
    } else if (command == "quit") {
      break;
    } else if (command == "help") {
      std::cout << "help    :   print this help" << std::endl;
      std::cout << "uptime  :   print the uptime" << std::endl;
      std::cout << "stop    :   stop the server" << std::endl;
      std::cout << "quit    :   quit" << std::endl;
    } else {
      std::cerr << "[error] " << command << " : Unknown command." << std::endl;
    }
    vire::running::run_status_type runStatus = svr_.get_rc().get_status();
    if (runStatus == vire::running::RUN_STATUS_STOPPED
        || runStatus == vire::running::RUN_STATUS_STOPPING
        || svr_.get_rc().is_stop_requested()) {
      std::cerr << "[devel] Break shell.\n";
      break;
    }
  } // end of shell loop
  std::clog << std::endl;
  std::cerr << "[devel] Shell is terminated.\n";
  return;
}

void cmsserver_stopper(vire::cmsserver::server & svr_, const unsigned int elapsed_seconds_= 10)
{
  std::this_thread::sleep_for(std::chrono::seconds(elapsed_seconds_));
  svr_.stop();
  return;
}

void test_server_1(const params_type & params_)
{
  std::clog << "\ntest_server_1: basics" << std::endl;
  void (*prev_handler)(int);
  prev_handler = std::signal(SIGTERM, cmsserver_signal_handler);

  vire::cmsserver::server cmsServer;
  gCmsServerHandle = &cmsServer;

  cmsServer.set_logging(datatools::logger::PRIO_DEBUG);

  datatools::properties cmsServerConfig;
  cmsServerConfig.read_configuration("@snemock:config/cms/test1/server/main.conf",
                                     datatools::properties::config::RESOLVE_PATH);
  cmsServer.initialize(cmsServerConfig);
  cmsServer.tree_dump(std::clog, "Vire CMS Server: ");


  const datatools::service_manager & services = cmsServer.get_services();
  if (services.is_a<vire::resource::manager>(vire::cmsserver::server::resources_service_name())) {
    const vire::resource::manager & resources = services.get<vire::resource::manager>(vire::cmsserver::server::resources_service_name());
    resources.tree_dump(std::clog, "Resources: ");
  }
  
  std::thread t(&vire::cmsserver::server::run, &cmsServer);
  
  std::unique_ptr<std::thread> tui;
  if (params_.use_tui) {
    tui.reset(new std::thread(cmsserver_shell, std::ref(cmsServer)));
  }
  std::unique_ptr<std::thread> stopper;
  if (params_.use_stopper) {
    stopper.reset(new std::thread(cmsserver_stopper, std::ref(cmsServer), 10));
  }
  
  if (params_.use_gui) {
    int argc = 1;
    const char * argv[] = { "test-cmsserver-server" };
    QApplication app(argc, (char **) argv);

    using vire::cmsserver::ui::server_panel;
    server_panel * serverPanel = new server_panel;
    serverPanel->set_server(cmsServer);

    QWidget window;
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(serverPanel);
    window.setLayout(layout);
    QObject::connect(&cmsServer.grab_emitter(), SIGNAL(sig_stop()),
                     QApplication::instance(), SLOT(quit()));
    window.show();
    app.exec();
  }
  
  if (tui) {
    tui->join();
    tui.reset();
  }
  if (stopper) {
    stopper->join();
    stopper.reset();
  }
  t.join();
   
  cmsServer.reset();
  gCmsServerHandle = nullptr;
  std::clog << std::endl;
  return;
}
