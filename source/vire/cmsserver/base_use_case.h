//! \file  vire/cmsserver/base_use_case.h
//! \brief Vire CMS base use case
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
//
// This file is part of Vire.
//
// Vire is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Vire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_CMSSERVER_BASE_USE_CASE_H
#define VIRE_CMSSERVER_BASE_USE_CASE_H

// Standard Library:
#include <string>
#include <memory>
#include <future>
#include <exception>
#include <map>
#include <mutex>

// Third party:
// - Boost:
#include <boost/noncopyable.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/factory_macros.h>

// This project:
#include <vire/time/utils.h>
#include <vire/resource/role.h>
#include <vire/cmsserver/utils.h>
#include <vire/cmsserver/parametrized_resource_specifications.h>
#include <vire/cmsserver/running.h>
#include <vire/cmsserver/uc_composition.h>
#include <vire/cmsserver/uc_time_constraints.h>
#include <vire/cmsserver/uc_resource_constraints.h>

namespace vire {

  namespace cmsserver {

    class session;

    /// \brief Vire CMS base use case
    //!
    //! Possible derived composite use case:
    //!  - composite_use_case
    //!    - sequencer_use_case [N children]
    //!    - parallelizer_use_case [N children]
    //!    - repeater/loop_use_case [1 child]
    //!  - wait_use_case / wait_condition_use_case
    //!  - lock_use_case
    class base_use_case
      : private boost::noncopyable
      , public datatools::enriched_base
    {
    public:

      /// Dictionary of relative functional device/resource path
      typedef std::map<std::string, std::string> relative_functional_dict_type;

      enum init_flags {
        INIT_DRY_RUN = datatools::bit_mask::bit00 ///< Dry run bit flag
      };
      
      //! Default constructor
      base_use_case(const uint32_t flags_ = 0);

      //! Destructor
      virtual ~base_use_case();

      //! Check dry run mode
      bool is_dry_run() const;

      bool has_resource_constraints() const;

      const uc_resource_constraints & get_resource_constraints() const;

      bool has_time_constraints() const;

      const uc_time_constraints & get_time_constraints() const;

    private:

      virtual std::shared_ptr<uc_resource_constraints> _build_resource_constraints() /* = 0 */;

      virtual std::shared_ptr<uc_time_constraints> _build_time_constraints() /* = 0 */;

    public:
      
      /// Check initialization status
      bool is_initialized() const;

      /// Initialization
      void initialize_simple();

      /// Initialization
      void initialize(const datatools::properties & config_);

      /// Destruction
      void finalize();

      // Run mode:
      
      /// Check run control
      bool has_rc() const;
      
      /// Return a non mutable run control structure:
      const running::run_control & get_rc() const;

      /// Run preparation stage (not in 'dry-run' mode)
      running::run_stage_completion run_prepare();

      /// Run distributable up stage (not in 'dry-run' mode)
      running::run_stage_completion run_distributable_up();

      /// Run functional up stage (not in 'dry-run' mode)
      running::run_stage_completion run_functional_up();

      /// Run functional work stage (not in 'dry-run' mode)
      running::run_stage_completion run_functional_work();

      /// Run functional down stage (not in 'dry-run' mode)
      running::run_stage_completion run_functional_down();

      /// Run distributable down stage (not in 'dry-run' mode)
      running::run_stage_completion run_distributable_down();

      /// Run termination stage (not in 'dry-run' mode)
      running::run_stage_completion run_terminate();

      // Dry-run mode

      /// Generate a session info
      void dry_run_generate(session_info & sinfo_) const;
      
      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_ = datatools::i_tree_dumpable::empty_options()) const;

      /// Check if specifications about relative functional devices and resources are set
      bool has_relative_functional_requirements() const;

      /// Return the specifications about relative functional devices and resources
      virtual const parametrized_resource_specifications &
      get_relative_functional_requirements() const;

      /// Check if functional mount points has been set
      bool has_functional_mount_point(const std::string & name_) const;

      /// Check if all functional mount points have been set
      bool is_completed_functional_mount_points() const;

      /// Set a functional device mount point
      void set_functional_device_mount_point(const std::string & name_,
                                             const std::string & device_path_);

      /// Set a functional resource mount point
      void set_functional_resource_mount_point(const std::string & name_,
                                               const std::string & resource_path_);

     
    public:

      /// Factory method(s) for run/dry-run modes...
      // static use_case_ptr_type _create_use_case_(const std::string & use_case_type_id_,
      //                                           const session_info * sinfo_,
      //                                            session * mother_session_ = nullptr);
      
    private:

      // Initialization/destruction:
    
      /// Return a non mutable run control structure:
      running::run_control & _grab_rc();

      void _basic_initialize_(const datatools::properties & config_);

      void _basic_finalize_();

      virtual void _at_initialize_(const datatools::properties & config_);

      virtual void _at_finalize_();
      
      // Running (not in "dry run" mode):

      virtual void _at_run_prepare_();

      virtual void _at_run_distributable_up_();

      virtual void _at_run_functional_up_();

      virtual void _at_run_functional_work_loop_iteration_();

      virtual void _at_run_functional_down_();

      virtual void _at_run_distributable_down_();

      virtual void _at_run_terminate_();

      /// Generate a session info for this use case (only in "dry-run" mode)
      virtual void _at_dry_run_generate_(session_info & sinfo_) const;

    protected:

      bool _check_run_stop_requested() const;

      void _run_functional_work_loop_status_continue();

      void _run_functional_work_loop_status_stop();

    private:

      // System management:
      bool _initialized_ = false; //!< Initialization flag
      bool _dry_run_     = false; //!< "dry-run" mode flag
      const session * _mother_session_ = nullptr; //!< Not in "dry-run" mode

      // Configuration:

      /// Functional resource mount points
      relative_functional_dict_type _functional_mount_points_;

      /// Composition model 
      std::unique_ptr<uc_composition> _composition_;

      /// Time constraints 
      std::shared_ptr<uc_time_constraints> _time_constraints_;

      /// Resource constraints 
      std::shared_ptr<uc_resource_constraints> _resource_constraints_;
 
      // Run management/control for the session (only if not in 'dry-run' mode)
      // boost::optional<std::size_t> _max_run_functional_loops_;
      std::unique_ptr<running::run_control> _rc_;

      friend class session;
      friend class uc_factory;

      // Factory declaration :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_use_case)

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_BASE_USE_CASE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
