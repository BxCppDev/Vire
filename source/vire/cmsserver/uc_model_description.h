//! \file  vire/cmsserver/uc_model_description.h
//! \brief The description of a use case for the CMS server
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

#ifndef VIRE_CMSSERVER_UC_MODEL_DESCRIPTION_H
#define VIRE_CMSSERVER_UC_MODEL_DESCRIPTION_H

// Standard Library:
#include <string>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/optional.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/bit_mask.h>
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/cmsserver/utils.h>
#include <vire/cmsserver/uc_composition_description.h>

namespace vire {

  namespace cmsserver {

    /// \brief Setup description for a CMS use case
    class uc_model_description
      : public datatools::i_tree_dumpable
      , public datatools::i_serializable
    {
    public:

      /// Default constructor
      uc_model_description();

      /// Destructor
      virtual ~uc_model_description();

      /// Check if the name is set
      bool has_name() const;

      /// Set the name
      void set_name(const std::string & name_);

      /// Return the name
      const std::string & get_name() const;

      /// Check if the description is set
      bool has_description() const;

      /// Set the description
      void set_description(const std::string & description_);

      /// Return the description
      const std::string & get_description() const;

      /// Check if the use case type identifier is set
      bool has_use_case_type_id() const;

      /// Set the use case identifier
      void set_use_case_type_id(const std::string & use_case_type_id_);

      /// Return the use case identifier
      const std::string & get_use_case_type_id() const;

      /// Check if the use case configuration path is set
      bool has_use_case_config_path() const;

      /// Set the use case configuration path
      void set_use_case_config_path(const std::string & use_case_config_path_);

      /// Return the use case configuration path (path, URN...)
      const std::string & get_use_case_config_path() const;

      /// Check if the explicit use case configuration is set
      bool has_use_case_config() const;

      /// Return the explicit use case configuration
      const datatools::properties & get_use_case_config() const;

      /// Set the explicit use case configuration
      void set_use_case_config(const datatools::properties &);

      /// Reset the explicit use case configuration
      void reset_use_case_config();

      /// Check if the composition is set
      bool has_composition_description() const;

      /// Set the composition
      void set_composition_description(const uc_composition_description &);
      
      /// Return the composition
      const uc_composition_description & get_composition_description() const;    

      /// Return the composition
      uc_composition_description & grab_composition_description();    

      /// Use case model is a leaf (no daughter use cases)
      bool is_leaf() const;

      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;
      
      /// Configuration
      void configure();

      /// Configuration
      void configure(const datatools::properties & config_);

      /// \brief Flags for export to a container of properties
      enum export_config_flags {
        EXPORT_CONFIG_CLEAR             = datatools::bit_mask::bit00,
        EXPORT_CONFIG_NAME              = datatools::bit_mask::bit01,
        EXPORT_CONFIG_TYPE_ID           = datatools::bit_mask::bit02,
        EXPORT_CONFIG_DESCRIPTION       = datatools::bit_mask::bit03,
        EXPORT_CONFIG_CONFIG_PATH       = datatools::bit_mask::bit04,
        EXPORT_CONFIG_CONFIG            = datatools::bit_mask::bit05,
        EXPORT_CONFIG_COMPOSITION       = datatools::bit_mask::bit06,
        EXPORT_CONFIG_DEFAULT           = (EXPORT_CONFIG_CLEAR
                                           | EXPORT_CONFIG_NAME
                                           | EXPORT_CONFIG_TYPE_ID
                                           | EXPORT_CONFIG_DESCRIPTION
                                           | EXPORT_CONFIG_CONFIG_PATH
                                           | EXPORT_CONFIG_CONFIG
                                           | EXPORT_CONFIG_COMPOSITION),
        EXPORT_CONFIG_LAST             = EXPORT_CONFIG_COMPOSITION
      };
      
      /// Export to a container of properties
      void export_to_config(datatools::properties & config_,
                            const uint32_t flags_ = EXPORT_CONFIG_DEFAULT,
                            const std::string & prefix_ = "") const;
      
      /// Reset/invalidate the session info
      void reset();

    private:

      /// Configuration
      void _configure_(const datatools::properties & config_);

    private:

      std::string                            _name_;                 ///< Unique use case name
      std::string                            _use_case_type_id_;     ///< Use case type identifier
      std::string                            _description_;          ///< Description of the use case
      std::string                            _use_case_config_path_; ///< Path of the use case configuration (mutually exclusive with 'use_case_config')
      boost::optional<datatools::properties> _use_case_config_;      ///< Explicit embedded use case configuration (mutually exclusive with 'use_case_config_path')
      uc_composition_description             _composition_description_; ///< Description of the composition of daughter sub-use cases
      
      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION()

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_MODEL_DESCRIPTION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
