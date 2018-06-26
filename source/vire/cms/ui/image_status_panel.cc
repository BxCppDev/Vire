// vire/cms/ui/image_status_panel.cc
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
#include <vire/cms/ui/image_status_panel.h>

// Third party:
// - Qt:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace cms {
    
    namespace ui {

      image_status_panel::image_status_panel(QWidget * parent_)
        : QWidget(parent_)
      {
        return;
      }
      
      image_status_panel::~image_status_panel()
      {
        return;
      }
     
      bool image_status_panel::has_status() const
      {
        return _status_ != nullptr;
      }

      void image_status_panel::set_status(const image_status & status_)
      {
        _status_ = &status_;
        _status_emitter_ = &_status_->get_emitter();
        _construct_();
        return;
      }
 
      bool image_status_panel::has_no_labels() const
      {
        return _no_labels_;
      }

      void image_status_panel::set_no_labels(const bool no_labels_)
      {
        _no_labels_ = no_labels_;
        return;
      }

      void image_status_panel::slot_update()
      {
        slot_update_time();
        slot_update_leds();
        return;
      }
      
      void image_status_panel::slot_update_time()
      {
        if (!_status_) return;
        if (_status_->has_timestamp()) {
          _update_value_label_->setText(vire::time::to_string(_status_->get_timestamp()).c_str());
        } else {
          _update_value_label_->setText("?");
        }
        return;
      }

      void image_status_panel::slot_update_leds()
      {
        if (!_status_) return;
        if (_status_->has_missing()) {
          _missing_led_->set_value(_status_->is_missing());
        } else {
          _missing_led_->set_indeterminate();
        }
        if (_status_->has_disabled()) {
          _disabled_led_->set_value(_status_->is_disabled());
        } else {
          _disabled_led_->set_indeterminate();
        }
        if (_status_->has_pending()) {
          _pending_led_->set_value(_status_->is_pending());
        } else {
          _pending_led_->set_indeterminate();
        }
        if (_status_->has_failed()) {
          _failed_led_->set_value(_status_->is_failed());
        } else {
          _failed_led_->set_indeterminate();
        }
        return;
      }

      void image_status_panel::_construct_()
      {
        if (!has_no_labels()) {
          _missing_label_ = new QLabel(this);
          _missing_label_->setText("M");
          _missing_label_->setFixedSize(14, 14);
          _missing_label_->setStyleSheet("QLabel { text-align : center; }");

          _disabled_label_ = new QLabel(this);
          _disabled_label_->setText("D");
          _disabled_label_->setFixedSize(14, 14);
          _disabled_label_->setStyleSheet("QLabel { text-align : center; }");

          _pending_label_ = new QLabel(this);
          _pending_label_->setText("P");
          _pending_label_->setFixedSize(14, 14);
          _pending_label_->setStyleSheet("QLabel { text-align : center; }");

          _failed_label_ = new QLabel(this);
          _failed_label_->setText("F");
          _failed_label_->setFixedSize(14, 14);
          _failed_label_->setStyleSheet("QLabel { text-align : center; }");
     
          _update_label_ = new QLabel(this);
          _update_label_->setText("Last update (UTC)");
          _update_label_->setFixedHeight(14);
          _update_label_->setStyleSheet("QLabel { text-align : left; }");
        }
        
        using datatools::qt::led;
        
        _missing_led_  = new led(led::Square, led::Red, led::Grey, led::Orange, this);
        _missing_led_->setFixedSize(14, 14);
        _missing_led_->setToolTip("Missing");
        
        _disabled_led_ = new led(led::Square, led::Yellow, led::Grey, led::Orange, this);
        _disabled_led_->setFixedSize(14, 14);
        _disabled_led_->setToolTip("Disabled");
 
        _pending_led_  = new led(led::Square, led::Green, led::Grey, led::Orange, this);
        _pending_led_->setFixedSize(14, 14);
        _pending_led_->setToolTip("Pending");
 
        _failed_led_  = new led(led::Square, led::Red, led::Grey, led::Orange, this);
        _failed_led_->setFixedSize(14, 14);
        _failed_led_->setToolTip("Failed");

        _update_value_label_ = new QLabel(this);
        _update_value_label_->setText("");
        _update_value_label_->setFixedHeight(14);
        _update_value_label_->setStyleSheet("QLabel { background-color : white; color : black; }");
        _update_value_label_->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
        _update_value_label_->setText("");
        _update_value_label_->setToolTip("Last update timestamp (UTC)");

        QGridLayout * layout = new QGridLayout;

        int row = 0;
        if (!has_no_labels()) {
          layout->addWidget(_missing_label_,  row, 0);
          layout->addWidget(_disabled_label_, row, 1);
          layout->addWidget(_pending_label_,  row, 2);
          layout->addWidget(_failed_label_,   row, 3);
          layout->addWidget(_update_label_,   row, 4);
          row++;
        }
        layout->addWidget(_missing_led_,        row, 0);
        layout->addWidget(_disabled_led_,       row, 1);
        layout->addWidget(_pending_led_,        row, 2);
        layout->addWidget(_failed_led_,         row, 3);
        layout->addWidget(_update_value_label_, row, 4);

        this->setLayout(layout);
        slot_update_time();

        QObject::connect(_status_emitter_, SIGNAL(status_changed()),
                         this,             SLOT(slot_update()));
        return;
      }

    } // namespace ui

  } // namespace cms

} // namespace vire
