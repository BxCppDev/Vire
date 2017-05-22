//! \file  vire/com/base_port.h
//! \brief Vire com base port
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

#ifndef VIRE_COM_BASE_PORT_H
#define VIRE_COM_BASE_PORT_H

// Standard library:
#include <memory>

// This project:
#include <vire/com/i_encoding_driver.h>

namespace vire {

  namespace com {

    //! \brief Base communication port
    class base_port : public datatools::i_tree_dumpable
    {
    public:
      enum io_type {
        io_none  = 0,
        io_in    = 0x1,
        io_out   = 0x2
      };

      //! Constructor
      base_port();

      //! Destructor
      virtual ~base_port();

      //! Check I/O type
      bool has_io_type() const;

      //! Set the port I/O type
      void set_io_type(io_type io_);

      bool is_io_in() const;

      bool is_io_out() const;

      bool has_encoding_driver() const;

      void set_encoding_driver(i_encoding_driver &);

      void set_encoding_driver(i_encoding_driver *);

      const i_encoding_driver & get_encoding_driver() const;

      i_encoding_driver & grab_encoding_driver();

      void reset();

    protected:

      void _reset_encoding_driver();

    private:

      io_type _io_ = io_none; //!< I/O type
      bool _owned_encoding_driver_ = false;
      i_encoding_driver * _encoding_driver_ = nullptr; //!< Encoding driver handle

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_BASE_PORT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
