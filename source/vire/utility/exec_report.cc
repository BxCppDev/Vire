// vire/utility/exec_report.cc
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

// Ourselves:
#include <vire/utility/exec_report.h>

namespace vire {

  namespace utility {

    exec_report::exec_report()
    {
      return;
    }

    exec_report::exec_report(const int code_, const std::string & message_)
    {
      set_code(code_);
      set_message(message_);
      return;
    }

    void exec_report::set_code(const int code_)
    {
      _code_ = code_;
      return;
    }

    int exec_report::get_code() const
    {
      return _code_;
    }
 
    bool exec_report::is_success() const
    {
      return _code_ == error::CODE_SUCCESS;
    }

    bool exec_report::is_warning() const
    {
      return _code_ == error::CODE_WARNING;
    }

    bool exec_report::is_failure() const
    {
      return _code_ >= error::CODE_FAILURE;
    }

    bool exec_report::has_message() const
    {
      return !_message_.empty();
    }

    void exec_report::set_message(const std::string & message_)
    {
      _message_ = message_;
      return;
    }

    const std::string & exec_report::get_message() const
    {
      return _message_;
    }
    
  } // namespace utility

} // namespace vire
