//! \file  vire/cms/image.h
//! \brief Image of a device or resource
//
// Copyright (c) 2017-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <map>
#include <iostream>

// Third party:
// - Qt:
#include <QObject>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>

// This project:
#include <vire/cms/monitoring_record.h>
#include <vire/cms/resource_status_record.h>

namespace vire {

  namespace cms {

    class base_image;

    /// \brief Qt-based signal emitter for device/resource image 
    struct base_image_signal_emitter
      : public QObject
    {
      Q_OBJECT
    public:
     
      base_image_signal_emitter(const base_image & image_);

      const base_image & get_image() const;
 
      void emit_value_change();
 
      void emit_status_change();
      
    signals:
      
      void sig_value_changed();
      
      void sig_status_changed();
  
    private:
      
      const base_image & _image_;
      
    };

    //! \brief Base image of a device or a resource
    class base_image
      : public datatools::i_tree_dumpable
      , private boost::noncopyable
    {
    public:

      typedef std::map<std::string, std::string> metadata_col_type;

      //! Default constructor
      base_image();

      //! Destructor
      virtual ~base_image();

      //! Reset the image
      virtual void reset();

      /// Return the path
      virtual const std::string & get_path() const;

      /// Check if image is a device
      virtual bool is_device() const;

      /// Check if image is a resource
      virtual bool is_resource() const;

      //! Check if image can store a value
      virtual bool can_store_value() const;
     
      //! Return the non mutable monitoring record
      const monitoring_record & get_monitoring() const;

      //! Return the mutable monitoring record
      monitoring_record & grab_monitoring();

      //! Reset the monitoring record
      void reset_monitoring();

      const image_status & get_status() const;

      image_status & grab_status();

      //! Apply a status record and update the monitoring record
      void update_status(const resource_status_record & record_);

      //! Return the non mutable metadata
      const metadata_col_type & get_metadata() const;

      bool has_metadata(const std::string & key_) const;

      void set_metadata(const std::string & key_, const std::string & value_);

      void remove_metadata(const std::string & key_);

      const std::string & get_metadata(const std::string & key_) const;

      void clear_metadata();

      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

      void on_status_change();

      void on_value_change();

      const base_image_signal_emitter & get_emitter() const; 
      
      base_image_signal_emitter & grab_emitter(); 

    private :

      monitoring_record _monitoring_; //!< Monitoring record (current status and value)
      metadata_col_type _metadata_;   //!< Key/value pairs
 
      // Qt signal emitter wrapper:
      std::unique_ptr<base_image_signal_emitter> _emitter_;
      
    };

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_IMAGE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
