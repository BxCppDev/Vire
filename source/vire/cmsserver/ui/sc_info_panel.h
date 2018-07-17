//! \file  vire/cmsserver/ui/sc_info_panel.h
//! \brief Subcontractor viewer panel
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

#ifndef VIRE_CMSSERVER_UI_SC_INFO_PANEL_H
#define VIRE_CMSSERVER_UI_SC_INFO_PANEL_H

// Third party:
// - Qt:
#include <QWidget>
#include <QObject>
// - Bayeux/datatools:
#include <bayeux/datatools/qt/led.h>

// Forward declarations:
class QLabel;
class QPushButton;
class QCheckBox;

// This project:
#include <vire/cmsserver/sc_info.h>

namespace vire {

  namespace cmsserver {
    
    namespace ui {

      /// \brief A widget to browse/edit a configuration variant registry
      class sc_info_panel
        : public QWidget
      {
        Q_OBJECT

      public:
       
        /// Default constructor
        sc_info_panel(QWidget * parent_ = nullptr);

        /// Destructor
        virtual ~sc_info_panel();
      
        bool has_subcontractor_info() const;

        void set_subcontractor_info(sc_info & sc_info_);

        QSize sizeHint() const override;
 
      public slots:
        
       void slot_update_auto_connect();
       
       void slot_update_connection();
       
       void slot_connect();
      
       void slot_disconnect();
      
       void slot_toggle_autoconnect();
 
      private:

        void _construct_();
 
      private:

        // Configuration:
        sc_info *                _subcontractor_info_         = nullptr;
        sc_info_signal_emitter * _subcontractor_info_emitter_ = nullptr;

        // Working UI:
        QLabel *             _sc_name_label_       = nullptr;
        QLabel *             _sc_name_value_label_ = nullptr;
        QLabel *             _sc_desc_label_       = nullptr;
        QLabel *             _sc_desc_value_label_ = nullptr;
        QLabel *             _connection_label_    = nullptr;
        datatools::qt::led * _connection_led_      = nullptr;
        QPushButton *        _connection_button_   = nullptr;
        QPushButton *        _disconnection_button_ = nullptr;
        QCheckBox *          _autoconnect_check_ = nullptr;
      };
      
    } // namespace ui

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UI_SC_INFO_PANEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
