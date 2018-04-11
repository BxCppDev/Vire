// -*- mode: c++; -*-
// dummy_use_case.h

#ifndef VIRE_CMSSERVER_TEST_DUMMY_USE_CASE_H
#define VIRE_CMSSERVER_TEST_DUMMY_USE_CASE_H

// Standard library:
#include <thread>
#include <future>

// This project:
#include <vire/cmsserver/base_use_case.h>

namespace vire {
  namespace cmsserver{
    namespace test{

      class dummy_use_case
        : public vire::cmsserver::base_use_case
      {
      public:

        //! Default constructor
        dummy_use_case(const unsigned int futs_ = 4,
                       const unsigned int fwts_ = 8,
                       const unsigned int fdts_ = 3)
          : _functional_up_time_sec_(futs_)
          , _functional_work_time_sec_(fwts_)
          , _functional_down_time_sec_(fdts_)
        {
          return;
        }

        //! Destructor
        virtual ~dummy_use_case()
        {
          return;
        }

        void _at_initialize_(const datatools::properties & config_) override
        {
          if (config_.has_key("functional_up_time_sec")) {
            unsigned int futs = config_.fetch_positive_integer("functional_up_time_sec");
            _functional_up_time_sec_ = futs;
          }

          if (config_.has_key("functional_work_time_sec")) {
            unsigned int fwts = config_.fetch_positive_integer("functional_work_time_sec");
            _functional_work_time_sec_ = fwts;
          }

          if (config_.has_key("functional_down_time_sec")) {
            unsigned int fdts = config_.fetch_positive_integer("functional_down_time_sec");
            _functional_down_time_sec_ = fdts;
          }

          return;
        }

        void _compute_run_distributable_times_() override
        {
          return;
        }

        void _compute_run_functional_times_() override
        {
          set_functional_up_max_duration(boost::posix_time::seconds(_functional_up_time_sec_));
          set_functional_work_min_duration(boost::posix_time::seconds(_functional_work_time_sec_));
          set_functional_work_max_duration(boost::posix_time::seconds(_functional_work_time_sec_ + 1));
          set_functional_down_max_duration(boost::posix_time::seconds(_functional_down_time_sec_));
          return;
        }

        void _at_finalize_() override
        {
          _functional_up_time_sec_   = 4;
          _functional_work_time_sec_ = 8;
          _functional_down_time_sec_ = 3;
          return;
        }

        // void _at_run_prepare_() override
        // {
        //   return;
        // }

        // void _at_run_distributable_up_() override
        // {
        //   return;
        // }

        void _at_run_functional_up_() override
        {
          std::cerr << "dummy_use_case: up functional: entering..." << std::endl;
          for (unsigned int tick = 1; tick <= _functional_up_time_sec_; tick++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cerr << "dummy_use_case: up functional: elapsed = "
                      << tick << '/'<< _functional_up_time_sec_ << " sec" << std::endl;
          }
          std::cerr << "dummy_use_case: up functional: exiting." << std::endl;
          return;
        }

        void _at_run_functional_work_loop_iteration_() override
        {
          std::cerr << "dummy_use_case: work functional: entering..." << std::endl;
          for (unsigned int tick = 1; tick <= _functional_work_time_sec_; tick++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cerr << "dummy_use_case: work: elapsed = "
                      << tick << '/'<< _functional_work_time_sec_ << " sec" << std::endl;
          }
          std::cerr << "dummy_use_case: work functional: exiting." << std::endl;
          return;
        }

        void _at_run_functional_down_() override
        {
          std::cerr << "dummy_use_case: down functional: entering..." << std::endl;
          for (unsigned int tick = 1; tick <= _functional_down_time_sec_; tick++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cerr << "dummy_use_case: down: elapsed = "
                      << tick << '/'<< _functional_down_time_sec_ << " sec" << std::endl;
          }
          std::cerr << "dummy_use_case: down functional: exiting." << std::endl;
          return;
        }

        const ::vire::resource::role * _create_minimal_role_() override
        {
          const ::vire::resource::role * r = new ::vire::resource::role();
          return r;
        }

        void set_functional_work_time_sec(const unsigned int fwts_)
        {
          DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already initialized!");
          _functional_work_time_sec_ = fwts_;
          return;
        }

        unsigned int get_functional_work_time_sec() const
        {
          return _functional_work_time_sec_;
        }

        void set_functional_up_time_sec(const unsigned int futs_)
        {
          DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already initialized!");
          _functional_up_time_sec_ = futs_;
          return;
        }

        unsigned int get_functional_up_time_sec() const
        {
          return _functional_up_time_sec_;
        }

        void set_functional_down_time_sec(const unsigned int fdts_)
        {
          DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already initialized!");
          _functional_down_time_sec_ = fdts_;
          return;
        }

        unsigned int get_functional_down_time_sec() const
        {
          return _functional_down_time_sec_;
        }

        unsigned int get_functional_total_time_sec() const
        {
          return get_functional_work_time_sec() +  get_functional_up_time_sec() + get_functional_down_time_sec();
        }

        unsigned int _functional_up_time_sec_   = 5;
        unsigned int _functional_work_time_sec_ = 8;
        unsigned int _functional_down_time_sec_ = 3;

      };

    } // namespace test

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_TEST_DUMMY_USE_CASE_H

// end
