//! \file  vire/cmssubcontractor/sc.h
//! \brief The CMS subcontractor
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

#ifndef VIRE_CMSSUBCONTRACTOR_SC_H
#define VIRE_CMSSUBCONTRACTOR_SC_H

// Standard Library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>

// This project:
#include <vire/cmssubcontractor/sc_description.h>
#include <vire/com/manager.h>


namespace vire {

  namespace cmssubcontractor {

    /// \brief CMS subcontractor 
    class sc
    {

    public:

      /// Default constructor
      sc();
      
      /// Destructor
      ~sc();

      void set_description(const sc_description &);

      const sc_description & get_description() const;

      bool is_initialized() const;
      
    private:

      // Management
      bool _initialized_ = false;
      
      sc_description _desc_;    ///< Description
      vire::com::manager _com_; ///< Communication service
      
    };

  } // namespace cmssubcontractor

} // namespace vire

#endif // VIRE_CMSSUBCONTRACTOR_SC_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
