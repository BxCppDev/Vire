//! \file  vire/utility/exec_report.h
//! \brief Simple execution report
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_EXEC_REPORT_H
#define VIRE_UTILITY_EXEC_REPORT_H

// Standard Library:
#include <string>

// This project:
#include <vire/utility/error.h>

namespace vire {

  namespace utility {

    class exec_report
    {
    public:

      exec_report();

      exec_report(const int, const std::string &);

      void set_code(const int);

      int get_code() const;
      
      bool is_success() const;

      bool is_warning() const;

      bool is_failure() const;

      bool has_message() const;

      void set_message(const std::string &);

      const std::string & get_message() const;
      
    private:

      int         _code_ = error::CODE_SUCCESS;
      std::string _message_;
      
    };

  } // namespace utility

} // namespace vire

#endif // VIRE_UTILITY_EXEC_REPORT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
