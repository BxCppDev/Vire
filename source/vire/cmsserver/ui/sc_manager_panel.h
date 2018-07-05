//! \file  vire/cmsserver/ui/sc_manager_panel.h
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

#ifndef VIRE_CMSSERVER_UI_SC_MANAGER_PANEL_H
#define VIRE_CMSSERVER_UI_SC_MANAGER_PANEL_H

// Third party:
// - Qt:
#include <QWidget>
#include <QObject>
// - Bayeux/datatools:
#include <bayeux/datatools/qt/led.h>

// Forward declarations:
class QLabel;

// This project:
#include <vire/cmsserver/sc_manager.h>

namespace vire {

  namespace cmsserver {
    
    namespace ui {

      /// \brief A widget to browse/edit a configuration variant registry
      class sc_manager_panel
        : public QWidget
      {
        Q_OBJECT

      public:
       
        /// Default constructor
        sc_manager_panel(QWidget * parent_ = nullptr);

        /// Destructor
        virtual ~sc_manager_panel();
      
        bool has_manager() const;

        void set_manager(sc_manager & sc_manager_);

        QSize sizeHint() const override;
 
      public slots:
        
	// void slot_update_connection();
 
      private:

        void _construct_();
 
      private:

        // Configuration:
        sc_manager *                _manager_         = nullptr;
        // sc_manager_signal_emitter * _manager_emitter_ = nullptr;

   
      };
      
    } // namespace ui

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UI_SC_MANAGER_PANEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
