//! \file  vire/resource/by_tag_resource_selector.h
//! \brief A selector of resources with explicit list of resource paths
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_RESOURCE_BY_TAG_RESOURCE_SELECTOR_H
#define VIRE_RESOURCE_BY_TAG_RESOURCE_SELECTOR_H

// Third party:
// - Bayeux/cuts:
#include <cuts/i_cut.h>

// This project:
#include <vire/resource/base_resource_selector.h>
#include <vire/resource/utils.h>

namespace vire {

  namespace resource {

    class resource;

    //! \brief A selector of resources with explicit access
    class by_tag_resource_selector : public base_resource_selector
    {
    public:

      //! Default constructor
      //! @arg logging_priority_ the logging priority threshold
      by_tag_resource_selector(datatools::logger::priority logging_priority_ =
                                  datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~by_tag_resource_selector();

      //! Initialization
      //! @arg config_ the container of configuration parameters
      //! @arg srvc_mgr_ the service manager optionally used to initialize the cut (not used)
      //! @arg cuts_ the dictionary of cuts optionally used to initialize the cut (not used)
      virtual void initialize(const datatools::properties & config_,
                              datatools::service_manager & /* srvc_mgr_ */,
                              cuts::cut_handle_dict_type & /* cuts_ */);

      //! Export to a container of configuration parameters
      void export_to_config(datatools::properties & config_,
                            uint32_t flags_ = EXPORT_CONFIG_DEFAULT,
                            const std::string & prefix_ = "") const;

      //! Reset
      virtual void reset();

      //! Check if requested pair role tag / value is valid
      const bool has_role_tag() const;

      //! Set the requested pair role tag / value
      //! @arg access_ the requested access
      void set_role_tag(const std::string tag_, const std::string value_);

      //! Return the requested role tag
      const std::string get_role_tag() const;

      //! Return the requested role value
      const std::string get_tag_value() const;

      //! Smart print
      //! @arg out_ the output stream where to print
      //! @arg title_ the title
      //! @arg indent_ the indentation string
      //! @arg inherit_ the flag for inherited method
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Return the set of resource paths
      const std::set<std::string> & get_resource_paths() const;

    protected :

      const std::string VALID_PREFIX =  "aux.snemo.role_tag";
      const std::string INVALID_STR  =  "**invalid**";

      // Configuration:
      std::string _tag_;   //!< Requested role tag
      std::string _value_; //!< Requested value

      //! Selection
      virtual int _accept();

    private:

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(by_tag_resource_selector);

    };

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_BY_TAG_RESOURCE_SELECTOR_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
