//! \file  vire/user/enumerated_user_selector.h
//! \brief A selector of users with explicit list of user identifiers
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

#ifndef VIRE_USER_ENUMERATED_USER_SELECTOR_H
#define VIRE_USER_ENUMERATED_USER_SELECTOR_H

// Third party:
// - Bayeux/cuts:
#include <cuts/i_cut.h>

namespace vire {

  namespace user {

    // Forward declaration:
    class manager;

    //! \brief A selector of users with explicit enumerated user identifiers (UIDs)
    class enumerated_user_selector : public cuts::i_cut
    {
    public:

      //! Default constructor
      //! @arg logging_priority_ the logging priority threshold
      enumerated_user_selector(datatools::logger::priority logging_priority_ =
                               datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~enumerated_user_selector();

      //! Initialization
      //! @arg config_ the container of configuration parameters
      //! @arg srvc_mgr_ the service manager optionally used to initialize the cut (not used)
      //! @arg cuts_ the dictionary of cuts optionally used to initialize the cut (not used)
      virtual void initialize(const datatools::properties & config_,
                              datatools::service_manager & srvc_mgr_,
                              cuts::cut_handle_dict_type & cuts_);

      //! Export to a container of configuration parameters
      void export_to_config(datatools::properties & config_,
                            uint32_t flags_ = EXPORT_CONFIG_DEFAULT,
                            const std::string & prefix_ = "") const;

      //! Reset
      virtual void reset();

      //! Check the validity of the time interval
      //! @return true if the embedded time interval is valid
      bool has_uid(int32_t uid_) const;

      //! Add an user identifier (UID)
      //! @arg uid_ the UID to be added
      void add_uid(int32_t uid_);

      //! Remove an user identifier (UID)
      //! @arg uid_ the UID to be removed
      void remove_uid(int32_t uid_);

      //! Check the user manager
      bool has_user_mgr() const;

      //! Set the user manager
      void set_user_mgr(manager &);

      //! Reset the user manager
      void reset_user_mgr();

      //! Smart print
      //! @arg out_ the output stream where to print
      //! @arg title_ the title
      //! @arg indent_ the indentation string
      //! @arg inherit_ the flag for inherited method
      // virtual
      void tree_dump(std::ostream & out_ = std::clog,
                     const std::string & title_  = "",
                     const std::string & indent_ = "",
                     bool inherit_ = false) const;

      //! Return the set of UIDs
      const std::set<int32_t> & get_uids() const;

    protected :

      //! Selection
      // virtual
      int _accept();

    private:

      std::set<int32_t> _uids_;     //!< Set of the user identifiers (UIDs) validated by this selector
      manager *         _user_mgr_; //!< Temporary handle to an external user manager (use only at construction)

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(enumerated_user_selector);

    };

  } // namespace user

} // namespace vire

#endif // VIRE_USER_ENUMERATED_USER_SELECTOR_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
