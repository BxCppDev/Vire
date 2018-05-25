// -*- mode: c++; -*-
// dummy_use_case.h

#ifndef VIRE_CMSSERVER_TEST_FOO_USE_CASE_H
#define VIRE_CMSSERVER_TEST_FOO_USE_CASE_H

// Standard library:
#include <thread>
#include <future>

// This project:
#include <vire/cmsserver/base_use_case.h>

namespace vire {
  
  namespace cmsserver{

    namespace test{

      class foo_use_case
        : public vire::cmsserver::base_use_case
      {
      protected:
        
        static void build_relative_functional_specifications(parametrized_resource_specifications & prs_)
        {
          prs_.add_device("john", "HV.PowerSupply.Board");
          prs_.add_resource("v1", "unit_dimension=temperature");
          prs_.add_resource("s2", "unit_dimension=speed");
          return;
        }
       
        static const parametrized_resource_specifications & functional_resource_specifications()
        {
          // Static singleton:
          static std::unique_ptr<parametrized_resource_specifications> _prs;
          if (_prs.get() == nullptr) {
            _prs.reset(new parametrized_resource_specifications);
            build_relative_functional_specifications(*_prs.get());
          }
          return *_prs.get();
        }
        
        virtual const parametrized_resource_specifications & get_functional_resource_specifications() const
        {
          return foo_use_case::functional_resource_specifications();
        }
       
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

        void _at_initialize_(const datatools::properties & config_) override
        {
 
          return;
        }

        void _at_finalize_() override
        {
	  return;
        }

      };
 
    } // namespace test

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_TEST_FOO_USE_CASE_H

// end
