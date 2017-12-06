//! \file  vire/com/actor.h
//! \brief Vire communication actor
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_ACTOR_H
#define VIRE_COM_ACTOR_H

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>

namespace vire {

  namespace com {

    //! \brief Actor description
    //!
    //! Vire uses 3 types of actors:for a given control and monitoring system
    //! associated to an experiment:
    //! * a unique Vire server,
    //! * an arbitrary number of Vire clients,
    //! * optional Vire subcontractors (generally at least one) responsible
    //!   of the management of hardware devices.
    //!
    class actor
      : public ::datatools::i_tree_dumpable
    {
    public:

      //! \brief Actor categories
      enum category_type {
        CATEGORY_INVALID       = 0, ///< Invalid actor category
        CATEGORY_SERVER        = 1, ///< Server actor category
        CATEGORY_CLIENT        = 2, ///< Server client category
        CATEGORY_SUBCONTRACTOR = 3, ///< Server subcontractor category
        CATEGORY_SYSTEM        = 4  ///< Private system user category
      };

      //! Return the label associated to a category type
      static std::string category_label(const category_type);

      //! Return the category type associated to a label
      static category_type category(const std::string &);

      //! Build actor name
      static std::string build_name(const category_type category_,
                                    const std::string & setup_name_ = "",
                                    const std::string & id_ = "");

      //! Default constructor
      actor();

      //! Constructor
      actor(const std::string & name_, const category_type category_);

      //! Destructor
      virtual ~actor();

      //! Check actor validity
      bool is_valid() const;

      //! Reset/invalidate
      void reset();

      //! Check if name is set
      bool has_name() const;

      //! Set the actor name
      void set_name(const std::string & name_);

      //! Return the actor name
      const std::string & get_name() const;

      //! Check if category is set
      bool has_category() const;

      //! Set the actor category
      void set_category(const category_type & category_);

      //! Return the actor category
      category_type get_category() const;

      //! Check server actor category
      bool is_server() const;

      //! Check client actor category
      bool is_client() const;

      //! Check subcontractor actor category
      bool is_subcontractor() const;

      //! Check system actor category
      bool is_system() const;

      //! Return the metadata
      datatools::properties & grab_metadata();

      //! Return the metadata
      const datatools::properties & get_metadata() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      category_type         _category_; //!< Actor category
      std::string           _name_;     //!< Actor name
      datatools::properties _metadata_; //!< Metadata

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_ACTOR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
