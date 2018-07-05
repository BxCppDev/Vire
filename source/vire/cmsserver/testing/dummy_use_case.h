// -*- mode: c++; -*-
// dummy_use_case.h

#ifndef VIRE_CMSSERVER_TEST_DUMMY_USE_CASE_H
#define VIRE_CMSSERVER_TEST_DUMMY_USE_CASE_H

// Standard library:
#include <thread>
#include <memory>

// This project:
#include <vire/cmsserver/base_use_case.h>
#include <vire/cmsserver/uc_time_constraints.h>

namespace vire {
  namespace cmsserver{
    namespace test{

      class dummy_use_case
        : public vire::cmsserver::base_use_case
      {
      public:
 
        void _build_resource_requirements_(parametrised_resource_specifications & specs_) const override
        {
          specs_.add_device("PS", "HV.PowerSupply.Mod2817A", vire::device::TYPE_CRATE);
          specs_.add_device("Probe", "Probe42", vire::device::TYPE_GENERIC);
          specs_.add_resource("T1", 
                              vire::resource::RESOURCE_CATEGORY_DATAPOINT,
                              vire::resource::ACCESS_READ_ONLY,
                              datatools::introspection::DATA_TYPE_DOUBLE,
                              "temperature");
          specs_.add_resource("P1",
                              vire::resource::RESOURCE_CATEGORY_DATAPOINT,
                              vire::resource::ACCESS_READ_ONLY,
                              datatools::introspection::DATA_TYPE_DOUBLE,
                              "pressure");
          specs_.add_resource("P2",
                              vire::resource::RESOURCE_CATEGORY_DATAPOINT,
                              vire::resource::ACCESS_READ_ONLY,
                              datatools::introspection::DATA_TYPE_DOUBLE,
                              "pressure");
          return;
        }
                
        //! Default constructor
        dummy_use_case(const unsigned int futs_ = 4,
                       const unsigned int fwts_ = 8,
                       const unsigned int fdts_ = 3)
          : _up_time_sec_(futs_)
          , _work_time_sec_(fwts_)
          , _down_time_sec_(fdts_)
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
            _up_time_sec_ = futs;
          }

          if (config_.has_key("functional_down_time_sec")) {
            unsigned int fdts = config_.fetch_positive_integer("functional_down_time_sec");
            _down_time_sec_ = fdts;
          }

          if (config_.has_key("functional_work_time_sec")) {
            unsigned int fwts = config_.fetch_positive_integer("functional_work_time_sec");
            _work_time_sec_ = fwts;
          }

          return;
        }

        void _at_finalize_() override
        {
          _up_time_sec_   = 4;
          _down_time_sec_ = 3;
          _work_time_sec_ = 8;
          return;
        }

        // void _at_run_prepare_() override
        // {
        //   return;
        // }

        // void _at_run_terminate_() override
        // {
        //   return;
        // }

        void _at_run_up_() override
        {
          std::cerr << "dummy_use_case: up functional: entering..." << std::endl;
          for (unsigned int tick = 1; tick <= _up_time_sec_; tick++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cerr << "dummy_use_case: up functional: elapsed = "
                      << tick << '/'<< _up_time_sec_ << " sec" << std::endl;
          }
          std::cerr << "dummy_use_case: up functional: exiting." << std::endl;
          return;
        }

        running::run_work_loop_status_type _at_run_work_loop_iteration_() override
        {
          running::run_work_loop_status_type ret = running::RUN_WORK_LOOP_STOP;
          std::cerr << "dummy_use_case: work functional: entering..." << std::endl;
          for (unsigned int tick = 1; tick <= _work_time_sec_; tick++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cerr << "dummy_use_case: work: elapsed = "
                      << tick << '/'<< _work_time_sec_ << " sec" << std::endl;
          }
          std::cerr << "dummy_use_case: work functional: exiting." << std::endl;
          return ret;
        }

        void _at_run_down_() override
        {
          std::cerr << "dummy_use_case: down functional: entering..." << std::endl;
          for (unsigned int tick = 1; tick <= _down_time_sec_; tick++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cerr << "dummy_use_case: down: elapsed = "
                      << tick << '/'<< _down_time_sec_ << " sec" << std::endl;
          }
          std::cerr << "dummy_use_case: down functional: exiting." << std::endl;
          return;
        }

        void set_work_time_sec(const unsigned int fwts_)
        {
          DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already initialized!");
          _work_time_sec_ = fwts_;
          return;
        }

        unsigned int get_work_time_sec() const
        {
          return _work_time_sec_;
        }

        void set_up_time_sec(const unsigned int futs_)
        {
          DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already initialized!");
          _up_time_sec_ = futs_;
          return;
        }

        unsigned int get_up_time_sec() const
        {
          return _up_time_sec_;
        }

        void set_down_time_sec(const unsigned int fdts_)
        {
          DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already initialized!");
          _down_time_sec_ = fdts_;
          return;
        }

        unsigned int get_down_time_sec() const
        {
          return _down_time_sec_;
        }

        unsigned int get_total_time_sec() const
        {
          return get_work_time_sec() +  get_up_time_sec() + get_down_time_sec();
        }

        virtual std::shared_ptr<uc_time_constraints> _build_time_constraints_() const override
        {
          std::shared_ptr<uc_time_constraints> p_utc = std::make_shared<uc_time_constraints>();

          boost::posix_time::time_duration td_min = boost::posix_time::seconds(_up_time_sec_);
          boost::posix_time::time_duration td_max = boost::posix_time::seconds(_up_time_sec_);
          p_utc->add_constraint(vire::cmsserver::running::RUN_STAGE_FUNCTIONAL_UP_RUNNING,
                                vire::time::duration_interval(td_min, td_max));
         
          td_min = boost::posix_time::seconds(_down_time_sec_);
          td_max = boost::posix_time::seconds(_down_time_sec_);
          p_utc->add_constraint(vire::cmsserver::running::RUN_STAGE_FUNCTIONAL_DOWN_RUNNING,
                                vire::time::duration_interval(td_min, td_max));
           
          td_min = boost::posix_time::seconds(_work_time_sec_);
          td_max = boost::posix_time::seconds(_work_time_sec_);
          p_utc->add_constraint(vire::cmsserver::running::RUN_STAGE_FUNCTIONAL_WORK_RUNNING,
                                vire::time::duration_interval(td_min, td_max));
          
          return p_utc;
        }
        
      private:
        
        unsigned int _up_time_sec_   = 5;
        unsigned int _down_time_sec_ = 3;
        unsigned int _work_time_sec_ = 8;
          
        VIRE_USE_CASE_REGISTRATION_INTERFACE(dummy_use_case)
        
      };
      
      VIRE_USE_CASE_REGISTRATION_IMPLEMENT(dummy_use_case, "vire::cmsserver::test::dummy_use_case")

    } // namespace test

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_TEST_DUMMY_USE_CASE_H

// end
