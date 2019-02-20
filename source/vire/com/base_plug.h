/// \file  vire/com/base_plug.h
/// \brief Vire com base plug
//
// Copyright (c) 2016-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <string>
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/com/utils.h>
#include <vire/com/i_encoding_driver.h>

namespace vire {

  namespace com {

    class access_hub;
    class domain;
   
    /// \brief Base communication plug
    class base_plug
      : public datatools::i_tree_dumpable
    {
    protected:

      /// Constructor
      base_plug(const std::string & name_,
                const access_hub & parent_,
                const domain & domain_,
                const datatools::logger::priority logging_ = datatools::logger::PRIO_FATAL);

    public:

      datatools::logger::priority get_logging() const;
      
      void set_logging(const datatools::logger::priority);
       
      /// Destructor
      virtual ~base_plug();

      /// Return the name
      const std::string & get_name() const;

      /// Return the parent actor
      const access_hub & get_parent() const;

      const domain & get_domain() const;
      
      const datatools::properties & get_metadata() const;

      datatools::properties & grab_metadata();

      /// Return category
      virtual plug_category_type get_category() const = 0;
     
      /// Return the next message unique ID
      int get_next_message_id() const;

      /// Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_ = empty_options()) const override;

    protected:
      
      /// Pop the next message unique ID
      int _pop_next_message_id();

    private:

      // Configuration:
      std::string         _name_;   ///< Plug name (unique key for the parent actor)
      const access_hub &  _parent_; ///< Parent hub
      const domain &      _domain_; ///< Domain scope
      datatools::logger::priority _logging_;
      datatools::properties _metadata_;
      int32_t               _next_message_id_ = 0;

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
