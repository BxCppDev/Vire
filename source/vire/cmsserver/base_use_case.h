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
#include <tuple>

// Third party:
// - Boost:
#include <boost/noncopyable.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/factory_macros.h>

// This project:
#include <vire/time/utils.h>
#include <vire/device/manager.h>
#include <vire/cmsserver/utils.h>
#include <vire/cmsserver/uc_parametrised_resource_specifications.h>
#include <vire/cmsserver/uc_functional_resource_specifications.h>
#include <vire/cmsserver/running.h>
#include <vire/cmsserver/uc_model_description.h>
#include <vire/cmsserver/uc_time_constraints.h>
#include <vire/cmsserver/uc_resource_constraints.h>
#include <vire/cmsserver/uc_utils.h>
#include <vire/cmsserver/uc_mounting_table.h>
#include <vire/cmsserver/uc_scope_resources.h>

namespace vire {
  namespace device {
    class manager;
  } // namespace device
} // namespace vire
  
namespace vire {
  namespace cmsserver { 
    class base_use_case;
  } // namespace cmsserver
} // namespace vire

#define VIRE_USE_CASE_REGISTRATION_INTERFACE(UC_CLASS_NAME)             \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::cmsserver::base_use_case, UC_CLASS_NAME) \
  public:                                                               \
  virtual std::string uc_class_id() const;                              \
  /**/

#define VIRE_USE_CASE_REGISTRATION_IMPLEMENT(UC_CLASS_NAME,UC_ID)       \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION (::vire::cmsserver::base_use_case,UC_CLASS_NAME,UC_ID) \
  std::string UC_CLASS_NAME::uc_class_id() const {return UC_ID;}
/**/

namespace vire {

  namespace cmsserver {

    class uc_factory;

    /// \brief Vire CMS base use case
    //!
    //! Possible derived composite use case:
    //!  - composite_use_case
    //!    - sequencer_use_case [N children]
    //!    - parallelizer_use_case [N children]
    //!    - repeater/loop_use_case [1 child]
    //!  - wait_use_case / wait_condition_use_case
    //!  - lock_use_case
    //!
    //! \code
    //!             start                                         stop
    //! --------------[---------------------------------------------[-----------------------------> time
    //!               :-------------------------------------------->: use case run duration
    //!  +-------+--------------------------------------------------+---------+------+
    //!  |prepare|////|                                             |terminate|//////| System
    //!  +-------+--------------------------------------- ----------+---------+------+
    //!  :------>: preparation duration                             :-------->: termination duration
    //!               +---+--+--------------------+----+----+-------+
    //!               |   |//|       work         |////|    |///////|
    //!               |up |//|loop -> loop -> loop|////|down|///////| Functional resources
    //!               |   |//|                    |////|    |///////|
    //!               +---+--+--------------------+----+----+-------+
    //!               :-->: up duration           :    :--->: down duration    
    //!                      :------------------->: work duration
    //! \endcode
    //!
    class base_use_case
      : private boost::noncopyable
      , public datatools::enriched_base
    {
    public:

      /// \brief Build mode
      ///
      /// The 'dry-run' mode is used to construct the use case in such a way
      /// all time and resource constraints can be computed and the structure
      /// of a running session can be created.
      ///
      /// The 'run' mode is used to construct the use case in such a way
      /// it can be effectively run by a real-time session, using the informations created
      /// by a former 'dry-run' mode.
      ///
      /// Possible run paths are:
      /// \code
      ///                 ,-->- RUN_MODE_DRYRUN -->--.
      ///                /                            \
      ///  x ----> RUN_MODE_UNDEF                RUN_MODE_DEAD
      ///                \                            /
      ///                 `-->-- RUN_MODE_RUN ---->--'
      /// \endcode
      ///
      enum run_mode_type {
        RUN_MODE_UNDEF  = 0, ///< Mode before entering effective running mode
        RUN_MODE_DRYRUN = 1, ///< Dry-run mode (defines structure and constraints)
        RUN_MODE_RUN    = 2, ///< Real-time run mode
        RUN_MODE_DEAD   = 3  ///< Dead use case
      };
      
      static std::string run_mode_to_label(const run_mode_type);

      /// \brief Use case's construction states
      enum construction_stage_type {
        CONSTRUCT_UNDEF        = 0, ///< Undefines state
        CONSTRUCT_READY        = 1, ///< Ready for construction (run mode is set)
        CONSTRUCT_CONFIG_SETUP = 2, ///< Main configuration setup is done
        CONSTRUCT_CONFIG_COMPO = 3, ///< Configuration of the composition is done
        CONSTRUCT_COMPOSED     = 4, ///< Use case is compsed (daughter use cases are instantiated)
        CONSTRUCT_SCOPE_SPECS  = 5, ///< Scope resources specifications are set
        CONSTRUCT_DISTRIBUTABLE_MOUNT_SPECS  = 6, ///< Distributable resources mounting table is set
        CONSTRUCT_FUNCTIONAL_SPECS = 8, ///< Functional resources mounting table is set

        CONSTRUCT_RELATIVE_SCOPE = 10, ///< Relative scope resources are effectively computed and checked
        
        CONSTRUCT_MOUNTED      = 20, ///< Resources are effectively mounted
        CONSTRUCT_CONSTRAINED  = 50, ///< Resources/time constraints are effectively computed
        CONSTRUCT_CONFIG_SETUP_POST_COMPO = 100, ///< Post-construction is done 
      };
       
      static std::string to_string(const construction_stage_type);

      /// Dictionary of relative device/resource path
      typedef std::map<std::string, std::string> mount_point_dict_type;

      //>>> factory scope
      
      /// Daughter use case instantiation record
      struct composition_entry_type {
        std::string name;
        std::string description;
        int         serial_rank = -1;
        std::shared_ptr<base_use_case> instance;
        bool        from_setup = false;
      };

      /// Dictionary of daughter use cases:
      typedef std::map<std::string, composition_entry_type> composition_dict_type;
      
      //<<< factory scope
  
      //! Default constructor
      base_use_case();

      //! Destructor
      virtual ~base_use_case();

      // +----------------------------------------+
      // | GENERAL METHODS                        |
      // +----------------------------------------+
  
      //! Use case allows to optimize run system prepare/terminate calls
      virtual bool is_run_system_optimizable() const; 

      //! Use case 
      virtual bool is_repeatable() const; 
      
      //! Set the run mode
      void set_run_mode(const run_mode_type);
      
      //! Reset the run mode
      void reset_run_mode();
      
      //! Check dry run mode
      bool is_dry_run_mode() const;

      //! Check run mode
      bool is_run_mode() const;

      /// Return the construction stage
      construction_stage_type get_construction_stage() const;

      /// Return the construction context
      const uc_construction_context_type & get_cc() const;

      /// Set the construction context
      void set_cc(const uc_construction_context_type & cc_);
      
      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

      // +----------------------------------------+
      // | CONSTRUCTION METHODS                   |
      // +----------------------------------------+
      
      /// General configuration of the use case
      void configure_setup(const datatools::properties & config_);
      
      /// Return the use case's general configuration
      const datatools::properties & get_configuration() const;

      /// Unset the use case's general configuration 
      void deconfigure_setup();

    private:

      /// Callback method to configure the use case
      virtual void _at_configure_setup_(const datatools::properties & config_);

      /// Callback method to deconfigure the use case
      virtual void _at_deconfigure_setup_();

    public:

      /// Configure the composition of the use case
      void configure_composition(const uc_composition_description & setup_comp_desc_);

      /// Deconfigure the composition of the use case
      void deconfigure_composition();

    private:
      
      /// Callback method to build the type's composition description
      virtual void _build_composition_description_(uc_composition_description & type_comp_desc_) const;
      
      /// Merge a setup composition to the type's composition description 
      void _merge_composition_description_setup_(const uc_composition_description & setup_comp_desc_);

    public:
      
      //! Return the description of the composition
      const uc_composition_description & get_composition_description() const;

    public:

      //! Build composition
      void build_composition();
      
      void post_composition_configure_setup();
      
      void post_composition_deconfigure_setup();
      
    private:
      
      /// Callback method to finalize the configuration of the use case after composition
      virtual void _at_post_composition_configure_setup_(const datatools::properties & config_);

      /// Callback method to deconfigure of the use case before destroying the composition
      virtual void _at_post_composition_deconfigure_setup_();

    public:
      
      //! Build resource requirements
      void build_scope_requirements();
      
      /// Return specifications about relative devices and resources that must be mounted
      const uc_parametrised_resource_specifications & get_scope_requirements() const;

    private:
      
      /// Build the static specifications about relative devices and resources
      virtual void _build_scope_requirements_(uc_parametrised_resource_specifications &) const;

    public:
      
      //! Build local mounting table of the distributable resources/devices (for daughter use cases)
      void build_distributable_mounting_table();
     
      /// Return specifications about relative devices and resources that must be mounted
      const uc_resource_mounting_table & get_distributable_mounting_table() const;

    private:

      //! Build distributable mounting table
      void _build_distributable_mounting_table_(uc_resource_mounting_table & mtab_);

      //! Additional completion of the distributable mounting table from setup composition (if any)
      void _setup_distributable_mounting_table_();

      //! Check the consistence of the distributable mounting table
      void _check_distributable_mounting_table_();

    public:

      /**
       *
       *
       * \code
       * +-------------+---------------------------------------+-----------------------+   
       * |   .     to  |               Device                  |                       |
       * |       .     +-----------+------------+--------------+       Resource        |
       * | from      . |   plain   | sub-device | sub-resource |                       |
       * +-------------+-----------+------------+--------------+-----------------------+
       * |             |           |            |              |                       |
       * |             |           |            |              |                       |
       * |    Device   |           |            |      x       |           x           |
       * |             |           |            |              |                       |
       * +-------------+-----------+------------+--------------+-----------------------+
       * |             |           |            |              |                       |
       * |             |           |            |              |                       |
       * |   Resource  |     x     |     x      |              |                       |
       * |             |           |            |              |                       |
       * +-------------+-----------+------------+--------------+-----------------------+
       * \endcode
       *
       */
      std::tuple<bool,std::string> match_mount(const uc_parametrised_resource_specifications::resource_spec_entry & daughter_spec_,
                                               const uc_parametrised_resource_specifications::resource_spec_entry & parent_spec_,
                                               const vire::utility::relative_path & relpath_) const;

    public:

      /// Build the functional resources requirements
      void build_functional_requirements();

      void add_relative_functional(const std::string & key_,
                                   const vire::utility::relative_path & rp_);

      void add_absolute_functional(const std::string & key_,
                                   const vire::utility::path & absolute_path_);

      /// Return the functional resource requirements
      const uc_functional_resource_specifications & get_functional_requirements() const;
      
    private:

      /// Call back to build the functional resource requirements from the scope resource/device requirements
      virtual void _build_functional_requirements_(uc_functional_resource_specifications  & fund_res_specs_);

      void _check_functional_requirements_();







      
      
      public:
      
      void build_scope_resources();

    private:

      // void _compute_list_of_distributables_();

      // //! Callback to build the list of functional resources
      // virtual void _build_list_of_functional_resources_(uc_scope_resources &);

      //  void _build_list_of_scope_resources_();











      
      
    private:

      /// Callback method to build a functional resource constraints object associated to the use case (only in "dry-run" mode)
      virtual std::shared_ptr<uc_resource_constraints> _build_resource_constraints_() const;

      /// Callback method to build a time constraints object associated to the use case (only in "dry-run" mode)
      virtual std::shared_ptr<uc_time_constraints> _build_time_constraints_() const;

    public:
 
      //! Check if resource constraints are computed and set
      bool has_resource_constraints() const;

      //! Return resource constraints
      const uc_resource_constraints & get_resource_constraints() const;

      //! Check if time constraints are computed and set
      bool has_time_constraints() const;

      //! Return time constraints
      const uc_time_constraints & get_time_constraints() const;

      /// Check if mount points has been set
      bool has_mount_point(const std::string & name_) const;

      /// Check if all mount points have been set
      bool is_completed_mount_points() const;

      /// Set a device mount point
      void set_device_mount_point(const std::string & name_,
                                  const std::string & device_path_);
      
      /// Set a resource mount point
      void set_resource_mount_point(const std::string & name_,
                                    const std::string & resource_path_);
            

      // +----------------------------------------+
      // | RUN MODE SPECIFIC METHODS              |
      // +----------------------------------------+
      
      /// Check run control ("run" mode only)
      bool has_rc() const;
      
      /// Return a non mutable run control structure ("run" mode only)
      const running::run_control & get_rc() const;

      /// Set a run control ("run" mode only)
      void set_rc(const std::shared_ptr<running::run_control> & rc_);
     
    private:
    
      /// Return a non mutable run control structure:
      running::run_control & _grab_rc_();


    public:
      
      /// Run preparation stage (not in 'dry-run' mode)
      running::run_stage_completion run_prepare();

      /// Run termination stage (not in 'dry-run' mode)
      running::run_stage_completion run_terminate();

      /// Run functional up stage (not in 'dry-run' mode)
      running::run_stage_completion run_up();

      /// Run functional down stage (not in 'dry-run' mode)
      running::run_stage_completion run_down();

      /// Run functional work stage (not in 'dry-run' mode)
      running::run_stage_completion run_work();

      
    private:
       
      // Running (only in "run" mode):

      /// System preparation action (default implementation does nothing and returns success)
      virtual void _at_run_prepare_();

      /// System terminate action (default implementation does nothing and returns success)
      virtual void _at_run_terminate_();

      /// Automatic working action on functional resources at the beginning of the work stage (default implementation does nothing)
      virtual void _at_run_up_();

      /// Automatic working action on functional resources at the end of the work stage (default implementation does nothing)
      virtual void _at_run_down_();

      /// Action working loop on functional resources during the work stage (default implementation does nothing and breaks the working loop successfully)
      virtual running::run_work_loop_status_type
      _at_run_work_loop_iteration_();


      // +----------------------------------------+
      // | ATTRIBUTES                             |
      // +----------------------------------------+
      
    private:

      // System management:
      construction_stage_type _construct_stage_ = CONSTRUCT_UNDEF; //!< Construction stage
      run_mode_type           _run_mode_        = RUN_MODE_UNDEF;  //!< Run mode flag

      // +-------------------------------+
      // | Configuration and description |
      // +-------------------------------+

      // std::unique_ptr<uc_construction_context_type> _construction_context_ptr_;

      uc_construction_context_type _cc_;
      
      /// General configuration:
      datatools::properties      _configuration_;
      
      /// Composition description (description of daughter sub-use cases)
      uc_composition_description _composition_description_;
     
      /// Composition (daughter sub-use case instances)
      composition_dict_type      _composition_;
      
      /// Description of required mount points for scope resources/devices :     
      uc_parametrised_resource_specifications _scope_requirements_;
         
      /// Distributable resource mounting table
      uc_resource_mounting_table _distributable_mounting_table_;
      
      /// Description of functional resources:     
      uc_functional_resource_specifications _functional_requirements_;



      
      // /// Lists of relative and absolute scope resources
      // std::shared_ptr<uc_scope_resources> _scope_resources_ptr_;



      
      /// Dictionary of resource mount points
      mount_point_dict_type _mount_points_;

      /// Time constraints 
      std::shared_ptr<uc_time_constraints> _time_constraints_;

      /// Resource constraints 
      std::shared_ptr<uc_resource_constraints> _resource_constraints_;
 
      /// Run management/control for the session (only if not in 'dry-run' mode)
      std::shared_ptr<running::run_control>    _rc_;
      
      friend class uc_factory;

      // Factory declaration :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_use_case)
      VIRE_USE_CASE_REGISTRATION_INTERFACE(base_use_case)
      
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_BASE_USE_CASE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
