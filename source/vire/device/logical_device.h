//! \file  vire/device/logical_device.h
//! \brief Vire logical device
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_DEVICE_LOGICAL_DEVICE_H
#define VIRE_DEVICE_LOGICAL_DEVICE_H

// Standard library:
#include <string>
#include <vector>
#include <map>

// Third Party:
// - Bayeux/datatools:
#include <datatools/enriched_base.h>

// This project:
#include <vire/device/device_utils.h>

namespace vire {

  namespace device {

    // Forward declarations:
    class physical_device;
    class physical_port;
    class physical_link;
    class base_device_model;

    //! \brief The logical device
    class logical_device : public datatools::enriched_base
    {
    public:

      //! Dictionary of const physical device pointers
      typedef std::map<std::string, const physical_device *> daughters_dict_type;

      //! Dictionary of const physical port pointers
      typedef std::map<std::string, const physical_port *> ports_dict_type;

      //! Dictionary of const physical link pointers
      typedef std::map<std::string, const physical_link *> links_dict_type;

      //! Default constructor
      logical_device();

      //! Destructor
      virtual ~logical_device();

      //! Check if the logical has a device model
      bool has_model() const;

      //! Set a non mutable reference to the device model
      void set_model(const base_device_model &);

      //! Return a non mutable reference to the device model
      const base_device_model & get_model() const;

      //! Build the list of all daughter physical devices' labels
      void build_daughters_labels(std::vector<std::string>&) const;

      //! Return a reference to the non mutable dictionary of daughter physical devices
      const daughters_dict_type & get_daughters() const;

      //! Check if there is a daughter physical device a given label
      bool has_daughter(const std::string & daughter_label_) const;

      //! Add a daughter physical device
      void add_daughter(const std::string & daughter_label_,
                        const physical_device & physical_);

      //! Return a non mutable reference to the daughter physical device
      const physical_device & get_daughter(const std::string & daughter_label_) const;

      //! Remove a daughter physical device
      void remove_daughter(const std::string & daughter_label_);

      //! Build the list of all physical ports' labels
      void build_ports_labels(std::vector<std::string>&) const;

      //! Return a reference to the non mutable dictionary of physical ports
      const ports_dict_type & get_ports() const;

      //! Check if there is a physical port a given label
      bool has_port(const std::string & port_label_) const;

      //! Add a physical port
      void add_port(const std::string & port_label_,
                    const physical_port & physical_);

      //! Return a non mutable reference to the physical get_port
      const physical_port & get_port(const std::string & port_label_) const;

      //! Remove a physical port
      void remove_port(const std::string & port_label_);

      //! Build the list of all physical links' labels
      void build_links_labels(std::vector<std::string>&) const;

      //! Return a reference to the non mutable dictionary of physical links
      const links_dict_type & get_links() const;

      //! Check if there is a physical link a given label
      bool has_link(const std::string & link_label_) const;

      //! Add a physical link
      void add_link(const std::string & link_label_,
                    const physical_link & physical_);

      //! Return a non mutable reference to the physical get_link
      const physical_link & get_link(const std::string & link_label_) const;

      //! Remove a physical link
      void remove_link(const std::string & link_label_);

      //! Check initialization flag
      bool is_initialized() const;

      //! Initialize
      void initialize();

      //! Reset
      void reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      bool                      _initialized_; //!< Initialization flag
      const base_device_model * _model_;       //!< Pointer to the device model from which the logical is constructed
      daughters_dict_type       _daughters_;   //!< Dictionary of daughter physical devices addressed by label
      ports_dict_type           _ports_;       //!< Dictionary of physical ports addressed by label
      links_dict_type           _links_;       //!< Dictionary of physical links addressed by label

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_LOGICAL_DEVICE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
