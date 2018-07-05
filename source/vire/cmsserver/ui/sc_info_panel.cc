// vire/cmsserver/ui/sc_info_panel.cc
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
#include <vire/cmsserver/ui/sc_info_panel.h>

// Third party:
// - Qt:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QCoreApplication>

// This project:
#include <vire/time/utils.h>
#include <vire/cms/ui/image_status_panel.h>

namespace vire {

  namespace cmsserver {
    
    namespace ui {

      sc_info_panel::sc_info_panel(QWidget * parent_)
        : QWidget(parent_)
      {
        return;
      }

      // virtual
      sc_info_panel::~sc_info_panel()
      {
        return;
      }
      
      bool sc_info_panel::has_subcontractor_info() const
      {
        return _subcontractor_info_ != nullptr;
      }

      void sc_info_panel::set_subcontractor_info(sc_info & sc_info_)
      {
        _subcontractor_info_ = &sc_info_;
        _subcontractor_info_emitter_ = &_subcontractor_info_->grab_emitter();
        _construct_();
        return;
      }
      
      QSize sc_info_panel::sizeHint() const
      {
        return QSize(200,100);
      }
      
      void sc_info_panel::_construct_()
      {
        using datatools::qt::led;

        _sc_name_label_ = new QLabel(this);
        _sc_name_label_->setText("<b>Subcontractor</b>");
        _sc_name_label_->setFixedHeight(14);
        _sc_name_label_->setFixedWidth(100);
        _sc_name_label_->setStyleSheet("QLabel { text-align : left; }");
       
        _sc_name_value_label_ = new QLabel(this);
        _sc_name_value_label_->setFixedHeight(14);
        _sc_name_value_label_->setText(_subcontractor_info_->get_name().c_str());

        // std::string tooltipText;
        // if (
        // _sc_name_value_label_->setTooltip(_subcontractor_info_->get_name().c_str());
        _sc_name_value_label_->setStyleSheet("QLabel { background-color : white; color : black; }");
        _sc_name_value_label_->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);

        QHBoxLayout * name_layout = new QHBoxLayout;
        name_layout->addWidget(_sc_name_label_);
        name_layout->addWidget(_sc_name_value_label_);
        
        _connection_label_ = new QLabel(this);
        _connection_label_->setText("Connected");
        _connection_label_->setFixedHeight(14);
        _connection_label_->setStyleSheet("QLabel { text-align : left; }");

        _connection_led_  = new led(led::Square, led::Green, led::Grey, this);
        _connection_led_->setFixedSize(14, 14);

        QHBoxLayout * connection_layout = new QHBoxLayout;
        connection_layout->addWidget(_connection_led_);
        connection_layout->addWidget(_connection_label_);

        QFrame * mounted_device_frame = new QFrame(this);
        mounted_device_frame->setFrameStyle(QFrame::Box);
        mounted_device_frame->setLineWidth(1);
        
        QVBoxLayout * mounted_panel_layout = new QVBoxLayout;
        
        QGridLayout * mounted_devices_layout = new QGridLayout;
        std::set<std::string> mdev_names;
        _subcontractor_info_->build_mounted_devices(mdev_names);
        int dev_counter = 0;
        int row_skip = 1;

        QLabel * mdLbl = new QLabel(this);
        mdLbl->setText("<b>Mounted devices</b>");
        mdLbl->setFixedHeight(14);

        QLabel * mLbl = new QLabel(this);
        mLbl->setText("<b> M</b>");
        mLbl->setFixedSize(14, 14);

        QLabel * dLbl = new QLabel(this);
        dLbl->setText("<b> D</b>");
        dLbl->setFixedSize(14, 14);

        QLabel * pLbl = new QLabel(this);
        pLbl->setText("<b> P</b>");
        pLbl->setFixedSize(14, 14);

        QLabel * fLbl = new QLabel(this);
        fLbl->setText("<b> F</b>");
        fLbl->setFixedSize(14, 14);

        QLabel * uLbl = new QLabel(this);
        uLbl->setText("<b>Last update (UTC)</b>");
        uLbl->setFixedHeight(14);
        
        QHBoxLayout * titleLayout = new QHBoxLayout;
        titleLayout->addWidget(mLbl);
        titleLayout->addWidget(dLbl);
        titleLayout->addWidget(pLbl);
        titleLayout->addWidget(fLbl);
        titleLayout->addWidget(uLbl);
        mounted_devices_layout->addWidget(mdLbl, 0, 0);
        mounted_devices_layout->addLayout(titleLayout, 0, 1);

        using vire::cms::ui::image_status_panel;
        for (const auto & mdev_name : mdev_names) {
          sc_info::device_info & devInfo = _subcontractor_info_->grab_mounted_device_info(mdev_name);
          image_status_panel * devStatusPanel = new vire::cms::ui::image_status_panel;
          devStatusPanel->set_no_labels(true);    
          devStatusPanel->set_status(devInfo.status);
          QHBoxLayout * dev_layout = new QHBoxLayout;
          QLabel * dev_path_value_label = new QLabel(this);
          dev_path_value_label->setText(mdev_name.c_str());
          dev_path_value_label->setFixedHeight(14);
          dev_path_value_label->setStyleSheet("QLabel { background-color : white; color : black; }");
          dev_path_value_label->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
          mounted_devices_layout->addWidget(dev_path_value_label, dev_counter + row_skip, 0, 1, 1);
          mounted_devices_layout->addWidget(devStatusPanel,       dev_counter + row_skip, 1, 1, 1);
          dev_counter++;
          devStatusPanel->slot_update();
          QObject::connect(&devInfo.status.grab_emitter(), SIGNAL(status_changed()),
                           devStatusPanel, SLOT(slot_update()));

        }
        mounted_panel_layout->addLayout(mounted_devices_layout);
        mounted_device_frame->setLayout(mounted_panel_layout);

        QVBoxLayout * main_layout = new QVBoxLayout;
        main_layout->addLayout(name_layout);
        main_layout->addLayout(connection_layout);       
        main_layout->addWidget(mounted_device_frame);
        
        this->setLayout(main_layout);
        //        this->repaint();
        // QCoreApplication::processEvents();
        slot_update_connection();

        QObject::connect(_subcontractor_info_emitter_, SIGNAL(connection_changed()),
                         this, SLOT(slot_update_connection()));
 
        return;
      }
        
      void sc_info_panel::slot_update_connection()
      {
        if (!_subcontractor_info_) return;
        if (_subcontractor_info_->is_connected()) {
          _connection_led_->set_value(true);
        } else {
          _connection_led_->set_value(false);
        }
        return;
      }
     
    } // namespace ui

  } // namespace cmsserver

} // namespace vire
