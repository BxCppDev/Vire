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

#include <bayeux/datatools/library_loader.h>
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/vire.h>

void test_server_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::server' class."
              << std::endl;

    datatools::library_loader dll_loader;
    dll_loader.load("Vire_RabbitMQ");
    test_server_1();

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
    if (gCmsServerHandle && gCmsServerHandle->is_running()) {
      std::cerr << "Stop the CMS server...\n";
      gCmsServerHandle->request_stop();
    }
  }
  if (signum_ == SIGABRT) {
    if (gCmsServerHandle && gCmsServerHandle->is_running()) {
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
  while (!svr_.is_stopped() && !stop_shell) {
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
      if (svr_.is_running()) {
        svr_.request_stop();
        break;
      } else {
        std::cerr << "[error] server is not running." << std::endl;
      }
    } else if (command == "uptime") {
      if (svr_.is_running()) {
        std::cout << svr_.compute_uptime() / CLHEP::second << " s" << std::endl;
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
    if (svr_.is_stopped() || svr_.is_requested_stop()) {
      std::cerr << "[devel] Break shell.\n";
      break;
    }
  } // end of shell loop
  std::clog << std::endl;
  std::cerr << "[devel] Shell is terminated.\n";
  return;
}

void cmsserver_stopper(vire::cmsserver::server & svr_)
{
  std::this_thread::sleep_for(std::chrono::seconds(10));
  svr_.request_stop();
  return;
}

void test_server_1()
{
  std::clog << "\ntest_server_1: basics" << std::endl;
  void (*prev_handler)(int);
  prev_handler = std::signal(SIGTERM, cmsserver_signal_handler);

  vire::cmsserver::server cmsServer;
  gCmsServerHandle = &cmsServer;

  cmsServer.set_logging(datatools::logger::PRIO_DEBUG);

  datatools::properties cmsServerConfig;
  cmsServerConfig.read_configuration("@snemo:config/snemo/demonstrator/cms/server/main.conf",
                                     datatools::properties::config::RESOLVE_PATH);
  cmsServer.initialize(cmsServerConfig);
  cmsServer.tree_dump(std::clog, "Vire CMS Server: ");

  std::thread t(&vire::cmsserver::server::run, &cmsServer);
  std::thread ui(cmsserver_shell, std::ref(cmsServer));
  std::thread stopper(cmsserver_stopper, std::ref(cmsServer));
  t.join();
  ui.join();
  stopper.join();

  cmsServer.reset();
  gCmsServerHandle = nullptr;
  std::clog << std::endl;
  return;
}
