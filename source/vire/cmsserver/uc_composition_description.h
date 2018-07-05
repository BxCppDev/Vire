//! \file  vire/cmsserver/uc_composition_description.h
//! \brief Vire CMS use case composition
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

#ifndef VIRE_CMSSERVER_UC_COMPOSITION_DESCRIPTION_H
#define VIRE_CMSSERVER_UC_COMPOSITION_DESCRIPTION_H

// Standard Library:
#include <string>
#include <map>
#include <set>
#include <list>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/bit_mask.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/i_serializable.h>

// This project:
#include <vire/running/scheduling.h>

namespace vire {

  namespace cmsserver {

    /// \brief Description of the composition of an use case in terms of daughter use cases
    class uc_composition_description
      : public datatools::i_tree_dumpable
      , public datatools::i_serializable
    {

    public:

      static const vire::running::scheduling_type DEFAULT_SCHEDULING = vire::running::SCHED_PARALLEL;
      
      /// \brief Daughter use case descriptive record
      struct daughter_entry {
        std::string model_id;    //!< Identifier of the daughter use case model
        std::string description; //!< Description of the daughter use case instance within its parent
        int         serial_rank = -1; 
        bool        from_setup = false; //!< Flag to identify a 'setup' daughter use case from a 'type' daughter use case
        //! Serialization interface
        BOOST_SERIALIZATION_BASIC_DECLARATION();
      };

      // /// \brief Check daughter's name predicate
      // struct check_daughter_name_predicate {
      //   check_daughter_name_predicate(const std::string & name_)
      //     : _name_(name_) {}
      //   bool operator()(const daughter_entry & entry_) const {
      //     return _name_ == entry_.name;
      //   }
      // private:
      //   const std::string & _name_; ///< Name to be checked
      // };    

      /// \brief Dictionary of daughter use case descriptive entries
      typedef std::map<std::string, daughter_entry> daughters_coll_type;

      /// Check if the daughter use case is added by the user setup
      bool is_from_setup() const;

      /// Check if the scheduling model is set
      bool has_scheduling() const;
      
      /// Set the scheduling model
      void set_scheduling(const vire::running::scheduling_type sched_);

      /// Return the scheduling model
      vire::running::scheduling_type get_scheduling() const;

      /// Check if the scheduling model is "parallel"
      bool is_scheduling_parallel() const;

      /// Check if the scheduling model is "serial"
      bool is_scheduling_serial() const;

      /// Check if the scheduling model is "repeat"
      bool is_scheduling_repeat() const;

      /// Check if the number of repetitions for the "repeat" scheduling model is set
      bool has_repeat_number_of_repetitions() const;

      /// Set the number of repetitions for the "repeat" scheduling model
      void set_repeat_number_of_repetitions(const std::size_t);
      
      /// Return the number of repetitions for the "repeat" scheduling model
      std::size_t get_repeat_number_of_repetitions() const;

      /// Default constructor
      uc_composition_description();

      /// Destructor
      virtual ~uc_composition_description();

      bool is_empty() const;
      
      /// Return the number of daughters
      std::size_t get_number_of_daughters() const;

      /// Check if the compoisiton contains some daughter use case(s)
      bool has_daughters() const;

      /// Check if the compoisiton contains a daughter use case with given name
      bool has_daughter(const std::string & name_) const;

      /// Build the set of daughters' names
      void build_daughter_names(std::set<std::string> & names_) const;

      /// Build the list of daughters' names (ordered if needed)
      void build_daughter_names(std::vector<std::string> & names_) const;

      /// Build the ordered list of daughters' names in serial scheduling
      void build_serial_daughter_names(std::vector<std::string> & names_) const;
      
      /// Add a daughter use case with given name
      void add_daughter(const std::string & name_,
                        const std::string & model_id_,
                        const std::string & description_ = "",
                        const bool from_setup_ = false);

      /// Remove a daughter use case with given name
      void remove_daughter(const std::string & name_);

      /// Return a daughter use case with given name
      const daughter_entry & get_daughter(const std::string & name_) const;
       
      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_ = empty_options()) const;

      /// Configure the composition from a container of properties
      void configure(const datatools::properties & config_);

      bool can_merge(vire::running::scheduling_type scheduling_) const;
      
      void merge(const uc_composition_description &, bool from_setup_ = false);
     
      /// \brief Flags for export to a container of properties
      enum export_config_flags {
        EXPORT_CONFIG_CLEAR             = datatools::bit_mask::bit00,
        EXPORT_CONFIG_LOCKED            = datatools::bit_mask::bit01,
        EXPORT_CONFIG_SCHEDULING        = datatools::bit_mask::bit02,
        EXPORT_CONFIG_REPETITIONS       = datatools::bit_mask::bit03,
        EXPORT_CONFIG_DAUGHTERS         = datatools::bit_mask::bit04,
        EXPORT_CONFIG_DEFAULT           = (EXPORT_CONFIG_CLEAR
                                           | EXPORT_CONFIG_LOCKED
                                           | EXPORT_CONFIG_SCHEDULING
                                           | EXPORT_CONFIG_REPETITIONS
                                           | EXPORT_CONFIG_DAUGHTERS),
        EXPORT_CONFIG_LAST             = EXPORT_CONFIG_DAUGHTERS
      };
      
      /// Export to a container of properties
      void export_to_config(datatools::properties & config_,
                            const uint32_t flags_ = EXPORT_CONFIG_DEFAULT,
                            const std::string & prefix_ = "") const;
       
      /// Check if the container of resource specifications is locked
      bool is_locked() const;

      /// Lock the container of resource specifications
      void lock();
     
      /// Reset the container of composition
      void reset();

    private:

      /// Load the composition
      void _load_(const datatools::properties & config_);
    
      void _at_lock_();
      
    private:

      // Management:
      bool                _locked_ = false; ///< Lock flag

      // Configuration:
      vire::running::scheduling_type _scheduling_ = vire::running::SCHED_UNDEF;  ///< Scheduling model
      std::size_t         _number_of_repetitions_ = 0; ///< Number of repetitions (only in ``SCHED_REPEAT`` scheduling model)
      // parallel_concurrent_mounting_policy:
        
      daughters_coll_type _daughters_;                 ///< Dictionary of daughter use case descriptions

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION()
      
    };
    
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_COMPOSITION_DESCRIPTION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
