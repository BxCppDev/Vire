//! \file  vire/cms/ui/image_panel.h
//! \brief Image viewer panel of a monitored device or resource
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

#ifndef VIRE_CMS_UI_IMAGE_PANEL_H
#define VIRE_CMS_UI_IMAGE_PANEL_H

// Third party:
// - Qt:
#include <QWidget>
#include <QObject>
// - Bayeux/datatools:
#include <bayeux/datatools/introspection/data_description.h>
#include <bayeux/datatools/units.h>

// Forward declarations:
class QLabel;
class QComboBox;

// This project:
#include <vire/cms/base_image.h>
#include <vire/cms/resource_image.h>
#include <vire/cms/device_image.h>

namespace vire {

  namespace cms {
    
    namespace ui {

      class image_status_panel;

      /// \brief A widget to browse/edit a configuration variant registry
      class image_panel
        : public QWidget
      {
        Q_OBJECT

      public:
        
        /// Default constructor
        image_panel(QWidget * parent_ = nullptr);

        /// Destructor
        virtual ~image_panel();

        // QSize sizeHint() const override;
 
        void set_image(const base_image & image_);

        bool has_no_labels() const;

        void set_no_labels(const bool);

        bool has_no_value() const;

        void set_no_value(const bool);

        bool has_no_status() const;

        void set_no_status(const bool);

        bool has_no_id() const;

        void set_no_id(const bool);

      public slots:

        void slot_update();

        void slot_update_value();

        void slot_update_real_value();

      private:

        void _construct_();

      private:

        // Configure:
        bool _no_labels_ = false;
        bool _no_id_     = false;
        bool _no_value_  = false;
        bool _no_status_ = false;
        const base_image *                _image_   = nullptr;
        const base_image_signal_emitter * _image_emitter_ = nullptr;
        const datatools::introspection::data_description * _value_data_desc_ = nullptr;

        // Work:
        const datatools::units::unit_dimension * _unit_dimension_ = nullptr;
        const datatools::units::unit *           _current_unit_   = nullptr;  
        QLabel * _value_label_ = nullptr;
        QComboBox * _value_unit_combo_ = nullptr;
        image_status_panel * _status_panel_ = nullptr;
        datatools::introspection::data_type _value_type_ = datatools::introspection::DATA_TYPE_INVALID;
        
      };
      
    } // namespace ui

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_UI_IMAGE_PANEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
