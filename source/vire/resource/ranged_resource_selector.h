//! \file  vire/resource/ranged_resource_selector.h
//! \brief A selector of resources with explicit range of resource paths
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

#ifndef VIRE_RESOURCE_RANGED_RESOURCE_SELECTOR_H
#define VIRE_RESOURCE_RANGED_RESOURCE_SELECTOR_H

// Third party:
// - Bayeux/cuts:
#include <cuts/i_cut.h>

// This project:
#include <vire/resource/base_resource_selector.h>

namespace vire {

  namespace resource {

    //! \brief A selector of resources with explicit ranged resource paths
    class ranged_resource_selector : public base_resource_selector
    {
    public:

      //! Default constructor
      //! @arg logging_priority_ the logging priority threshold
      ranged_resource_selector(datatools::logger::priority logging_priority_ =
                                   datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~ranged_resource_selector();

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

      //! Check if a resource with given path is selected
      //! @return true if the resource path is selected
      bool has_path(const std::string & rpath_) const;

      //! Check if a range of resource paths is set
      bool has_range() const;

      //! Return the first resource path
      const std::string & get_first_path() const;

      //! Return the second resource path
      const std::string & get_second_path() const;

      //! Add a resource path
      //! @arg first_path_ the first resource path of the range
      //! @arg second_path_ the second resource path of the range
      void set_range(const std::string & first_path_,
                     const std::string & second_path_);

      //! Remove the range on resource paths
      void reset_range();

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
      std::string _first_path_;  //!< First path in the range
      std::string _second_path_; //!< Second path in the range

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(ranged_resource_selector);

    };

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_RANGED_RESOURCE_SELECTOR_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
