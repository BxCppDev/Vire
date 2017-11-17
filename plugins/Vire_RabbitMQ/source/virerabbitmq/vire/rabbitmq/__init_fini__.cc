
// Standard library:
#include <iostream>
#include <cstdlib>

// This project:
#include <datatools/logger.h>

// __attribute__((constructor)) void _vire_rabbitmq_init_(void)
// {
//   std::cerr << ("library loaded!\n");
//   return;
// }
// __attribute__((destructor)) void _vire_rabbitmq_fini_(void)
// {
//   std::cerr << ("library done!\n");
//   return;
// }

namespace {

  struct initializer
  {
    initializer()
    {
      char * e = getenv("VIRE_RABBITMQ_LOGGING");
      if (e != nullptr) {
        logging = datatools::logger::get_priority(std::string(e));
      }

      DT_LOG_NOTICE(logging, "Loading the Vire RabbitMQ plugin library...");
    }

    ~initializer()
    {
      DT_LOG_NOTICE(logging, "Unloading the Vire RabbitMQ plugin library...");
    }

    datatools::logger::priority logging = datatools::logger::PRIO_FATAL;

  };
  static initializer i;
}
