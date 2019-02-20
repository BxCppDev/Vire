// vire/cmsserver/ui/sc_manager_panel.cc
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
#include <vire/cmsserver/ui/sc_manager_panel.h>

// Third party:
// - Qt:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QCoreApplication>
#include <QTabWidget>

// This project:
#include <vire/time/utils.h>
#include <vire/cmsserver/ui/sc_driver_panel.h>

namespace vire {

  namespace cmsserver {
    
    namespace ui {

      sc_manager_panel::sc_manager_panel(QWidget * parent_)
        : QWidget(parent_)
      {
        return;
      }

      // virtual
      sc_manager_panel::~sc_manager_panel()
      {
        return;
      }
      
      bool sc_manager_panel::has_manager() const
      {
        return _manager_ != nullptr;
      }

      void sc_manager_panel::set_manager(sc_manager & sc_manager_)
      {
        _manager_ = &sc_manager_;
        //_manager_emitter_ = &_manager_->grab_emitter();
        _construct_();
        return;
      }
      
      QSize sc_manager_panel::sizeHint() const
      {
        return QSize(200,100);
      }
      
      void sc_manager_panel::_construct_()
      {
        using datatools::qt::led;

        QTabWidget * scTabs = new QTabWidget(this);

        std::set<std::string> sc_names;
        _manager_->build_subcontractor_names(sc_names);

        using vire::cmsserver::sc_driver;
        using vire::cmsserver::ui::sc_driver_panel;
        for (const std::string & sc_name : sc_names) {
          sc_driver & scDriver = _manager_->grab_driver(sc_name);
          sc_driver_panel * scDriverPanel = new sc_driver_panel(this);
          scDriverPanel->set_driver(scDriver);
          std::string display_name = sc_name;
          if (scDriver.has_display_name()) {
            display_name = scDriver.get_display_name();
          }
          scTabs->addTab(scDriverPanel, display_name.c_str());
        }

        QVBoxLayout * main_layout = new QVBoxLayout;
        main_layout->addWidget(scTabs);
        
        this->setLayout(main_layout);

        // slot_update_connection();

        /*
        QObject::connect(_subcontractor_info_emitter_, SIGNAL(connection_changed()),
                         this, SLOT(slot_update_connection()));
        */
        
        return;
      }
        
      // void sc_manager_panel::slot_update_connection()
      // {
      //   if (!_subcontractor_info_) return;
      //   if (_subcontractor_info_->is_connected()) {
      //     _connection_led_->set_value(true);
      //   } else {
      //     _connection_led_->set_value(false);
      //   }
      //   return;
      // }
     
    } // namespace ui

  } // namespace cmsserver

} // namespace vire
