//! \file  vire/cms/image.h
//! \brief Image of the current/dynamic status of a device or resource
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

#ifndef VIRE_CMS_IMAGE_H
#define VIRE_CMS_IMAGE_H

// Standard Library:
#include <string>
#include <set>
#include <vector>
#include <map>

// Third Party:
// - Boost:
#include <boost/logic/tribool.hpp>
// #include <boost/optional.hpp>
#include <boost/variant.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// This project:
#include <vire/device/logical_device.h>
#include <vire/resource/resource.h>
#include <vire/cms/status.h>
#include <vire/cms/resource_status_record.h>
#include <vire/time/utils.h>

namespace vire {

  namespace cms {

    //! \brief Dynamic status of a device or resource
    class image : public datatools::i_tree_dumpable
    {
    public:

      /// Array of pointers to daughter images
      typedef std::map<std::string, image *> daughter_dict_type;

      /// Value type aliases
      typedef boost::variant<bool,int32_t, double, std::string> value_type;
      typedef std::map<uint32_t, value_type> values_dict_type;

      enum value_timestamping_mode_type {
        VALUE_TIMESTAMPING_INVALID  = 0, //!< Invalid value timestamping mode
        VALUE_TIMESTAMPING_POLLING  = 1, //!< Instantaneous value timestamping mode (automated polling)
        VALUE_TIMESTAMPING_ONCHANGE = 2  //!< On change value timestamping mode (Pub/Sub event driven sampling)
      };

    public :

      struct base_info
      {
        std::map<std::string, std::string> metadata;
      };

      struct resource_info : public base_info
      {
        void reset();
        bool is_valid() const;
        // Attributes:
        const vire::resource::resource * res = nullptr;
      };

      struct device_info : public base_info
      {
        void reset();
        bool is_valid() const;
        // Attributes:
        std::string path;
        const vire::device::logical_device * log = nullptr;
      };

      struct pubsub_info : public base_info
      {
        void reset();
        bool is_valid() const;
        // Attributes:
        std::string domain;
        std::string address;
      };

      //! Default constructor
      image();

      //! Constructor
      image(const vire::resource::resource &);

      //! Constructor
      image(const std::string & device_path_, const vire::device::logical_device & device_log_);

      //! Destructor
      virtual ~image();

      //! Check the validity of the resource image
      bool is_valid() const;

      /// Check if the path is defined
      bool has_path() const;

      /// Return the path
      const std::string & get_path() const;

      /// Return the leaf name
      const std::string & get_leaf_name() const;

      /// Check if the group is defined
      bool has_group() const;

      /// Return the group
      const std::string & get_group() const;

      /// Set the group
      void set_group(const std::string &);

      /// Check if image is a device
      bool is_device() const;

      /// Check if image is a resource
      bool is_resource() const;

      //! Reset the device handler
      void reset_device();

      //! Reset the resource handler
      void reset_resource();

      //! Return the reference to the device handler
      const vire::device::logical_device & get_log_device() const;

      //! Return the reference to the resource handler
      const vire::resource::resource & get_resource() const;

      /// Check if the image has a parent
      bool has_parent() const;

      /// Set the parent record and register itself as a daughter of the parent record
      void set_parent(image & parent_, const std::string & daughter_name_);

      /// Return the parent
      const image & get_parent() const;

      /// Return the parent
      image & grab_parent();

      /// Add a daughter image
      void add_daughter(image &);

      /// Return the dictionary of daughter records
      daughter_dict_type & grab_daughters();

      /// Return the dictionary of daughter records
      const daughter_dict_type & get_daughters() const;

      //! Reset the resource image
      void reset();

      //! Check if timestamp is set
      bool has_timestamp() const;

      //! Set the timestamp
      void set_timestamp(const boost::posix_time::ptime &);

      //! Return the timestamp
      const boost::posix_time::ptime & get_timestamp() const;

      //! Check if missing status is known
      bool has_missing() const;

      //! Check if the resource is present
      bool is_present() const;

      //! Check if the resource not present
      bool is_missing() const;

      //! Set the missing flag
      void set_missing(bool);

      //! Reset the missing flag
      void reset_missing();

      //! Check if disabled status is known
      bool has_disabled() const;

      //! Check if the resource is enabled
      bool is_enabled() const;

      //! Check if the resource is disabled
      bool is_disabled() const;

      //! Set the disabled flag
      void set_disabled(bool);

      //! Reset the disabled flag
      void reset_disabled();

      //! Check if pending status is known
      bool has_pending() const;

      //! Check if the resource is idle
      bool is_idle() const;

      //! Check if the resource is pending
      bool is_pending() const;

      //! Set the pending flag
      void set_pending(bool);

      //! Reset the pending flag
      void reset_pending();

      //! Check if failed status is known
      bool has_failed() const;

      //! Check if the resource has no failed
      bool is_success() const;

      //! Check if the resource is in failed status
      bool is_failed() const;

      //! Set the failed flag
      void set_failed(bool);

      //! Reset the failed flag
      void reset_failed();

      //! Check if Pub/Sub status is known
      bool has_pubsub() const;

      //! Check if the resource is pubsub
      bool is_pubsub() const;

      //! Set the pubsub flag
      void set_pubsub(bool);

      //! Reset the pubsub flag
      void reset_pubsub();

      //! Apply a resource status record
      void update(const resource_status_record & record_);

      //! Make all status bits indeterminate
      void indeterminate_status();

      //! Check if image can store a value
      bool can_value() const;

      //! Check if value is set
      bool has_value(std::size_t index_ = 0) const;

      //! Reset the value
      void reset_value(std::size_t index_ = 0);

      //! Reset the values
      void reset_values();

      //! Return the size of the value
      std::size_t get_values_size() const;

      //! Return the number of the set value
      std::size_t get_number_of_set_values() const;

      //! Return the boolean value
      bool get_boolean_value(std::size_t index_ = 0) const;

      //! Set the boolean value
      void set_boolean_value(const bool value_, std::size_t index_ = 0);

      //! Return the integer value
      int32_t get_integer_value(std::size_t index_ = 0) const;

      //! Return the real value
      double get_real_value(std::size_t index_ = 0) const;

      //! Return the string value
      const std::string & get_string_value(std::size_t index_ = 0) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_ = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      void start();

      // apply_resource_change_event

      // apply_device_change_event

      // export to image log record...

    private:

      /// Set the leaf name
      void _set_leaf_name_(const std::string &);

      void _at_start_();

    private:

      // Description:
      std::string        _leaf_name_;        //!< Leaf name
      device_info        _device_;           //!< Device description
      resource_info      _resource_;         //!< Resource description
      image *            _parent_ = nullptr; //!< Handle to the parent image
      daughter_dict_type _daughters_;        //!< Dictionary of pointers to daughter records
      std::string        _group_;            //!< Optional group

      // Internal data:
      boost::posix_time::ptime    _timestamp_; //!< Last update timestamp (status or value)
      boost::logic::tribool       _missing_  = boost::logic::indeterminate; //!< Missing flag
      boost::logic::tribool       _disabled_ = boost::logic::indeterminate; //!< Disabled flag
      boost::logic::tribool       _pending_  = boost::logic::indeterminate; //!< Pending flag
      boost::logic::tribool       _failed_   = boost::logic::indeterminate; //!< Failed flag
      boost::logic::tribool       _pubsub_   = boost::logic::indeterminate; //!< PubSub activation flag

      /**
       *  Value timestamping: 'polling' mode
       *
       *                          +     +                             +
       *              +     +     :     :           +                 :     +
       *        +     :     :     :     :     +     :     +     +     :     :
       *     ---:-----:-----:-----:-----:-----:-----:-----:-----:-----:-----:----------------> t
       *
       *  Value timestamping: 'on-change' mode
       *
       *                          +---------->:   +---->:
       *                    +---->:           +-->:     :              +---->:
       *        +---------->:     :           :   :     +------------->:     +------- - - - -
       *     ---:-----------:-----:-----------:---:-----:--------------:-----:---------------> t
       *
       *
       */
      value_timestamping_mode_type _value_mode_;
      values_dict_type             _values_; //! Current array of values

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace resource

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::cms::image);
#endif // Q_MOC_RUN

#endif // VIRE_CMS_IMAGE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
