//! \file  vire/com/base_plug.h
//! \brief Vire com base plug
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_BASE_PLUG_H
#define VIRE_COM_BASE_PLUG_H

// Standard library:
#include <cstdint>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>

// This project:
// #include <vire/utility/model_identifier.h>

namespace vire {

  namespace com {

    enum plug_category_type {
      PLUG_INVALID        = 0,
      PLUG_EVENT_EMITTER  = 1,
      PLUG_EVENT_LISTENER = 2,
      PLUG_SERVICE_CLIENT = 3,
      PLUG_SERVICE_SERVER = 4
    };

    std::string plug_category_to_label(const plug_category_type);

    class domain;

    //! \brief Base communication plug
    class base_plug
      : public datatools::i_tree_dumpable
    {
    public:

      //! Default constructor
      base_plug();

      //! Constructor
      base_plug(domain & dom_,
                const std::string & name_,
                const plug_category_type category_);

      //! Destructor
      virtual ~base_plug();

      bool is_valid() const;

      //! Check domain
      bool has_domain() const;

      //! Set domain
      void set_domain(domain &);

      //! Return the domain
      const domain & get_domain() const;

      //! Check name
      bool has_name() const;

      //! Set name
      void set_name(const std::string & name_);

      //! Return name
      const std::string & get_name() const;

      //! Check category
      bool has_category() const;

      //! Set category
      void set_category(const plug_category_type & category_);

      //! Return category
      plug_category_type get_category() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      bool is_initialized() const;

    protected:

      void _set_initialized(bool);

      void _base_init();

      void _base_reset();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
      domain *           _dom_ = nullptr;
      std::string        _name_;    //!< Identifier
      plug_category_type _category_ = PLUG_INVALID; //!< Plug category

    };

    typedef std::shared_ptr<base_plug> plug_ptr_type;

  } // namespace com

} // namespace vire

#endif // VIRE_COM_BASE_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
