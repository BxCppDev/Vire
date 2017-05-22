//! \file utility/testing/test-utility-base_error.cxx
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

// Standard library:
#include <iostream>

// Third party:
// - Boost
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>
// - BxJsontools
#include <jsontools/iofile.h>
#include <jsontools/base_type_converters.h>

// This project:
#include <vire/utility/base_error.h>
#include <vire/utility/invalid_context_error.h>

/// \brief A dummy error record
class dummy_error : public vire::utility::base_error
{
public:

  /// Constructor
  dummy_error(const std::string & message_format_ = "Dummy value [%v]!")
    : vire::utility::base_error(1, message_format_),
      _value_(0)
  {
    return;
  }

  /// Set the value
  void set_value(int value_)
  {
    _value_ = value_;
    return;
  }

  /// Build the message
  virtual void _build_message(std::string & message_) const final
  {
    message_ = boost::replace_all_copy(get_message_format(),
                                       "%v",
                                       boost::lexical_cast<std::string>(_value_));
    return;
  }

  //! Smart print
  virtual void tree_dump(std::ostream & out_ = std::clog,
                         const std::string & title_  = "",
                         const std::string & indent_ = "",
                         bool inherit_ = false) const;

  /// Main JSON (de-)serialization method
  virtual void jsonize(jsontools::node & node_,
                         unsigned long int version_ = 0);

private:

  int _value_;

};

void dummy_error::jsonize(jsontools::node & node_,
                            unsigned long int version_)
{
  this->base_error::jsonize(node_);
  node_["value"] % _value_;
  return;
}

void dummy_error::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
{
  this->base_error::tree_dump(out_, title_, indent_, true);

  out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
       << "Value : '" << _value_ << "'" << std::endl;

  return;
}

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    // Base error:
    {
      vire::utility::base_error err;
      err.set_code(3);
      err.set_message("An error.");
      err.tree_dump(std::clog, "Base error:");
      std::clog << std::endl;
      jsontools::store("test-utility-base_error.json", err);
    }

    {
      vire::utility::base_error err;
      jsontools::load("test-utility-base_error.json", err);
      err.tree_dump(std::clog, "Base error (loaded):");
      std::clog << std::endl;
    }

    // Dummy error:
    {
      dummy_error derr;
      derr.set_value(42);
      derr.tree_dump(std::clog, "Dummy error:");
      std::clog << std::endl;
      std::clog << "Message: '" << derr.get_message() << "'" << std::endl;
      std::clog << std::endl;
      jsontools::store("test-utility-base_error_2.json", derr);
    }

    {
      dummy_error derr;
      jsontools::load("test-utility-base_error_2.json", derr);
      derr.tree_dump(std::clog, "Dummy error (loaded):");
      std::clog << std::endl;
    }

    // Dummy error:
    {
      vire::utility::invalid_context_error icerr;
      icerr.set_code(666);
      icerr.set_message_format("Invalid testing context");
      icerr.tree_dump(std::clog, "Dummy error:");
      std::clog << std::endl;
      jsontools::store("test-utility-base_error_3.json", icerr);
    }

    {
      vire::utility::invalid_context_error icerr;
      jsontools::load("test-utility-base_error_3.json", icerr);
      icerr.tree_dump(std::clog, "Dummy error (loaded):");
      std::clog << std::endl;
    }

  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
