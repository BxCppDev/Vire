//! \file  vire/cmsserver/dummy_process.h
//! \brief Dummy cmsserver process
//
// Copyright (c) 2016 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_DUMMY_PROCESS_H
#define VIRE_CMSSERVER_DUMMY_PROCESS_H

// This project:
#include <vire/cmsserver/base_process.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace cmsserver {

    //! \brief Base process
    class dummy_process : public base_process
    {
    public:

      //! Constructor
      dummy_process();

      //! Destructor
      virtual ~dummy_process();

    protected:

      virtual bool _is_subprocess_allowed() const final;

    private:

      bool _initialized_ = false;

      // Class registration:
      VIRE_CMSSERVER_PROCESS_REGISTRATION_INTERFACE(dummy_process);

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_DUMMY_PROCESS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
