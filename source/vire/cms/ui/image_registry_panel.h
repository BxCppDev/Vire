//! \file  vire/cms/ui/image_registry_panel.h
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

#ifndef VIRE_CMS_UI_IMAGE_REGISTRY_PANEL_H
#define VIRE_CMS_UI_IMAGE_REGISTRY_PANEL_H

// Standard Library:
#include <string>
#include <map>

// Third party:
// - Qt:
#include <QWidget>
#include <QObject>
#include <QList>
#include <QVariant>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QStringList>
#include <QStyledItemDelegate>
#include <QMetaType>

// Forward declarations:
class QLabel;
class QComboBox;
class QComboBox;

// This project:
#include <vire/cms/image_registry.h>
#include <vire/utility/path.h>
// #include <vire/cms/ui/image_panel.h>

namespace vire {

  namespace cms {
    
    namespace ui {

      class image_panel;
      
      struct image_registry_data
      {
      public:
         
        enum data_type {
          DATA_ROOT = 0,
          DATA_DEVICE_UNTRACKED = 1,
          DATA_DEVICE = 2,
          DATA_RESOURCE = 3 
        };
        
        enum column_type {
          COL_PATH      = 0,
          COL_IMAGE     = 1,
          COL_COUNT     = 2
        };
        
        data_type               type;
        vire::utility::path     image_path;
        std::string             label;
        vire::cms::base_image * image;
        
      };
    }
  }
}
  
// template int qRegisterMetaType<image_registry_data>("vire::cms::ui::image_registry_data");
Q_DECLARE_METATYPE(vire::cms::ui::image_registry_data)

namespace vire {

  namespace cms {
    
    namespace ui {
      
      class image_registry_tree_model;
      
      /// \brief Node item
      // https://www.informit.com/articles/article.aspx?p=1405547&seqNum=3
      class image_registry_tree_item
      {
      public:
        
        explicit image_registry_tree_item(const image_registry_data & data_,
                                          image_registry_tree_item * parent_ = nullptr);
        
        ~image_registry_tree_item();
         
        const image_registry_data & get_data() const;
        
        image_registry_data & grab_data();
        
        image_registry_tree_item * parentItem();
       
        void appendChild(image_registry_tree_item * child_);
        
        image_registry_tree_item * child(int row_);
        
        int childCount() const;
        
        int columnCount() const;
        
        int row() const;
         
      private:
        
        image_registry_data              _data_;
        image_registry_tree_item *       _parent_ = nullptr;
        QList<image_registry_tree_item*> _children_;
        
        friend class image_registry_tree_model;
      };

      class image_registry_tree_model;

      // https://stackoverflow.com/questions/16660292/qt-using-custom-qitemdelegate-for-qtableview
      // https://doc.qt.io/qt-5/qtwidgets-itemviews-stardelegate-stardelegate-h.html
      class image_registry_delegate
        : public QStyledItemDelegate
      {
          Q_OBJECT
        
      public:
        
        explicit image_registry_delegate(image_registry_tree_model * model_,
                                         QAbstractItemView * view_,
                                         QObject * parent_ = nullptr);
        
        virtual ~image_registry_delegate();

        virtual void paint(QPainter * painter_,
                           const QStyleOptionViewItem & option_,
                           const QModelIndex & index_) const override;
        
        virtual QSize sizeHint(const QStyleOptionViewItem & option_,
                               const QModelIndex & index_) const override;
        
        // virtual QWidget* createEditor(QWidget * parent_,
        //                               const QStyleOptionViewItem & option_,
        //                               const QModelIndex & index_);
        
        // virtual void setEditorData(QWidget * editor_,
        //                            const QModelIndex & index_);
        
        // virtual void setModelData(QWidget * editor_,
        //                           QAbstractItemModel * model_,
        //                           const QModelIndex & index_);
        
        // virtual void updateEditorGeometry(QWidget * editor_,
        //                                   const QStyleOptionViewItem & option_,
        //                                   const QModelIndex & index_);

      private:

        image_registry_tree_model * _model_ = nullptr;
        QAbstractItemView * _view_ = nullptr;
        std::map<std::string,image_panel*> _imgpanels_;
        
      };
      
      /// \brief Image registry tree model
      class image_registry_tree_model
        : public QAbstractItemModel
      {
        Q_OBJECT

      public:
        
        explicit image_registry_tree_model(image_registry & imreg_,
                                           QObject * parent_ = nullptr);
        
        virtual ~image_registry_tree_model();

        void set_root(image_registry_tree_item * item_);
        
        QModelIndex index(int row_,
                          int column_,
                          const QModelIndex & parent_ = QModelIndex()) const override;
        
        QModelIndex parent(const QModelIndex & index_) const override;
         
        int rowCount(const QModelIndex & parent_ = QModelIndex()) const override;
        
        int columnCount(const QModelIndex & parent_ = QModelIndex()) const override;
       
        QVariant data(const QModelIndex & index_, int role_) const override;
       
        QVariant headerData(int section_,
                            Qt::Orientation orientation_,
                            int role_ = Qt::DisplayRole) const override;
        
        // Qt::ItemFlags flags(const QModelIndex &index_) const override;

        void construct();
        
        void insert_icons(const image_registry_data::data_type type_,
                          const QIcon & icons_); 
        
        image_registry_tree_item * item_from_index(const QModelIndex & index_) const;

        int get_path_max_depth() const;
        int get_path_max_depth_leaf_width() const;
     
      private:
        
        void _setup_model_data_(image_registry & imreg_);

      private:
        image_registry * _imreg_ = nullptr;
        image_registry_tree_item * _root_ = nullptr;
        std::map<image_registry_data::data_type,QIcon> _type2icon_;
        int _path_max_depth_ = 0;
        int _path_max_depth_leaf_width_ = 0;
      };
       
      /// \brief A widget to browse a image_registry_panel
      class image_registry_panel
        : public QWidget
      {
        Q_OBJECT

      public:
        
        /// Default constructor
        image_registry_panel(QWidget * parent_ = nullptr);

        /// Destructor
        virtual ~image_registry_panel();
 
        void set_registry(image_registry & registry_);

      private:

        void _construct_();

        void _construct_tree_model_();

      private:

        // Configure:
        image_registry * _registry_ = nullptr;
        // const image_registry_emitter * _registry_emitter_ = nullptr;

        image_registry_tree_model * _tree_model_ = nullptr;
        image_registry_delegate   * _delegate_ = nullptr;
      };
      
    } // namespace ui

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_UI_IMAGE_REGISTRY_PANEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
