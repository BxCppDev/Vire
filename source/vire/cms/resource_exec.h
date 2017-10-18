//! \file  vire/cms/resource_exec.h
//! \brief Resource execution request
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

#ifndef VIRE_CMS_RESOURCE_EXEC_H
#define VIRE_CMS_RESOURCE_EXEC_H

// Standard Library:
#include <string>
#include <vector>

// This project:
#include <vire/utility/base_payload.h>
#include <vire/cms/method_argument.h>

namespace vire {

  namespace cms {

    /// \brief Resource execution command
    class resource_exec
      : public ::vire::utility::payload
     {
    public:

      /// Constructor
      resource_exec();

      /// Constructor
      resource_exec(const std::string & path_);

      /// Destructor
      virtual ~resource_exec();

      /// Check if path is set
      bool has_path() const;

      /// Set the path
      void set_path(const std::string & path_);

      /// Return the path
      const std::string & get_path() const;

      /// Reset the path
      void reset_path();

      /// Reset
      void reset();

      /// Check if input arguments are set
      bool has_input_arguments() const;

      /// Check if an input argument with given name is set
      bool has_input_argument(const std::string & name_) const;

      /// Add input argument
      void add_input_argument(const std::string & name_, const std::string & value_repr_);

      /// Add input argument
      void add_input_argument(const std::string & name_,
                              const std::string & value_repr_,
                              const std::string & meta_);

      /// Return input argument by index
      const method_argument & get_input_argument(const std::size_t index_) const;

      /// Return input argument by name
.      const method_argument & get_input_argument(const std::string & name_) const;

      /// Return the number of input arguments
      std::size_t get_number_of_input_arguments() const;

      /// Return input arguments
      const std::vector<method_argument> & get_input_arguments() const;

      /// Clear input arguments
      void clear_input_arguments();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

    private:

      std::string _path_; ///< Resource path
      std::vector<method_argument> _input_arguments_; ///< List of input arguments

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_exec)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_exec,
                                                  "vire::cms::resource_exec")

#endif // VIRE_CMS_RESOURCE_EXEC_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
