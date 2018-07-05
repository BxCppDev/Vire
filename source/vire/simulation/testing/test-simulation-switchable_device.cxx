// test-simulation-switchable_device.cxx

// Ourselves:
#include <vire/simulation/switchable_device.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <exception>
#include <thread>
#include <random>

// This project:
#include <vire/vire.h>

void test_switchable_device_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::simulation::switchable_device' class."
              << std::endl;

    test_switchable_device_1();

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

struct device_printer
{
  device_printer(const vire::simulation::switchable_device & dev_)
    : _device_(dev_)
  {
  }
  
  void operator()()
  {
    std::clog << "Device printer is starting..." << std::endl;
    do {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      std::clog << "Device: " << _device_.get_name() << std::endl;
      std::clog << "  Out-of-order : " << std::boolalpha << _device_.is_out_of_order() << std::endl;
      std::clog << "  On           : " << std::boolalpha << _device_.is_on() << std::endl;
      std::clog << std::endl;
    } while (!_device_.is_out_of_order());
    return;
  }
  
  const vire::simulation::switchable_device & _device_;
};

struct device_switcher
{
  device_switcher(vire::simulation::switchable_device & dev_, const double period_)
    : _device_(dev_)
    , _period_(period_)
  {
  }

  void operator()()
  {
    std::clog << "Device switcher is starting..." << std::endl;
    int n_milliseconds = (int) (_period_ * 1000); 
    while (!_device_.is_out_of_order()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(n_milliseconds));
      std::clog << "Now we switch the device!" << std::endl;
      _device_.switch_it();
    }
    return;
  }
  
  vire::simulation::switchable_device & _device_;
  double _period_; ///< Period
};

struct device_breaker
{
  device_breaker(vire::simulation::switchable_device & dev_, const double decay_)
    : _device_(dev_)
    , _decay_(decay_)
  {
  }

  void operator()()
  {
    std::clog << "Device breaker is starting..." << std::endl;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::clog << "decay : " << _decay_ << " s" << std::endl;
    double decay_ms = _decay_ * 1000;
    std::clog << "decay : " << decay_ms << " ms" << std::endl;
    std::exponential_distribution<double> distribution_ms(1.0/decay_ms);
    double breakdown_time_ms = std::min(distribution_ms(generator), 7000.0); + 1000.0;
    std::clog << "breakdown_time_ms : " << breakdown_time_ms << " ms" << std::endl;
    int n_milliseconds = (int) breakdown_time_ms; 
    std::clog << "Device scheduled lifetime : " << n_milliseconds << " ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(n_milliseconds));
    std::clog << "Now we break the device!" << std::endl;
    _device_.breakdown();
    return;
  }
  
  vire::simulation::switchable_device & _device_;
  double _decay_; ///< Mean lifetime in seconds
};

void test_switchable_device_1()
{
  std::clog << "\ntest_switchable_device_1: basics" << std::endl;

  using vire::simulation::switchable_device;

  switchable_device engine;
  engine.set_name("myEngine");
  engine.set_display_name("My engine");
  engine.set_terse_description("A dummy engine");
  engine.set_logging_priority(datatools::logger::PRIO_DEBUG);
  {
    boost::property_tree::ptree popts;
    popts.put("title", "Engine: ");
    engine.print_tree(std::clog, popts);
  }

  device_printer  printer(engine);
  device_switcher switcher(engine, 0.5);
  device_breaker  breaker(engine, 4.5);
  std::thread t1(printer);
  std::thread t2(switcher);
  std::thread t3(breaker);
  t1.join();
  t2.join();
  t3.join();
  {
    boost::property_tree::ptree popts;
    popts.put("title", "Engine: ");
    popts.put("indent", ">>> ");
    popts.put("inherit", true);
    engine.print_tree(std::clog, popts);
  }
   
  std::clog  << std::endl;
  return;
}

