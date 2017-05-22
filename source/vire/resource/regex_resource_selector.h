//! \file  vire/resource/regex_resource_selector.h
//! \brief A selector of resources with regex on resource paths
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

#ifndef VIRE_RESOURCE_REGEX_RESOURCE_SELECTOR_H
#define VIRE_RESOURCE_REGEX_RESOURCE_SELECTOR_H

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>
// - Bayeux/cuts:
#include <cuts/i_cut.h>

// This project:
#include <vire/resource/base_resource_selector.h>

namespace vire {

  namespace resource {

    //! \brief A selector of resources with explicit regex on resource paths (RIDs)
    class regex_resource_selector : public base_resource_selector
    {
    public:

      //! Default constructor
      //! @arg logging_priority_ the logging priority threshold
      regex_resource_selector(datatools::logger::priority logging_priority_ =
            datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~regex_resource_selector();

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

      //! Check if a regular expression on resource paths is set
      //! @return true if the regex is set
      bool has_regex_paths() const;

      //! Add a regular expression on resource paths
      //! @arg regex_ the regex on resource paths to be used
      void set_regex_paths(const std::string & regex_);

      //! Return the regular expression on resource paths
      const std::string & get_regex_paths() const;

      //! Check if a resource with given path is selected
      //! @return true if the resource path is selected
      bool has_path(const std::string & rpath_) const;

      //! Smart print
      //! @arg out_ the output stream where to print
      //! @arg title_ the title
      //! @arg indent_ the indentation string
      //! @arg inherit_ the flag for inherited method
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected :

      //! Selection
      virtual int _accept();

    private:

      // Configuration:
      std::string _regex_paths_; //!< Regular expression on resource paths

      // Working data:
      struct work;
      boost::scoped_ptr<work> _work_; //!< Internal working resources

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(regex_resource_selector);

    };

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_REGEX_RESOURCE_SELECTOR_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
