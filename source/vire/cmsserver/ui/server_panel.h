//! \file  vire/cmsserver/ui/server_panel.h
//! \brief Server viewer panel
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

#ifndef VIRE_CMSSERVER_UI_SERVER_PANEL_H
#define VIRE_CMSSERVER_UI_SERVER_PANEL_H


// Third party:
// - Qt:
#include <QWidget>
#include <QObject>
// - Bayeux/datatools:
#include <bayeux/datatools/qt/led.h>

// Forward declarations:
class QLabel;

// This project:
#include <vire/cmsserver/server.h>

namespace vire {

  namespace cmsserver {
    
    namespace ui {

      /// \brief A widget to browse/edit a configuration variant registry
      class server_panel
        : public QWidget
      {
        Q_OBJECT

      public:
       
        /// Default constructor
        server_panel(QWidget * parent_ = nullptr);

        /// Destructor
        virtual ~server_panel();
 
        void set_server(server & server_);

        QSize sizeHint() const override;

      private slots:

        void slot_stop_server();
        
      private:

        void _construct_();

      private:

        // Configuration:
        server * _server_  = nullptr;

        // PIMPL-ized internals:
        struct pimpl_type;
        std::unique_ptr<pimpl_type> _pimpl_;
        
      };
      
    } // namespace ui

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UI_SERVER_PANEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
