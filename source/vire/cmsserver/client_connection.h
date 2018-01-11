//! \file  vire/cmsserver/client_connection.h
//! \brief The client connection
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

#ifndef VIRE_CMSSERVER_CLIENT_CONNECTION_H
#define VIRE_CMSSERVER_CLIENT_CONNECTION_H

namespace vire {

  namespace cmsserver {

    /// \brief The client connection
    class client_connection
    {

    public:

      /// Default constructor
      client_connection(uint32_t flags_ = 0);

      /// Destructor
      ~client_connection();

      private:

        int32_t     _id_;
        std::string _key_;
        std::string _user_login_;
        boost::posix_time::time_period _time_interval_;

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_CLIENT_CONNECTION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
