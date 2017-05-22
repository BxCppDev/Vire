//! \file  vire/resource/base_resource_selector.h
//! \brief A base class for selector of resources
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

#ifndef VIRE_RESOURCE_BASE_RESOURCE_SELECTOR_H
#define VIRE_RESOURCE_BASE_RESOURCE_SELECTOR_H

// Third party:
// - Bayeux/cuts:
#include <cuts/i_cut.h>

namespace vire {

  namespace resource {

    // Forward declaration:
    class manager;

    //! \brief A base selector of resources
    class base_resource_selector : public cuts::i_cut
    {
    public:

      //! Default constructor
      //! @arg logging_priority_ the logging priority threshold
      base_resource_selector(datatools::logger::priority logging_priority_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~base_resource_selector();

      //! Check the resource manager
      bool has_resource_mgr() const;

      //! Set the resource manager
      void set_resource_mgr(const manager &);

      //! Reset the resource manager
      void reset_resource_mgr();

      //! Return the resource manager
      const manager & get_resource_mgr() const;

      // //! Check if the set of resource identifiers (RID) is cached
      // bool has_cached_rids() const;

      // //! Buil the cached set of resource identifiers (RID)
      // void build_cached_rids();

      // //! Return the set of RIDs
      // const std::set<int32_t> & get_set_of_rids() const;

      //! Smart print
      //! @arg out_ the output stream where to print
      //! @arg title_ the title
      //! @arg indent_ the indentation string
      //! @arg inherit_ the flag for inherited method
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected:

      //! Initialize the attributes
      void _base_init(const datatools::properties & config_,
                      datatools::service_manager * srvc_mgr_ = nullptr,
                      cuts::cut_handle_dict_type * cuts_ = nullptr);

      //! Reset the attributes
      void _base_reset();

      //! Set default attribute values
      void _set_defaults();

    private:

      const manager * _resource_mgr_; //!< Handle to an external resource manager

      // Working data:
      // boost::scoped_ptr<std::set<int32_t> > _cached_rids_; //!< Set of resource identifiers (RID)

    };

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_ENUMERATED_RESOURCE_SELECTOR_H

  /*
  ** Local Variables: --
  ** mode: c++ --
  ** c-file-style: "gnu" --
  ** tab-width: 2 --
  ** End: --
  */
