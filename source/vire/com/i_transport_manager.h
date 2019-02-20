//! \file  vire/com/i_transport_manager.h
//! \brief Vire com transport manager interface
//
// Copyright (c) 2018-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_I_TRANSPORT_MANAGER_H
#define VIRE_COM_I_TRANSPORT_MANAGER_H

// Standard library:
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/logger.h>
#include <datatools/factory_macros.h>
#include <datatools/i_tree_dump.h>

// This project:
#include <vire/com/utils.h>

namespace vire {

  namespace com {

    //! \brief Transport manager interface
    class i_transport_manager
      : public datatools::i_tree_dumpable
    {
    public:
      
      datatools::logger::priority get_logging() const;

      void set_logging(const datatools::logger::priority);
      
      const std::string & get_name() const;

      void set_name(const std::string &);

      //! Default constructor
      i_transport_manager();

      //! Destructor
      virtual ~i_transport_manager();

      //! Check initialization flag
      bool is_initialized() const;

      //! Initialize the driver from a set of configuration parameters
      void initialize(const datatools::properties &);

      //! Reset the driver
      void reset();

      //! Check if a domain exists
      virtual bool has_domain(const std::string & domain_name_) const = 0;
      
      //! Add a domain of given category
      virtual void add_domain(const std::string & domain_name_,
                              const domain_category_type category_) = 0;
      
      //! Remove a domain 
      virtual void remove_domain(const std::string & domain_name_) = 0;
 
      //! Check if a user exists
      virtual bool has_user(const std::string & login_) const = 0;
      
      //! Add a user of given category
      virtual void add_user(const std::string & login_,
                            const std::string & password_,
                            const access_category_type category_) = 0;
      
      //! Remove a user 
      virtual void remove_user(const std::string & login_) = 0;

      // //! Grant domain access to a user
      // virtual void grant_access(const std::string & login_,
      //                          const std::string & domain_name_) = 0;     
      
      //! Add transport resources associated to a subcontractor
      virtual void add_subcontractor(const subcontractor_info & sc_info_) = 0;
     
      //! Remove transport resources associated to a subcontractor
      virtual void remove_subcontractor(const subcontractor_info & sc_info_) = 0;
    
      //! Add transport resources associated to a client
      virtual void add_client(const client_info & client_info_) = 0;
     
      //! Remove transport resources associated to a client
      virtual void remove_client(const client_info & client_info_) = 0;

      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_ = empty_options()) const override;
      
    private:

      //! Initialization
      virtual void _at_initialize_(const datatools::properties &) = 0;

      //! Reset
      virtual void _at_reset_() = 0;

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag
      datatools::logger::priority _logging_ = datatools::logger::PRIO_FATAL;

      // Configuration:
      std::string _name_;
      
      // Factory stuff :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(i_transport_manager)

    };

    typedef std::shared_ptr<i_transport_manager> transport_manager_ptr; 

  } // namespace com

} // namespace vire

#define VIRE_COM_TRANSPORT_MANAGER_REGISTRATION_INTERFACE(TransportClassName) \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::com::i_transport_manager, TransportClassName) \
  /**/

#define VIRE_COM_TRANSPORT_MANAGER_REGISTRATION_IMPLEMENT(TransportClassName,TransportId) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(::vire::com::i_transport_manager,TransportClassName,TransportId) \
  /**/

#endif // VIRE_COM_I_TRANSPORT_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
