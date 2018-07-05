//! \file  vire/cms/ui/experiment_image_registry_panel.h
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

#ifndef VIRE_CMS_UI_EXPERIMENT_IMAGE_REGISTRY_PANEL_H
#define VIRE_CMS_UI_EXPERIMENT_IMAGE_REGISTRY_PANEL_H

// Third party:
// - Qt:
#include <QWidget>
#include <QObject>
#include <QList>
#include <QVariant>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QStringList>

// Forward declarations:
class QLabel;
class QComboBox;
class QComboBox;

// This project:
#include <vire/cms/experiment_image_registry.h>
#include <vire/utility/path.h>

namespace vire {

  namespace cms {
    
    namespace ui {

      /*
      struct eir_data
      {
        vire::utility::path image_path;
      };

      class eir_tree_item
      {
      public:
        explicit eir_tree_item(const eir_data & data_, eir_tree_item * parent_ = nullptr);
        ~eir_tree_item();
        void appendChild(eir_tree_item * child_);
        eir_tree_item * child(int row_);
        int childCount() const;
        int columnCount() const;
        QVariant data(int column_) const;
        int row() const;
        eir_tree_item *parentItem();
      private:
        QList<eir_tree_item*> _children_;
        // QVariant              _data_;
        eir_data              _data_;
        eir_tree_item *       _parent_ = nullptr;
      };
      
      class eir_tree_model : public QAbstractItemModel
      {
        Q_OBJECT

      public:
        explicit eir_tree_model(const QString & data_, QObject * parent_ = nullptr);
        virtual ~eir_tree_model();
        QVariant data(const QModelIndex &index_, int role_) const override;
        Qt::ItemFlags flags(const QModelIndex &index_) const override;
        QVariant headerData(int section_, Qt::Orientation orientation_,
                            int role_ = Qt::DisplayRole) const override;
        QModelIndex index(int row_, int column_,
                          const QModelIndex & parent_ = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex & index_) const override;
        int rowCount(const QModelIndex & parent_ = QModelIndex()) const override;
        int columnCount(const QModelIndex & parent_ = QModelIndex()) const override;
      private:
        void setupModelData(const QStringList & lines_, eir_tree_item * _parent_);
        eir_tree_item * _root_;
      };
      */
      
      /// \brief A widget to browse a experiment_image_registry_panel
      class experiment_image_registry_panel
        : public QWidget
      {
        Q_OBJECT

      public:
        
        /// Default constructor
        experiment_image_registry_panel(QWidget * parent_ = nullptr);

        /// Destructor
        virtual ~experiment_image_registry_panel();
 
        void set_registry(const experiment_image_registry & registry_);

      private:

        void _construct_();

        void _construct_tree_model_();

      private:

        // Configure:
        const experiment_image_registry *         _registry_   = nullptr;
        // const experiment_image_registry_emitter * _registry_emitter_ = nullptr;

        // eir_tree_model _tree_model_;
        
      };
      
    } // namespace ui

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_UI_EXPERIMENT_IMAGE_REGISTRY_PANEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
