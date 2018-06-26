//! \file  vire/cms/monitoring_record.h
//! \brief Monitoring record
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

#ifndef VIRE_CMS_MONITORING_RECORD_H
#define VIRE_CMS_MONITORING_RECORD_H

// Standard Library:
#include <memory>

// Third Party:
// - Qt:
#include <QObject>
// - Boost:
#include <boost/variant.hpp>
#include <boost/optional.hpp>

// This project:
#include <vire/cms/image_status.h>
#include <vire/cms/monitoring.h>

namespace vire {

  namespace cms {

    class monitoring_record;

    /// \brief Qt-based signal emitter for monitoring record 
    struct monitoring_record_signal_emitter
      : public QObject
    {
      Q_OBJECT
    public:
     
      monitoring_record_signal_emitter(const monitoring_record & record_);

      const monitoring_record & get_record() const;
 
      void emit_value_change();
      
    signals:
      
      void sig_value_changed();
  
    private:
      
      const monitoring_record & _record_;
      
    };
    
    //! \brief Monitoring record
    class monitoring_record
      : public datatools::i_tree_dumpable
    {
    public:
      
      monitoring_record(bool allowed_value_ = true);
      
      virtual ~monitoring_record();
      
      void reset();
      
      bool is_allowed_value() const;

      void disallow_value();
      
      bool has_status() const;
      
      const image_status & get_status() const;
      
      image_status & grab_status();
      
      void set_status(const image_status &);
      
      bool has_value() const;
      
      bool is_boolean() const;
      
      bool is_integer() const;
      
      bool is_real() const;
      
      bool is_string() const;
      
      void set_boolean_value(const bool);
      
      void set_integer_value(const int);
      
      void set_real_value(const double);
      
      void set_string_value(const std::string &);
      
      bool get_boolean_value() const;
      
      int get_integer_value() const;
      
      double get_real_value() const;
      
      const std::string & get_string_value() const;
      
      const monitored_value_type & get_value() const;
      
      monitored_value_type & grab_value();
      
      void reset_value();
      
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

      void on_value_change();

      const monitoring_record_signal_emitter & get_emitter() const; 
      
      monitoring_record_signal_emitter & grab_emitter();
      
    private:
      
      boost::optional<monitored_value_type> & _grab_opt_value_();
      
      void _disallow_value_();
       
    private:

      // Configuration:
      bool                                  _allowed_value_ = true;

      // Working data:
      image_status                          _status_; //!< The status of the monitored object
      boost::optional<monitored_value_type> _value_;  //!< The value of the monitored datapoint

      // Qt signal emitter wrapper:
      std::unique_ptr<monitoring_record_signal_emitter> _emitter_;
 
    };

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_MONITORING_RECORD_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
