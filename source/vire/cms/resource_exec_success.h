//! \file  vire/cms/resource_exec_success_response.h
//! \brief Success response to a resource execution request
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

#ifndef VIRE_CMS_RESOURCE_EXEC_SUCCESS_RESPONSE_H
#define VIRE_CMS_RESOURCE_EXEC_SUCCESS_RESPONSE_H

// Standard library:
#include <string>
#include <vector>

// This project:
#include <vire/cms/resource_base_response.h>
#include <vire/cms/method_argument.h>

namespace vire {

  namespace cms {

    /// \brief Resource execution success response
    class resource_exec_success_response
      : public resource_base_response
    {
    public:

      /// Default constructor
      resource_exec_success_response();

      /// Destructor
      virtual ~resource_exec_success_response();

      /// Check if output arguments are set
      bool has_output_arguments() const;

      /// Check if an output argument with given name is set
      bool has_output_argument(const std::string & name_) const;

      /// Return output argument by index
      const method_argument & get_output_argument(const std::size_t index_) const;

      /// Return output argument by name
      const method_argument & get_output_argument(const std::string & name_) const;

      /// Add output argument
      void add_output_argument(const std::string & name_,
                               const std::string & value_repr_);

      /// Add output argument
      void add_output_argument(const std::string & name_,
                               const std::string & value_repr_,
                               const std::string & meta_);

      /// Return the number of output arguments
      std::size_t get_number_of_output_arguments() const;

      /// Return output arguments
      const std::vector<method_argument> & get_output_arguments() const;

      /// Clear output arguments
      void clear_output_arguments();

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

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_exec_success_response)

    private:

      resource_status_record       _status_; ///< Resource status
      std::vector<method_argument> _output_arguments_; ///< List of output arguments

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_exec_success_response,
                                                  "vire::cms::resource_exec_success_response")

#endif // VIRE_CMS_RESOURCE_EXEC_SUCCESS_RESPONSE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
