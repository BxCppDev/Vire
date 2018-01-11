//! \file  vire/device/instance_tree.h
//! \brief Tree of Vire device instance info records
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_DEVICE_INSTANCE_TREE_H
#define VIRE_DEVICE_INSTANCE_TREE_H

// Standard library:
#include <string>
#include <map>
#include <set>

// Third Party:
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/bit_mask.h>
#include <datatools/i_tree_dump.h>

// This project:
#include <vire/device/instance_info.h>

namespace vire {

  namespace device {

    //! \brief Dictionary of device/port instances
    class instance_tree
      : public datatools::i_tree_dumpable
    {
    public:

      typedef std::map<std::string, instance_info_ptr> instance_info_dict_type;

      //! Default constructor
      instance_tree();

      //! Destructor
      virtual ~instance_tree();

      void reset();

      datatools::logger::priority get_logging_priority() const;

      void set_logging_priority(const datatools::logger::priority);

      bool has_instance(const std::string & path_) const;

      void add_instance(instance_info & ii_);

      void add_instance(instance_info_ptr & pii_);

      const instance_info & get_instance(const std::string & path_) const;

      instance_info & grab_instance(const std::string & path_);

      const instance_info_ptr & get_instance_ptr(const std::string & path_) const;

      instance_info_ptr & grab_instance_ptr(const std::string & path_);

      std::size_t size() const;

      enum build_path_flag {
        BP_CLEAR      = datatools::bit_mask::bit00,
        BP_NO_DEVICES = datatools::bit_mask::bit01,
        BP_NO_PORTS   = datatools::bit_mask::bit02
      };

      //! Return the path of the parent device instance
      const std::string & get_parent_path() const;

      //! Set the path of the parent device instance
      void set_parent_path(const std::string & path_);

      //! Return the path of the preferred device instance
      const std::string & get_preferred_path() const;

      //! Set the path of the preferred device instance
      void set_preferred_path(const std::string & path_);

      //! Check if a path is the parent of another one
      bool is_parent_of(const std::string & candidate_ancestor_, const std::string & path_) const;

      //! Check if a path is the child of another one
      bool is_child_of(const std::string & candidate_child_, const std::string & path_) const;

      //! Populate the set of paths from paths stored in the tree.
      //! @return the number of added paths in the set
      std::size_t build_paths(std::set<std::string> & paths_,
                              const uint32_t flags_ = 0) const;

      void build_children_of(const std::string & parent_path_,
                             std::set<std::string> & paths_,
                             const uint32_t flags_ = 0) const;

      void build_parents_of(const std::string & child_path_,
                            std::set<std::string> & paths_) const;

      const instance_info_dict_type & get_instances() const;

      bool is_locked() const;

      void lock();

      void unlock();

      //! Smart print
      //!
      //! Supported options:
      //! \code
      //! {
      //!   "title"          : "My title: ",
      //!   "indent"         : "[debug] ",
      //!   "inherit"        : false,
      //!   "list_instances" : true
      //! }
      //! \endcode
       virtual void print_tree(std::ostream & out_ = std::clog,
                               const boost::property_tree::ptree & options_ = datatools::i_tree_dumpable::empty_options()) const;

    private:

      // Management:
      datatools::logger::priority _logging_priority_; //!< Logging priority

      // Working/internal data:
      bool _locked_ = false;
      std::string _parent_path_;
      std::string _preferred_path_;
      instance_info_dict_type _instances_; //!< Dictionary of instances

    };

  } // end of namespace device

} // end of namespace vire

// #define BX_DATATOOLS_PRINT_TREE_DECLARE()                               \
//   public:                                                               \
//   virtual void print_tree(std::ostream & out_ = std::clog,              \
//                           const boost::property_tree::ptree & options_ = ::datatools::i_tree_dumpable::empty_options()) const; \
  /**/


#endif // VIRE_DEVICE_INSTANCE_TREE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
