//! \file    vire/device/local_role.h
//! \brief   Vire device local role support
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

#ifndef VIRE_DEVICE_LOCAL_ROLE_H
#define VIRE_DEVICE_LOCAL_ROLE_H

// Standard Library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>
#include <datatools/properties.h>

namespace vire {

  namespace device {

    //! \brief Vire device local role support
    class local_role : public datatools::i_tree_dumpable
    {
    public:

      //! Return the key of the property for role
      static const std::string & role_id_key();

      //! Return the name of the expert role locally defined in a device
      //! Example of the device model configuration file:
      //! \code
      //! vire.device.local_role.role_id : string = "expert"
      //! \endcode
      //!
      //! OBSOLETE
      //! Extraction from the transcient device model object:
      //!
      //! Parent device path: "SuperNEMO:///Calorimeter/HV/PS_0"
      //! Resource path     : "SuperNEMO:///Calorimeter/HV/PS_0/Current_*"
      //!
      //! \code
      //! struct vire::device::local_role {
      //!   bool is_user;
      //!   bool is_expert;
      //!   std::vector<std::string> tags;
      //! }
      //! vire::device::manager dev_mgr;
      //! const vire::device::mapping & dev_map = dev_mgr.get_mapping();
      //! std::string device_path = "SuperNEMO:///Calorimeter/HV/PS_0";
      //! vire::device::local_role my_dev_lrole;
      //! my_dev_lrole.is_user = false;
      //! my_dev_lrole.is_expert = false;
      //! if (dev_map.has_mapping_info(device_path)) {
      //!   const vire::device::mapping_info & minfo = dev_map.get_mapping_info(device_path);
      //!   if (minfo.is_device()) {
      //!     const vire::device::logical_device & log_dev = minfo.get_logical_device();
      //!     const vire::device::base_device_model & dev_mod = log_dev.get_model();
      //!     if (dev_mod.get_auxiliaries().has_key(vire::device::local_role::role_id_key()) {
      //!        const std::string & local_role_id = dev_mod.get_auxiliaries().fetch_string(vire::device::local_role::role_id_key());
      //!        if (local_role_id == vire::device::local_role::role_expert_id()) {
      //!           my_dev_lrole.is_user = true;
      //!           my_dev_lrole.is_expert = true;
      //!        } else if (local_role_id == vire::device::local_role::role_user_id()) {
      //!           my_dev_lrole.is_user = true;
      //!        } else {
      //!           DT_THROW(std::logic_error, "Invalid device local role id '" << local_role_id << "'!");
      //!        }
      //!     } else {
      //!       // Default:
      //!       my_dev_lrole.is_user = true;
      //!     }
      //!     if (dev_mod.get_auxiliaries().has_key(vire::device::local_role::role_tags_key()) {
      //!        dev_mod.get_auxiliaries().fetch(vire::device::local_role::role_tags_key(), my_dev_lrole.tags);
      //!     }
      //!   }
      //!
      //! }
      //! \endcode
      static const std::string & role_expert_id();

      //! Return the name of the user role locally defined in a device
      //! Example of the device model configuration file:
      //! \code
      //! vire.device.local_role.role_id : string = "user"
      //! \endcode
      static const std::string & role_user_id();

      //! Return the key of the property for local role tagging
      //! Example of the device model configuration file:
      //! \code
      //! vire.device.local_role.role_id : string    = "expert"
      //! vire.device.local_role.tags    : string[2] = "advanced" "guru"
      //! \endcode
      static const std::string & role_tags_key();

      //! \brief Supported local role id
      enum local_role_id {
        LR_INVALID = -1, //!< Invalid role id
        LR_USER    =  1, //!< User role
        LR_EXPERT  =  3, //!< Expert role
        LR_DEFAULT =  LR_USER //!< Default role
      };

      //! Convert a local role identifier to a string
      static const std::string & role_id_to_string(local_role_id);

      //! Convert a string to a local role identifier
      static local_role_id string_to_role_id(const std::string &);

      //! Default constructor
      local_role();

      //! Check if role identifier is defined
      bool has_role_id() const;

      //! Return the local role identifier
      local_role_id get_role_id() const;

      //! Set the local role identifier
      void set_role_id(local_role_id);

      //! Check user role identifier
      bool is_user() const;

      //! Check expert role identifier
      bool is_expert() const;

      //! Check if a role tag is supported
      bool has_tag(const std::string & tag_id_) const;

      //! Add a role tag
      void add_tag(const std::string & tag_id_);

      //! Check if tags are defined
      bool has_tags() const;

      //! Check initialization
      bool is_initialized() const;

      //! Initialize
      void initialize_simple();

      //! Initialize
      void initialize(const datatools::properties & config_);

      //! Reset
      void reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_         = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_          = false) const;

    private:

      local_role_id _role_id_;  //!< Local role identifier
      std::set<std::string> _tags_; //!< Set of role tags

    };

  } // namespace device

} // namespace vire

#endif // VIRE_DEVICE_LOCAL_ROLE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
