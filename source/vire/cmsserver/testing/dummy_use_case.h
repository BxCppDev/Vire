// -*- mode: c++; -*-
// dummy_use_case.h

#ifndef VIRE_CMSSERVER_TEST_DUMMY_USE_CASE_H
#define VIRE_CMSSERVER_TEST_DUMMY_USE_CASE_H

// Standard library:
#include <thread>
#include <future>

// This project:
#include <vire/cmsserver/base_use_case.h>
#include <vire/cmsserver/task_utils.h>

namespace vire {
  namespace cmsserver{
    namespace test{

      class dummy_use_case
        : public vire::cmsserver::base_use_case
      {
      public:

        //! Default constructor
        dummy_use_case(const unsigned int uts_ = 4,
                       const unsigned int wts_ = 8,
                       const unsigned int dts_ = 3)
          : _up_time_sec_(uts_)
          , _work_time_sec_(wts_)
          , _down_time_sec_(dts_)
        {
          return;
        }

        //! Destructor
        virtual ~dummy_use_case()
        {
          return;
        }

        virtual void _at_initialized_(const datatools::properties & config_)
        {
          return;
        }

        virtual void _at_reset_()
        {
          return;
        }

        virtual void _at_up_()
        {
          std::cerr << "dummy_use_case: up: entering..." << std::endl;
          for (unsigned int tick = 1; tick <= _up_time_sec_; tick++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cerr << "dummy_use_case: up: elapsed = "
                      << tick << '/'<< _up_time_sec_ << " sec" << std::endl;
          }
          std::cerr << "dummy_use_case: up: exiting." << std::endl;
          return;
        }

        virtual vire::cmsserver::work_report_type _at_work_()
        {
          std::cerr << "dummy_use_case: work: entering..." << std::endl;
          vire::cmsserver::work_report_type work_report;
          work_report.error_code = 0;
          for (unsigned int tick = 1; tick <= _work_time_sec_; tick++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cerr << "dummy_use_case: work: elapsed = "
                      << tick << '/'<< _work_time_sec_ << " sec" << std::endl;
          }
          std::cerr << "dummy_use_case: work: exiting." << std::endl;
          return work_report;
        }

        virtual void _at_down_()
        {
          std::cerr << "dummy_use_case: down: entering..." << std::endl;
          for (unsigned int tick = 1; tick <= _down_time_sec_; tick++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cerr << "dummy_use_case: down: elapsed = "
                      << tick << '/'<< _down_time_sec_ << " sec" << std::endl;
          }
          std::cerr << "dummy_use_case: down: exiting." << std::endl;
          return;
        }

        unsigned int get_work_time_sec() const
        {
          return _work_time_sec_;
        }

        unsigned int get_up_time_sec() const
        {
          return _up_time_sec_;
        }

        unsigned int get_down_time_sec() const
        {
          return _down_time_sec_;
        }

        unsigned int get_total_time_sec() const
        {
          return get_work_time_sec() +  get_up_time_sec() + get_down_time_sec();
        }

        unsigned int _up_time_sec_   = 5;
        unsigned int _work_time_sec_ = 8;
        unsigned int _down_time_sec_ = 3;

      };

    } // namespace test

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_TEST_DUMMY_USE_CASE_H

// end
