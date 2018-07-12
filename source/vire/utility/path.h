//! \file    vire/utility/path.h
//! \brief   Utilities for device/resource... path and relative path
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_PATH_H
#define VIRE_UTILITY_PATH_H

// Standard Library:
#include <iostream>
#include <string>
#include <vector>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/serialization_macros.h>

namespace vire {

  //! Nested namespace for the Vire library's utility module
  namespace utility {

    class relative_path;

    //! \brief Utilities for the manipulation of Vire resource/device paths and addresses
    //!
    //! The Vire resource/device path is supposed to be compatible with
    //! a typical URN format where the setup name corresponds to the
    //! URI scheme and the path to the resource/device is encoded like
    //! an absolute filesystem path (with the slash ('/') as separator
    //! and root symbol).
    //!
    //! Examples:
    //! \code
    //! "setup:/"
    //! "setup:/path/to/mydevice"
    //! "setup:/path/to/mydevice/this_resource"
    //! \endcode
    //!
    //! A Vire resource address is built from a resource path
    //! using the dot ('.') as separator:
    //! Examples:
    //! \code
    //! path = "setup:/path/to/mydevice/this_resource"
    //! addr = "setup.path.to.mydevice.this_resource"
    //! \endcode
    //! Addresses can be used as routing keys within RabbitMQ for example.
    //!
    class path
    {
    public:

      //! Return the separator character used after the setup name
      static const char setup_separator();

      //! Return the path root symbol
      static const char root_symbol();

      //! Return the path separator character
      static const char path_separator();

      //! Return the index separator character
      static const char index_separator();

      //! Return the address separator character
      static const char address_separator();

      //! Check if the path is the root
      static bool is_root(const std::string & path_);

      //! \brief Build the resource path from the setup and the list of path segments
      //!
      //! Format:
      //! \code
      //! "Setup" + ":"
      //! "Setup" + ":" + '/'
      //! "Setup" + ":" + '/' + "LeafSegment"
      //! "Setup" + ":" + '/' + "Segment/LeafSegment"
      //! "Setup" + ":" + '/' + "Segment/SubSegment/LeafSegment"
      //! "Setup" + ":" + '/' + "Segment/SubSegment/SubSegment/LeafSegment"
      //! \endcode
      //!
      //!
      static bool build(const std::string & setup_,
                        const std::vector<std::string> & segments_,
                        std::string & path_);

      //! \brief Return the resource path from the setup and the list of path segments
      //! \see build
      static std::string build(const std::string & setup_,
                               const std::vector<std::string> & segments_);

      //! \brief Build the resource path from the setup,
      //!        the domain (dirs) and the resource leaf name.
      //!
      //! Format:
      //! \code
      //! "Setup" + ":" + '/' + "Leaf"
      //! "Setup" + ":" + '/' + "Dir/SubDir/SubSubDir" + "/" + "Leaf"
      //! \endcode
      //!
      //!
      static bool build(const std::string & setup_,
                        const std::string & dirs_,
                        const std::string & leaf_,
                        std::string & path_);

      //! \brief Build a path as the child of a parent path
      static bool build_child(const std::string & parent_path_,
                              const std::string & leaf_,
                              std::string & path_);

      //! \brief Build a path as the child of a parent path with index (>=0)
      static bool build_child_with_index(const std::string & parent_path_,
                                         const std::string & leaf_,
                                         const uint32_t index_,
                                         std::string & path_);

      //! \brief Build a path as the child of a parent path with indexes (>=0)
      static bool build_child_with_indexes(const std::string & parent_path_,
                                           const std::string & leaf_,
                                           const std::vector<uint32_t> & indexes_,
                                           std::string & path_);

      //! \brief Build the resource path from the setup,
      //!        the directory path and the resource leaf name.
      static std::string build(const std::string & setup_,
                               const std::string & dirs_,
                               const std::string & leaf_);

      //! \brief Build the list of segments from a string representing directory path
      //!
      //! The directory path is absolute and thus must start with a slash.
      //!
      //! Examples:
      //! \code
      //! "/" -> { }
      //! "/way" -> { "way" }
      //! "/way/to" -> { "way", "to" }
      //! "/way/to/some/deep/item" -> { "way", "to", "some", "deep", "item" }
      //! \endcode
      static bool dirs_to_segments(const std::string & dirs_, std::vector<std::string> & segments_);

      //! \brief Build the directory path from a collection of segments
      //!
      //! Examples:
      //! \code
      //! { } -> "/"
      //! { "way" } -> "/way"
      //! { "way", "to" } -> "/way/to"
      //! { "way", "to", "some", "deep", "item" } -> "/way/to/some/deep/item"
      //! \endcode
      static bool segments_to_dirs(const std::vector<std::string> & segments_, std::string & dirs_);

      //! Extract the setup name, the directory path and the leaf name of a resource path
      static bool extract(const std::string & path_,
                          std::string & setup_,
                          std::vector<std::string> & segments_);

      //! Extract the setup name, the directory path and the leaf name of a resource path
      static bool extract(const std::string & path_,
                          std::string & setup_,
                          std::string & dirs_,
                          std::string & leaf_);

      //! Validate a setup label
      static bool validate_setup(const std::string & setup_);

      //! Validate a path segment
      static bool validate_path_segment(const std::string & token_);

      //! Validate a path
      static bool validate_path(const std::string & path_);

      //! Validate an address
      static bool validate_address(const std::string & address_);

      //! Convert a resource path to an address
      static bool to_address(const std::string & path_, std::string & address_);

      //! Return the address converted from a resource path
      static std::string to_address(const std::string & path_);

      //! Convert an address to a resource path
      static bool from_address(const std::string & address_, std::string & path_);

      //! Return the resource path converted from an address
      static std::string from_address(const std::string & address_);

      //! Check if a path is the parent of another one
      static bool is_parent_of(const std::string & parent_path_,
                               const std::string & child_path_,
                               bool direct_ = false);

      //! Check if a path is the parent of another one
      static bool is_child_of(const std::string & child_path_,
                              const std::string & parent_path_,
                              bool direct_ = false);

      //! Default constructor
      path();

      //! Constructor
      path(const std::string & setup_);

      //! Constructor
      path(const std::string & setup_, const std::vector<std::string> & segments_);

      //! Constructor
      path(const std::string & setup_, const std::string & dirs_, const std::string & leaf_ = "");

      //! Reset the path
      void reset();

      //! Check if path is valid
      bool is_valid() const;

      //! Check if path is root
      bool is_root() const;

      //! Return the path depth
      std::size_t get_depth() const;

      //! Return the setup identifier
      const std::string & get_setup() const;

      //! Set the setup identifier
      void set_setup(const std::string & setup_);

      //! Return the ordered list of path segments
      const std::vector<std::string> & get_segments() const;

      //! Set the ordered list of path segments
      void set_segments(const std::vector<std::string> & segments_, const bool no_check_ = false);

      //! Append a leaf to the current list of path segments
      void append_segment(const std::string & segment_, const bool no_check_ = false);
 
      //! Append a sequence of additional segments to the current list of path segments
      void append_segments(const std::vector<std::string> & segments_, const bool no_check_ = false);
 
      //! Append a relative path to the current list of path segments
      void append(const relative_path & relpath_);

      relative_path subtract(const path & other_) const;

      void truncate(const unsigned int nlevels_ = 1);
      
      bool is_parent_of(const path & other_, bool direct_ = false) const;
  
      bool is_child_of(const path & other_, bool direct_ = false) const;
      
      path make_child(const std::string & leaf_) const;
      
      path make_parent() const;

      bool has_leaf() const;

      const std::string & get_leaf() const;
     
      //! Equality operator
      bool operator==(const path & p_) const;

      //! Comparison operator
      bool operator<(const path & p_) const;
      
      relative_path operator-(const path & p_) const;
      
      path operator+(const relative_path & rp_) const;
      
      void to_string(std::string &) const;
      
      //! Return a string representation
      std::string to_string() const;

      //! Build from a string representation
      bool from_string(const std::string & repr_);
 
      friend std::ostream & operator<<(std::ostream & out_, const path & p_);
     
    private:

      std::string              _setup_;    //!< Setup identifier (as URI/URN scheme)
      std::vector<std::string> _segments_; //!< Ordered list of path segments (as URI path)

      BOOST_SERIALIZATION_BASIC_DECLARATION();
 
    };

    /// \brief A relative path made of named segments
    ///
    /// Exemple:
    /// \code
    /// ""
    /// "Bar"
    /// "Bar/Booz"
    /// "Foo/Bar/Baz_0/Test"
    /// \endcode
    class relative_path
    {
    public:

      relative_path();

      bool is_empty() const;

      void clear();

      std::size_t get_length() const;

      void prepend_segment(const std::string & head_);
 
      void prepend_segments(const std::vector<std::string> & more_);
 
      void append_segment(const std::string & leaf_);
      
      void append_segments(const std::vector<std::string> & more_);

      void cut_back(const std::size_t ncut_);

      void cut_front(const std::size_t ncut_);
        
      void set_segments(const std::vector<std::string> &);

      const std::vector<std::string> & get_segments() const;
 
      void to_string(std::string & repr_) const;
 
      std::string to_string() const;

      bool from_string(const std::string & repr_);
     
      friend std::ostream & operator<<(std::ostream & out_, const relative_path & rp_);
   
      //! Equality operator
      bool operator==(const relative_path & rp_) const;

      //! Comparison operator
      bool operator<(const relative_path & rp_) const;
     
      relative_path make_child(const std::string & leaf_) const;
     
      relative_path make_child(const std::vector<std::string> & more_) const;
           
      relative_path operator+(const relative_path & rp_) const;
 
    private:
      
      std::vector<std::string> _segments_; //!< Ordered list of path segments (as URI path)

      BOOST_SERIALIZATION_BASIC_DECLARATION();
      
    };

    const relative_path & empty_relative_path();

  } // namespace utility

} // namespace vire

#endif // VIRE_UTILITY_PATH_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
