//! \file  vire/cmssubcontractor/sc_description.h
//! \brief The CMS subcontractor description
//
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSUBCONTRACTOR_SC_DESCRIPTION_H
#define VIRE_CMSSUBCONTRACTOR_SC_DESCRIPTION_H

// Standard Library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>
#include <bayeux/datatools/properties.h>

namespace vire {

  namespace cmssubcontractor {

    /// \brief Description of a CMS subcontractor 
    class sc_description
      : public datatools::enriched_base
    {

    public:

      /// Default constructor
      sc_description();

      /// Destructor
      ~sc_description();

      /// Check validity
      bool is_complete() const;

      /// Check login
      bool has_com_login() const;

      const std::string & get_com_login() const;

      void set_com_login(const std::string & login_);

      /// Check password
      bool has_com_password() const;

      const std::string & get_com_password() const;

      void set_com_password(const std::string & password_);
 
      /// Check server host
      bool has_com_server_host() const;

      const std::string & get_com_server_host() const;

      void set_com_server_host(const std::string & host_);

      /// Check server port
      bool has_com_server_port() const;
      
      int get_com_server_port() const;

      void set_com_server_port(const int port_);

      void initialize(const datatools::properties & config_);

      void reset();

      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const override;
      
    private:

      // Configuration:
      std::string _com_server_host_;
      int         _com_server_port_ = 0;
      std::string _com_login_;
      std::string _com_password_;

    };
 
  } // namespace cmssubcontractor

} // namespace vire

#endif // VIRE_CMSSUBCONTRACTOR_SC_DESCRIPTION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
