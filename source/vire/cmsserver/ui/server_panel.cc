// vire/cmsserver/ui/server_panel.cc
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

// Ourselves:
#include <vire/cmsserver/ui/server_panel.h>

// Third party:
// - Qt:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QTimer>
// - Bayeux/datatools:
#include <bayeux/datatools/service_tools.h>

// This project:
#include <vire/time/utils.h>
#include <vire/cmsserver/ui/sc_manager_panel.h>

namespace vire {

  namespace cmsserver {
    
    namespace ui {

      struct server_panel::pimpl_type
      {
        server_panel * panel = nullptr;
        QTimer * timer = nullptr;
        pimpl_type(server_panel * panel_, QWidget * parent_)
        {
          panel = panel_;
          // timer = new QTimer(parent_);
          // timer->start(1000);
          return;
        }
      };
        
      server_panel::server_panel(QWidget * parent_)
        : QWidget(parent_)
      {
        _pimpl_.reset(new pimpl_type(this, parent_));
        return;
      }

      // virtual
      server_panel::~server_panel()
      {
        return;
      }

      void server_panel::set_server(server & server_)
      {
        _server_ = &server_;
        // _server_emitter_ = &_subcontractor_info_->grab_emitter();
        _construct_();
        return;
      }

      void server_panel::slot_stop_server()
      {
        QMessageBox::StandardButtons buttons = QMessageBox::No | QMessageBox::Yes;
        QMessageBox::StandardButton default_button = QMessageBox::NoButton;
        QMessageBox::StandardButton choice = QMessageBox::question((QWidget*)nullptr,
                                                                   "Stop the CMS/Server",
                                                                   "Do you really want to stop the CMS/Server?",
                                                                   buttons,
                                                                   default_button);
        if (choice != QMessageBox::Cancel) {
          if (choice == QMessageBox::Yes) {
            _server_->stop();
          }
        }
        return;
      }
      
      QSize server_panel::sizeHint() const
      {
        return QSize(200,100);
      }
      
      void server_panel::_construct_()
      {
        using datatools::qt::led;

        QLabel * server_name_label = new QLabel(this);
        server_name_label->setText("<b>CMS Server : </b>");
        server_name_label->setFixedHeight(14);
        server_name_label->setFixedWidth(120);

        QLabel * server_name_value_label = new QLabel(this);
        server_name_value_label->setText(_server_->get_setup_id().to_string().c_str());
        server_name_value_label->setFixedHeight(14);
        server_name_value_label->setStyleSheet("QLabel { background-color : white; color : black; }");
        server_name_value_label->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);

        QLabel * service_tab_title_label = new QLabel(this);
        service_tab_title_label->setText("<b>Services : </b>");
        service_tab_title_label->setFixedHeight(14);

        QGridLayout * grid_layout = new QGridLayout;
        grid_layout->addWidget(server_name_label, 0, 0);
        grid_layout->addWidget(server_name_value_label, 0, 1);
        grid_layout->addWidget(service_tab_title_label, 1, 0);

        QHBoxLayout * buttons_layout = new QHBoxLayout;
        QPushButton * stop_button = new QPushButton("&Stop", this);

        QObject::connect(stop_button, SIGNAL (released()),
                         this, SLOT (slot_stop_server()));
        
        buttons_layout->addWidget(stop_button);

        datatools::service_manager & serverServices = _server_->grab_services();
        const datatools::service_dict_type & serverServiceDict = serverServices.get_local_services();
        
        QTabWidget * services_tabs = new QTabWidget(this);

        // Add a tab for the "subcontractors" service:
        std::string scMgrName;
        if (datatools::find_service_name_with_id(serverServiceDict, "vire::cmsserver::sc_manager", scMgrName)) {
          vire::cmsserver::sc_manager & scMgr = serverServices.grab<vire::cmsserver::sc_manager>(scMgrName);
          sc_manager_panel * scMgrPanel = new sc_manager_panel(this);
          scMgrPanel->set_manager(scMgr);
          std::string display_name = scMgrName;
          if (scMgr.has_display_name()) {
            display_name = scMgr.get_display_name();
          }
          services_tabs->addTab(scMgrPanel, display_name.c_str());
        } else {
          QMessageBox::StandardButtons buttons = QMessageBox::Ignore | QMessageBox::Abort;
          QMessageBox::StandardButton default_buttons = QMessageBox::Ignore;
          QMessageBox::StandardButton answer
            = QMessageBox::question(this,
                                    "CMS/Server Error",
                                    "No subcontractor service has been found!",
                                    buttons, default_buttons);
          if (answer == QMessageBox::Abort) {
            _server_->stop();
          }
        }
        
        QVBoxLayout * main_layout = new QVBoxLayout;
        main_layout->addLayout(grid_layout);
        main_layout->addWidget(services_tabs);
        main_layout->addLayout(buttons_layout);

        this->setLayout(main_layout);
        return;
      }

    } // namespace ui

  } // namespace cmsserver

} // namespace vire
