// vire/cms/ui/image_panel.cc
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
#include <vire/cms/ui/image_panel.h>

// Third party:
// - Qt:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout> 
#include <QComboBox> 
#include <QLabel>
// - Bayeux:
#include <bayeux/datatools/introspection/method.h>
#include <bayeux/datatools/introspection/argument.h>
#include <bayeux/datatools/units.h>

// This project:
#include <vire/time/utils.h>
#include <vire/cms/ui/image_status_panel.h>
#include <vire/resource/base_resource_instance.h>
#include <vire/resource/method_resource_instance.h>
#include <vire/device/base_method_port_model.h>

namespace vire {

  namespace cms {
    
    namespace ui {

      image_panel::image_panel(QWidget * parent_)
        : QWidget(parent_)
      {
        return;
      }
      
      image_panel::~image_panel()
      {
        return;
      }
  
      void image_panel::set_image(const base_image & image_)
      {
        _image_ = &image_;
        _image_emitter_ = &_image_->get_emitter();
        _construct_();
        return;
      }
 
      bool image_panel::has_no_status() const
      {
        return _no_status_;
      }

      void image_panel::set_no_status(const bool no_status_)
      {
        _no_status_ = no_status_;
        return;
      }

      void image_panel::slot_update()
      {
        slot_update_value();
        return;
      }

      void image_panel::slot_update_real_value()
      {
        double value = _image_->get_monitoring().get_real_value();
        double unit_value = 1.0;
        if (_unit_dimension_ != nullptr && _value_unit_combo_ != nullptr) {
          QString unitSymbol = _value_unit_combo_->currentText();
          unit_value = datatools::units::get_unit(unitSymbol.toStdString());
        }
        double value_repr = value / unit_value;
        std::ostringstream value_repr_ss;
        value_repr_ss << value_repr;
        _value_label_->setText(value_repr_ss.str().c_str());
        return;
      }

      void image_panel::slot_update_value()
      {
        if (_value_label_ == nullptr) return;

        if (datatools::introspection::is_boolean(_value_type_)) {
          
          bool value = _image_->get_monitoring().get_boolean_value();
          std::ostringstream value_repr_ss;
          value_repr_ss << std::boolalpha << value;
          _value_label_->setText(value_repr_ss.str().c_str());
          
        } else if (datatools::introspection::is_integer(_value_type_)) {
          
          int value = _image_->get_monitoring().get_integer_value();
          std::ostringstream value_repr_ss;
          value_repr_ss << std::dec << value;
          _value_label_->setText(value_repr_ss.str().c_str());
          
        } else if (datatools::introspection::is_string(_value_type_)) {
          
          std::string value = _image_->get_monitoring().get_string_value();
          _value_label_->setText(value.c_str());
          
        } else if (datatools::introspection::is_real(_value_type_)) {
          slot_update_real_value();

        } else {
          _value_label_->setText("<unsupported type>");
        }
        
        return;
      }

      void image_panel::_construct_()
      {
        QGridLayout * main_layout = new QGridLayout;
        
        bool display_labels = !_no_labels_;
        bool display_id     = !_no_id_;
        bool display_value  = false;
        if (_image_->can_store_value()) {
          display_value = true;
        }
        bool display_status = !_no_status_;

        //         
        //     0 : labels            1 : values
        //   +-------------+-----------------------------+
        // 0 |             |        path                 | id
        //   +-------------+-----------------------------+
        // 1 |             |       value [unit]          | value
        //   +-------------+-----------------------------+
        // 2 |             |       status                | status 
        //   +-------------+-----------------------------+
        
        int labels_col = 0;
        int values_col = 1;
        int id_row     = 0;
        int value_row  = 1;
        int status_row = 2;
        if (!display_labels) {
          values_col--;
        }     
        if (!display_id) {
          value_row--;
          status_row--;
        }
        if (!display_value) {
          status_row--;
        }

        if (display_id) {
          // Identity: 
          if (display_labels) {
            QLabel * what_label = new QLabel(this);
            if (_image_->is_device()) {
              what_label->setText("Device: ");
            }
            if (_image_->is_resource()) {
              what_label->setText("Resource: ");
            }
            what_label->setFixedHeight(14);
            what_label->setStyleSheet("QLabel { text-align : left; }");
            main_layout->addWidget(what_label, id_row, labels_col);
          }
      
          QLabel * path_label = new QLabel(this);
          path_label->setText(_image_->get_path().c_str());
          path_label->setFixedHeight(14);
          path_label->setStyleSheet("QLabel { background-color : white; color : black; text-align : left; }");
          path_label->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);

          main_layout->addWidget(path_label, id_row, values_col);
        }
       
        if (display_value) {
          // Value:
          if (display_labels) {
            QLabel * value_title_label = new QLabel(this);
            value_title_label->setText("Value: ");
            value_title_label->setFixedHeight(14);
            main_layout->addWidget(value_title_label, value_row, labels_col);
          }
          
          if (_image_->is_resource()) {
 
            const resource_image * resImg
              = dynamic_cast<const resource_image *>(_image_);
            const vire::resource::resource & res = resImg->get_resource();
            const vire::resource::base_resource_instance & resInst = res.get_resource_instance();
            if (resInst.is_method()) {
              const vire::resource::method_resource_instance & methResInst
                = dynamic_cast<const vire::resource::method_resource_instance &>(resInst);
              const vire::device::base_method_port_model & methModel
                = methResInst.get_model();
              vire::utility::rw_access_type rw_access = methModel.get_rw_access();
              if (rw_access == vire::utility::RW_READABLE) {
                const datatools::introspection::method & meth = methModel.get_method();
                if (meth.has_unique_returned()) {
                  const datatools::introspection::argument & returned
                    = meth.get_unique_returned();
                  const datatools::introspection::data_description & ret_desc = returned.get_data_description();
                  _value_data_desc_ = &ret_desc;
                }
              }
            }

            if (_value_data_desc_ != nullptr) {
              
              bool supported_value = false;

              // Determine if the data type and layout is supported:
              datatools::introspection::data_layout dl  = _value_data_desc_->get_layout();
              datatools::introspection::data_type dt = _value_data_desc_->get_type();
              
              if (datatools::introspection::is_scalar(dl)) {
                if (datatools::introspection::is_boolean(dt)) {
                  supported_value = true;
                
                } else if (datatools::introspection::is_integer(dt)) {
                  supported_value = true;
        
                } else if (datatools::introspection::is_string(dt)) {
                  supported_value = true;

                } else if (datatools::introspection::is_real(dt)) {
                  supported_value = true;

                
                } else {
                  // Not supported.
                }
              }

              QHBoxLayout * value_layout = new QHBoxLayout;

              _value_label_ = new QLabel(this);
              _value_label_->setText("");
              _value_label_->setFixedHeight(24);
              _value_label_->setStyleSheet("QLabel { background-color : white; color : black; text-align : right; }");
              _value_label_->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
              value_layout->addWidget(_value_label_);
              if (! supported_value ) {
                _value_label_->setText("unsupported value");
                _value_label_->setStyleSheet("QLabel { background-color : grey; color : black; text-align : right; }");
              }
              
              if (supported_value) {
                _value_type_ = dt;
                if (datatools::introspection::is_real(dt) && _value_data_desc_->has_unit_support()) {
                  // Unit support:
                  const datatools::introspection::unit_info & uinfo = _value_data_desc_->get_unit_info();
                  if (uinfo.get_unit_support() != datatools::introspection::UNIT_SUPPORT_NONE) {
                    std::string unit_symbol;
                    std::string unit_dimension;
                    
                    if (uinfo.has_implicit_unit()) {
                      unit_symbol = uinfo.get_implicit_unit_symbol();
                      double unit_value = datatools::invalid_real();
                      DT_THROW_IF(!datatools::units::find_unit(unit_symbol, unit_value, unit_dimension),
                                  std::logic_error,
                                  "Unit '" << unit_symbol << "' is not supported!");
                    } else if (uinfo.has_explicit_unit_dimension()) {
                      unit_dimension = uinfo.get_explicit_unit_dimension_label();
                      if (uinfo.has_preferred_unit_symbol()) {
                        unit_symbol = uinfo.get_preferred_unit_symbol();
                      } else {
                        unit_symbol = datatools::units::get_default_unit_symbol_from_label(unit_dimension);
                      }
                    }

                    const datatools::units::registry & unit_reg = datatools::units::registry::const_system_registry();
                    const datatools::units::unit & value_unit
                      = unit_reg.get_unit_from_any(unit_symbol);
                    const datatools::units::unit_dimension & value_unit_dimension
                      = unit_reg.get_dimension(unit_dimension);
                    _unit_dimension_ = &value_unit_dimension;
                    std::list<std::string> unit_symbols;
                    unit_reg.build_ordered_unit_symbols(unit_dimension, unit_symbols);
                    if (unit_symbols.size()) {
                      _value_unit_combo_ = new QComboBox(this);
                      for (const auto & usymbol : unit_symbols) {
                        _value_unit_combo_->addItem(usymbol.c_str());
                      }
                      value_layout->addWidget(_value_unit_combo_);
                    }
                  }
                }                
                main_layout->addLayout(value_layout, value_row, values_col);
              } // end of supported_value
            } // end of value data description is set
          } // end of resource image
        } // end of display value
               
        if (display_status) {

          // Status:
          if (display_labels) {
            QLabel * status_label = new QLabel(this);
            status_label->setText("Status: ");
            status_label->setFixedHeight(14);
            main_layout->addWidget(status_label, status_row, labels_col);
          }
          
          _status_panel_ = new image_status_panel(this);
          _status_panel_->set_no_labels(true);
          _status_panel_->set_status(_image_->get_status());
          main_layout->addWidget(_status_panel_, status_row, values_col);
        }
        
        this->setLayout(main_layout);
        slot_update();

        
        QObject::connect(_image_emitter_, SIGNAL(sig_value_changed()),
                         this,             SLOT(slot_update_value()));
        if (_value_unit_combo_ != nullptr) {
          QObject::connect(_value_unit_combo_, SIGNAL(currentTextChanged(const QString &)),
                           this,             SLOT(slot_update_real_value()));
        }
        
        return;
      }

    } // namespace ui

  } // namespace cms

} // namespace vire
