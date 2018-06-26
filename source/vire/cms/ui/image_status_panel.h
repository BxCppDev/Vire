//! \file  vire/cms/ui/image_status_panel.h
//! \brief Image status viewer panel of a device or resource
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_CMS_UI_IMAGE_STATUS_PANEL_H
#define VIRE_CMS_UI_IMAGE_STATUS_PANEL_H

// Standard library:
#include <string>

// Third party:
// - Qt:
#include <QWidget>
#include <QObject>
// - Bayeux/datatools:
#include <bayeux/datatools/qt/led.h>

// Forward declarations:
class QLabel;

// This project:
#include <vire/cms/image_status.h>

namespace vire {

  namespace cms {
    
    namespace ui {

      /// \brief A widget to browse/edit a configuration variant registry
      class image_status_panel : public QWidget
      {
        Q_OBJECT

      public:
        
        /// Default constructor
        image_status_panel(QWidget * parent_ = nullptr);

        /// Destructor
        virtual ~image_status_panel();
       
        bool has_status() const;

        void set_status(const image_status & status_);

        bool has_no_labels() const;

        void set_no_labels(const bool);

      public slots:

        void slot_update();

        void slot_update_time();

        void slot_update_leds();

      private:

        void _construct_();

      private:

        // Configure:
        const image_status *                _status_         = nullptr;
        const image_status_signal_emitter * _status_emitter_ = nullptr;
        bool _no_labels_ = false;
        
        QLabel *  _missing_label_  = nullptr;
        QLabel *  _disabled_label_ = nullptr;
        QLabel *  _pending_label_  = nullptr;
        QLabel *  _failed_label_   = nullptr;
        QLabel *  _update_label_   = nullptr;
        QLabel *  _update_value_label_   = nullptr;
        datatools::qt::led * _missing_led_  = nullptr;
        datatools::qt::led * _disabled_led_ = nullptr;
        datatools::qt::led * _pending_led_  = nullptr;
        datatools::qt::led * _failed_led_   = nullptr;
        
      };
      
    } // namespace ui

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_UI_IMAGE_STATUS_PANEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
