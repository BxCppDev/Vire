//! \file  vire/cmsserver/uc_composition.h
//! \brief Vire CMS use case composition
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

#ifndef VIRE_CMSSERVER_UC_COMPOSITION_H
#define VIRE_CMSSERVER_UC_COMPOSITION_H

// Standard Library:
#include <string>
#include <map>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/cmsserver/use_case_info.h>

namespace vire {

  namespace cmsserver {

    /// \brief Use case composition 
    class uc_composition
    {

    public:

      /// \brief Daughter description record
      struct daughter_entry {
        std::string           name;
        use_case_info         uc_info;
        datatools::properties traits;
      };
      
      typedef std::map<std::string, daughter_entry> daughters_dict_type;

      uc_composition();

      ~uc_composition();

      bool is_initialized() const;
        
      void initialize(const datatools::properties & config_);

      void reset();

      bool has_daughters() const;

      bool has_daughter(const std::string & name_) const;

      void add_daughter(const std::string & name_,
                        const use_case_info & uc_info_,
                        const datatools::properties & traits_);

      void remove_daughter(const std::string & name_);

      const daughter_entry & get_daughter(const std::string & name_) const;
      
    private:

      bool                _initialized_ = false; ///< Initialization flag
      daughters_dict_type _daughters_;           ///< Dictionary of daughter use cases
      
    };
    
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_COMPOSITION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
