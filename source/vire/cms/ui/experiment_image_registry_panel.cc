// vire/cms/ui/experiment_image_registry_panel.cc
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
#include <vire/cms/ui/experiment_image_registry_panel.h>

// Third party:
// - Qt:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout> 
#include <QComboBox> 
#include <QLabel>
#include <QTreeWidget>

namespace vire {

  namespace cms {
    
    namespace ui {
      
      /*
      eir_tree_item::eir_tree_item(const eir_data & data_, eir_tree_item * parent_)
      {
        _parent_ = parent_;
        _data_ = data_;
        return;
      }

      eir_tree_item::~eir_tree_item()
      {
        qDeleteAll(_children_);
      }

      void eir_tree_item::appendChild(eir_tree_item * item_)
      {
        _children_.append(item_);
      }

      eir_tree_item * eir_tree_item::child(int row_)
      {
        return _children_.value(row_);
      }

      int eir_tree_item::childCount() const
      {
        return _children_.count();
      }

      int eir_tree_item::columnCount() const
      {
        return _data_.count();
      }
      
      QVariant eir_tree_item::data(int column_) const
      {
        if (column_ == 0) {
          return QString(_data_.image_path.to_string().c_str());
        }
        return QVariant("");
      }

      int eir_tree_item::row() const
      {
        if (_parent_)
          return _parent_->_children_.indexOf(const_cast<eir_tree_item*>(this));

        return 0;
      }
   
      eir_tree_item * eir_tree_item::parentItem()
      {
        return _parent_;
      }

      eir_tree_model::eir_tree_model(const QString & data_, QObject *parent_)
        : QAbstractItemModel(parent_)
      {
        QList<QVariant> rootData;
        rootData << "Path" << "Image";
        _root_ = new eir_tree_item(rootData);
        setupModelData(data_.split(QString("\n")), _root_);
      }

      eir_tree_model::~eir_tree_model()
      {
        delete _root_;
      }

      int eir_tree_model::columnCount(const QModelIndex & parent_) const
      {
        if (parent_.isValid())
          return static_cast<eir_tree_item*>(parent_.internalPointer())->columnCount();
        else
          return _root_->columnCount();
      }

      QVariant eir_tree_model::data(const QModelIndex & index_, int role_) const
      {
        if (!index_.isValid())
          return QVariant();

        if (role_ != Qt::DisplayRole)
          return QVariant();

        eir_tree_item * item = static_cast<eir_tree_item*>(index_.internalPointer());

        return item->data(index_.column());
      }

      Qt::ItemFlags eir_tree_model::flags(const QModelIndex & index_) const
      {
        if (!index_.isValid())
          return 0;

        return QAbstractItemModel::flags(index_);
      }

      QVariant eir_tree_model::headerData(int section_, Qt::Orientation orientation_,
                                          int role_) const
      {
        if (orientation_ == Qt::Horizontal && role_ == Qt::DisplayRole)
          return _root_->data(section_);

        return QVariant();
      }

      QModelIndex eir_tree_model::index(int row_, int column_, const QModelIndex & parent_)
        const
      {
        if (!hasIndex(row_, column_, parent_))
          return QModelIndex();

        eir_tree_item *parentItem;

        if (!parent_.isValid())
          parentItem = _root_;
        else
          parentItem = static_cast<eir_tree_item*>(parent_.internalPointer());

        eir_tree_item *childItem = parentItem->child(row_);
        if (childItem)
          return createIndex(row_, column_, childItem);
        else
          return QModelIndex();
      }

      QModelIndex eir_tree_model::parent(const QModelIndex & index_) const
      {
        if (!index_.isValid())
          return QModelIndex();

        eir_tree_item * childItem = static_cast<eir_tree_item*>(index_.internalPointer());
        eir_tree_item * parentItem = childItem->parentItem();

        if (parentItem == _root_)
          return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem);
      }

      int eir_tree_model::rowCount(const QModelIndex & parent_) const
      {
        eir_tree_item *parentItem;
        if (parent_.column() > 0)
          return 0;

        if (!parent_.isValid())
          parentItem = _root_;
        else
          parentItem = static_cast<eir_tree_item*>(parent_.internalPointer());

        return parentItem->childCount();
      }

      void eir_tree_model::setupModelData(const QStringList & lines_, eir_tree_item * parent_)
      {
        QList<eir_tree_item*> parents;
        QList<int> indentations;
        parents << parent_;
        indentations << 0;

        int number = 0;

        while (number < lines_.count()) {
          int position = 0;
          while (position < lines_[number].length()) {
            if (lines_[number].at(position) != ' ')
              break;
            position++;
          }

          QString lineData = lines_[number].mid(position).trimmed();

          if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
              columnData << columnStrings[column];

            if (position > indentations.last()) {
              // The last child of the current parent is now the new parent
              // unless the current parent has no children.

              if (parents.last()->childCount() > 0) {
                parents << parents.last()->child(parents.last()->childCount()-1);
                indentations << position;
              }
            } else {
              while (position < indentations.last() && parents.count() > 0) {
                parents.pop_back();
                indentations.pop_back();
              }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new eir_tree_item(columnData, parents.last()));
          }

          ++number;
        }
      }
      
      */
       
      experiment_image_registry_panel::experiment_image_registry_panel(QWidget * parent_)
        : QWidget(parent_)
      {
        return;
      }
      
      experiment_image_registry_panel::~experiment_image_registry_panel()
      {
        return;
      }
  
      void experiment_image_registry_panel::set_registry(const experiment_image_registry & registry_)
      {
        _registry_ = &registry_;
        // _registry_emitter_ = &_registry_->get_emitter();
        _construct_();
        return;
      }

      void experiment_image_registry_panel::_construct_tree_model_()
      {
        
        return;
      }
      
      void experiment_image_registry_panel::_construct_()
      {
        _construct_tree_model_();
        
        QGridLayout * mainLayout = new QGridLayout;

        QLabel * testLabel = new QLabel(this);
        testLabel->setText("test experiment image registry");
        testLabel->setFixedHeight(14);
        mainLayout->addWidget(testLabel, 0, 0);

        // QTreeView * treeView = new  QTreeView;
        // treeView->setModel(&_tree_model_);
        // mainLayout->addWidget(treeView, 1, 0);
        
        // QTreeWidget *treeWidget = new QTreeWidget();
        // treeWidget->setColumnCount(2);
        // mainLayout->addWidget(treeWidget, 1, 0);

        this->setLayout(mainLayout);
        return;
      }
 
    } // namespace ui

  } // namespace cms

} // namespace vire
