//! \file  vire/cms/argument_error.h
//! \brief Argument error object
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMS_ARGUMENT_ERROR_H
#define VIRE_CMS_ARGUMENT_ERROR_H

// This project:
#include <vire/utility/base_error.h>
#include <vire/cms/status.h>

namespace vire {

  namespace cms {

    /// \brief Error occuring in case of argument error
    ///
    /// Three cases are considered:
    /// - invalid number of arguments
    /// - invalid argument identifier/name
    /// - invalid argument value
    class argument_error
      : public ::vire::utility::base_error
    {
    public:

      static const int32_t EC_INVALID_NARGS     = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;
      static const int32_t EC_INVALID_ARG       = EC_INVALID_NARGS + 1;
      static const int32_t EC_INVALID_ARG_VALUE = EC_INVALID_ARG + 1;

      /// Default constructor
      argument_error();

      /// Destructor
      virtual ~argument_error();

      /// Check
      bool is_invalid_number_of_arguments() const;

      /// Make an invalid number of arguments error
      void make_invalid_number_of_arguments(std::size_t expected_nargs_,
                                            std::size_t invalid_nargs_);

      /// Return the expected number of arguments
      std::size_t get_expected_nargs() const;

      /// Return the invalid number of arguments
      std::size_t get_invalid_nargs() const;

      /// Check
      bool is_invalid_argument() const;

      /// Make an invalid argument error
      void make_invalid_argument(const std::string & argname_);

      /// Return the name of the invalid argument
      std::string get_invalid_argument_name() const;

      /// Check
      bool is_invalid_argument_value() const;

      /// Make an invalid argument value error
      void make_invalid_argument_value(const std::string & argname_,
                                       const std::string & value_repr_);

      /// Return the name of the argument with invalid value
      std::string get_argument_name() const;

      /// Return the invalid value representation
      std::string get_invalid_argument_value_repr() const;

      /// Reset
      virtual void reset();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected:

      virtual void _build_message(std::string & message_) const final;

    private:

      std::size_t _expected_nargs_;     ///< Expected number of arguments
      std::size_t _invalid_nargs_;      ///< Invalid number of arguments
      std::string _argument_name_;      ///< Invalid argument name
      std::string _invalid_value_repr_; ///< Invalid argument value representation

      VIRE_UTILITY_PAYLOAD_INTERFACE(argument_error)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::argument_error, "vire::cms::argument_error")

#endif // VIRE_CMS_ARGUMENT_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
