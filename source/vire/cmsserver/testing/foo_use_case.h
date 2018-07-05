// -*- mode: c++; -*-
// dummy_use_case.h

#ifndef VIRE_CMSSERVER_TEST_FOO_USE_CASE_H
#define VIRE_CMSSERVER_TEST_FOO_USE_CASE_H

// Standard library:
#include <thread>
#include <future>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/cmsserver/base_use_case.h>

namespace vire {
  
  namespace cmsserver{

    namespace test{

      class foo_use_case
        : public vire::cmsserver::base_use_case
      {
      private:
       
        void _build_resource_requirements_(parametrised_resource_specifications & param_specs_) const override
        {
          param_specs_.add_device("Board", "HV.PowerSupply.Board");
          param_specs_.add_resource("T",
                                    vire::resource::RESOURCE_CATEGORY_DATAPOINT,
                                    vire::resource::ACCESS_READ_ONLY,
                                    datatools::introspection::DATA_TYPE_DOUBLE,
                                    "temperature");
          param_specs_.add_resource("F",
                                    vire::resource::RESOURCE_CATEGORY_DATAPOINT,
                                    vire::resource::ACCESS_READ_ONLY,
                                    datatools::introspection::DATA_TYPE_DOUBLE,
                                    "frequency");
          return;
        }
        
        void _build_composition_description_(uc_composition_description & compdesc_) const override
        {
          compdesc_.set_scheduling(running::SCHED_PARALLEL);
          compdesc_.add_daughter("Mon1", "DummyUseCaseModel", "A mock daughter use case");
          compdesc_.add_daughter("Mon2", "DummyUseCaseModel", "Another mock daughter use case");
          compdesc_.add_daughter("Mon3", "DummyUseCaseModel", "Yet another mock daughter use case");
          return;
        }

        static std::tuple<double,double> time_abaci(const std::size_t count_, const run_stage_type stage_)
        {
          std::tuple<double,double> min_max;
          if (stage_ == running::RUN_STAGE_SYSTEM_PREPARING) {
            return std::make_tuple(std::sqrt(count_) * 10.25 * CLHEP::second,
                                   std::sqrt(count_) * 18.86 * CLHEP::second);
          }
          if (stage_ == running::RUN_STAGE_SYSTEM_TERMINATING) {
            return std::make_tuple(std::sqrt(count_) * 5.15 * CLHEP::second,
                                   std::sqrt(count_) * 8.36 * CLHEP::second);
          }
          if (stage_ == running::RUN_STAGE_FUNCTIONAL_UP_RUNNING) {
            return std::make_tuple(std::sqrt(count_) * 1.15 * CLHEP::second,
                                   std::sqrt(count_) * 1.46 * CLHEP::second);
          }
          if (stage_ == running::RUN_STAGE_FUNCTIONAL_DOWN_RUNNING) {
            return std::make_tuple(std::sqrt(count_) * 0.95 * CLHEP::second,
                                   std::sqrt(count_) * 2.65 * CLHEP::second);
          }
          if (stage_ == running::RUN_STAGE_FUNCTIONAL_RUN_STAGE_FUNCTIONAL_WORK_RUNNING_RUNNING) {
            return std::make_tuple(std::sqrt(count_) * 0.95 * CLHEP::second,
                                   datatools::invalid_real());
          }
          return std::make_tuple(datatools::invalid_real(),
                                 datatools::invalid_real());
        }
        
        std::shared_ptr<uc_time_constraints> base_use_case::_build_time_constraints_() const override
        {
          std::shared_ptr<uc_time_constraints> uctc = std::make_shared<uc_time_constraints>();
          for (const auto stage : uc_time_constraints::supported_stages()) {
            std::tuple<double,double> min_max = time_abaci(_load_counter_, stage);
            std::size_t min_millis = static_cast<std::size_t>(std::get<0>(min_max) / CLHEP::millisecond);
            std::size_t max_millis = static_cast<std::size_t>(std::get<1>(min_max) / CLHEP::millisecond);
            uctc.add_constraint(stage,
                                vire::cmsserver::duration_interval::make_min_max(boost::posix_time::seconds(min_millis),
                                                                                 boost::posix_time::seconds(max_millis)));
          }
          return uctc;
        }

      public:
        
        //! Constructor
        foo_use_case()
        {
          return;
        }

        //! Destructor
        virtual ~foo_use_case()
        {
          return;
        }

        void _at_run_prepare_() override
        {
          std::this_thread::sleep_for(std::chrono::seconds(1));
          return;
        }

        void _at_run_terminate_() override
        {
          std::this_thread::sleep_for(std::chrono::seconds(1));
          return;
        }

        void _at_run_up_() override
        {
          std::this_thread::sleep_for(std::chrono::seconds(1));
          return;
        }

        void _at_run_down_() override
        {
          std::this_thread::sleep_for(std::chrono::seconds(1));
          return;
        }
        
        running::run_work_loop_status_type _at_run_work_loop_iteration_() override
        {
          running::run_work_loop_status_type ret = running::RUN_WORK_LOOP_STOP;
          std::this_thread::sleep_for(std::chrono::seconds(1));
          return ret;
        }

        void _at_initialize_(const datatools::properties & config_) override
        {
          if (config_.has_key("load_counter")) {
            int nloads = config_.fetch_positive_integer("load_counter");
            DT_THROW_IF(nloads < 1, std::logic_error,
                        std::logic_error,
                        "Invalid matching scheduling mode at composition merging!");
            _load_counter_ = nloads;
          }
          return;
        }

        void _at_finalize_() override
        {
          return;
        }

      private:

        std::size_t _load_counter_ = 1;
          
        VIRE_USE_CASE_REGISTRATION_INTERFACE(foo_use_case)
        
      };
      
      VIRE_USE_CASE_REGISTRATION_IMPLEMENT(foo_use_case, "vire::cmsserver::test::foo_use_case")
      
    } // namespace test

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_TEST_FOO_USE_CASE_H

// end
