//! \file  vire/cmsserver/uc_utils.h
//! \brief Vire CMS use case factory utilities
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_UC_UTILS_H
#define VIRE_CMSSERVER_UC_UTILS_H

// Standard Library:
#include <string>
#include <iostream>

// This project:
#include <vire/utility/path.h>

namespace vire {
  namespace device {
    class manager;
  } // namespace device
  namespace resource {
    class manager;
  } // namespace resource
} // namespace vire

namespace vire {

  namespace cmsserver {

    class uc_factory;
    
    bool validate_name(const std::string & name_);

    /// \brief Use case construction context
    struct uc_construction_context_type
    {
    public:

      uc_construction_context_type();

      uc_construction_context_type(const vire::utility::path & path_,
                                   const std::string & model_id_,
                                   const std::string & description_);

      bool is_complete() const;
      
      vire::utility::path path;          ///< Use case instance global path
      std::string         model_id;      ///< Use case model ID
      std::string         description;   ///< Use case instance description
      
      uc_factory                    * factory   = nullptr; ///< Use case factory handle
      const vire::device::manager   * devices   = nullptr; ///< Device manager handle 
      const vire::resource::manager * resources = nullptr; ///< Resource manager handle
      
    };   

    /// \brief Unique identifier of a scope resource/device port within an use case
    ///
    /// \code
    ///         _______
    ///        /   A   \
    ///       +---------+---------------------+
    ///       |               ____________    |
    ///       O---           / Functional \   |     Functional resource port IDs
    ///       |   `-------<-o R1           |  | --> R1 
    ///       O-----------<-o R2   Algo    |  | --> R2  
    ///       |   ,-------<-o R3           |  | --> R3 
    ///       O--' ,------<-o R4           |  | --> R4 
    ///       |   |          \____________/   |
    ///       O-----.        _____            |
    ///       |   |  `      /  B  \           |
    ///       O   |  |      +-----+--------+  |
    ///       |   |  |      | sub-usecase  |  |     Distributable resource/device port IDs
    ///       O---c--|----<-O P1           |  | --> @B:P1
    ///       |   |  |      |              |  |
    ///       O---'  \  ,-<-O P2           |  | --> @B:P2
    ///       |       `/    |              |  |
    ///       O-------' `-<-O P3           |  | --> @B:P3
    ///       |             +--------------+  |
    ///       +-------------------------------+
    /// \endcode
    ///
    struct uc_port_id
    {
    public:
      
      uc_port_id();
    
      uc_port_id(const std::string & port_key_);
      
      uc_port_id(const std::string & name_,
                       const std::string & port_key_);

      bool is_functional() const;

      bool is_distributable() const;

      bool has_name() const;
      
      void set_name(const std::string & name_);
      
      const std::string & get_name() const;
 
      bool has_port_key() const;
      
      void set_port_key(const std::string & port_key_);
      
      const std::string & get_port_key() const;
      
      bool operator==(const uc_port_id & other_) const;
      
      bool operator<(const uc_port_id & other_) const;

      std::string to_string() const;

      void to_string(std::string &) const;
      
      bool from_string(const std::string & repr_);

      void reset();
      
      friend std::ostream & operator<<(std::ostream & out_, const uc_port_id &);
      
    private:
      
      std::string _name_;     ///< Name of the daughter use case (only for distributable)
      std::string _port_key_; ///< Use case's port key (unique port identifier within the object)

      BOOST_SERIALIZATION_BASIC_DECLARATION();

    };
 
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_FACTORY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
