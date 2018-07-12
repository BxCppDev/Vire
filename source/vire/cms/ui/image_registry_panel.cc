// vire/cms/ui/image_registry_panel.cc
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
#include <vire/cms/ui/image_registry_panel.h>

// Third party:
// - Qt:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout> 
#include <QComboBox> 
#include <QLabel>
#include <QTreeWidget>
#include <QIcon>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QHeaderView>

// This project:
#include <vire/device/manager.h>
#include <vire/cms/ui/utils.h>
#include <vire/cms/ui/image_panel.h>

namespace vire {

  namespace cms {
    
    namespace ui {
      
      image_registry_delegate::image_registry_delegate(image_registry_tree_model * model_,
                                                       QAbstractItemView * view_,
                                                       QObject * parent_)
        : QStyledItemDelegate(parent_)
        , _model_(model_)
        , _view_(view_)
      {
        return;
      }
    
      image_registry_delegate::~image_registry_delegate()
      {
        return;
      }

      // virtual
      QSize image_registry_delegate::sizeHint(const QStyleOptionViewItem & option_,
                                              const QModelIndex & index_) const
      {
        unsigned int cellSize
          = vire::cms::ui::display_context::get_instance().get_row_height();
        QSize originalSize = QStyledItemDelegate::sizeHint(option_, index_);
        if (index_.isValid() && index_.column() == image_registry_data::COL_PATH) {
          std::cerr << "***** devel ***** originalSize = " << originalSize.width() << "x" << originalSize.height()  << std::endl;
          int path_max_depth = _model_->get_path_max_depth();
          int path_max_depth_leaf_width = _model_->get_path_max_depth_leaf_width();
          const QFont & f = option_.font;
          QFontMetrics fm(f);
          int leafTextWidthInPixels = fm.width('a') * path_max_depth_leaf_width;
          std::cerr << "***** devel *****   path_max_depth_leaf_width = " << path_max_depth_leaf_width  << std::endl;      
          std::cerr << "***** devel *****   leafTextWidthInPixels     = " << leafTextWidthInPixels  << std::endl;
          // const QString & text = option_.text;
          // int textWidthInPixels = fm.width(text);
          // std::cerr << "***** devel *****   text     = '" << text.toStdString()  << "' of width = " << textWidthInPixels << std::endl;
          QSize szHint = originalSize;
          szHint.setHeight(cellSize);
       }
        return QStyledItemDelegate::sizeHint(option_, index_);  
      }
 
      void image_registry_delegate::paint(QPainter * painter_,
                                          const QStyleOptionViewItem & option_,
                                          const QModelIndex & index_) const
      {
        // std::cerr << "***** DEVEL ***** image_registry_delegate::paint: Entering..." << std::endl;
        if (index_.isValid() && index_.column() == image_registry_data::COL_IMAGE) {
          image_registry_tree_item * theItem = _model_->item_from_index(index_);
          image_registry_data & imgData = theItem->grab_data();
          std::string path_key = imgData.image_path.to_string();
          if (imgData.type == image_registry_data::DATA_DEVICE
              || imgData.type == image_registry_data::DATA_RESOURCE) {   
            std::cerr << "***** DEVEL ***** image_registry_delegate::paint: Display image." << std::endl;
            if (! _imgpanels_.count(path_key)) {
              image_panel * imgPanel = new image_panel() ; //this->parent());
              imgPanel->set_no_labels(true);
              imgPanel->set_no_value(true);
              imgPanel->set_no_id(true);
              imgPanel->set_image(*imgData.image);
              const_cast<image_registry_delegate*>(this)->_imgpanels_[path_key] = imgPanel;
              std::cerr << "***** DEVEL ***** image_registry_delegate::paint: "
                        << "Created new panel for path '" << path_key << "'..." << std::endl;
            }
            std::map<std::string, image_panel*>::const_iterator found = _imgpanels_.find(path_key);
            if (found != _imgpanels_.end()) {
              std::cerr << "***** DEVEL ***** image_registry_delegate::paint: Found panel for path '"
                        << path_key << "'..." << std::endl;
              image_panel * imgPanel = found->second;
              // QPainter painter();
              // imgPanel->setGeometry(option_.rect);
              // imgPanel->render(&painter,
              //                  QPoint(option_.rect.x(), option_.rect.y()),
              //                  QRegion(0, 0, option_.rect.width(), option_.rect.height()));

              painter_->save();

              painter_->setPen(QPen( Qt::red, 1 ));
              painter_->drawRect(option_.rect);

              std::cerr << "***** DEVEL ***** image_registry_delegate::paint: "
                        << "rect.x      = " << option_.rect.x()
                        << std::endl;
              std::cerr << "***** DEVEL ***** image_registry_delegate::paint: "
                        << "rect.y      = " << option_.rect.y()
                        << std::endl;
              std::cerr << "***** DEVEL ***** image_registry_delegate::paint: "
                        << "rect.width  = " << option_.rect.width()
                        << std::endl;
              std::cerr << "***** DEVEL ***** image_registry_delegate::paint: "
                        << "rect.height = " << option_.rect.height()
                        << std::endl;
              std::cerr << "***** DEVEL ***** image_registry_delegate::paint: "
                        << "rect.height = " << option_.rect.height()
                        << std::endl;
              imgPanel->setGeometry(option_.rect);
              // imgPanel->resize(option_.rect.size());
              // imgPanel->render(painter_); // painter_->deviceTransform().map(option_.rect.topLeft()));
           
              // imgPanel->render(painter_, painter_->deviceTransform().map(option_.rect.topLeft()));
              //painter_->translate(option_.rect.topLeft());
              QPaintDevice * original_pdev_ptr = painter_->device();
              painter_->end();
              QPainter painter();
              QPoint targetOffset(option_.rect.x(), option_.rect.y());
              QRegion sourceRegion(0, 0, option_.rect.width(), option_.rect.height()+130);
              //QPoint(option_.rect.x(), option_.rect.y() +25),
              //QRegion(0, 0, 0, 0),
              //QRegion(0, 0, option_.rect.width(), option_.rect.height()),
              imgPanel->render(painter_->device(),
                               targetOffset,
                               sourceRegion,
                               QWidget::RenderFlag::DrawChildren);   
              painter_->begin(original_pdev_ptr); 

              // imgPanel->render(painter_->device(),
              //                  QPoint(option_.rect.x(), option_.rect.y()));
              
              // imgPanel->render(painter_->device(),
              //                  QPoint(option_.rect.x(), option_.rect.y()),
              //                  QPoint(0, 0),
              //                  // QRegion(0, 0, 0, 0),
              //                  // QRegion(0, 0, 0, option_.rect.height()),
              //                  // QRegion(0, 0, option_.rect.width(), option_.rect.height()),
              //                  QRegion(option_.rect.x(), option_.rect.y(), option_.rect.width(), option_.rect.height()),
              //                  QWidget::DrawChildren);

              // painter_->restore();
              
              /*
              QPaintDevice * original_pdev_ptr = painter_->device();
              if (option_.state & QStyle::State_Selected) {
                painter_->fillRect(option_.rect, option_.palette.highlight());
              }
              imgPanel->setGeometry(option_.rect);
              painter_->end(); 
              imgPanel->render(painter_->device(),
                               QPoint(option_.rect.x(), option_.rect.y()),
                               QRegion(0, 0, option_.rect.width(), option_.rect.height()),
                               QWidget::DrawChildren);
              painter_->begin(original_pdev_ptr);
              painter_->drawEllipse(0,0, 10,10);   
              */
            }
          }
        } else {
          QStyledItemDelegate::paint(painter_, option_, index_);
        }
        // std::cerr << "***** DEVEL ***** image_registry_delegate::paint: Exiting." << std::endl;
        return;
      }

      /*
      QWidget * image_registry_delegate::createEditor(QWidget * parent_,
                                                      const QStyleOptionViewItem & option_,
                                                      const QModelIndex & index_)
      {
        std::cerr << "***** DEVEL ***** image_registry_delegate::createEditor: Entering..." << std::endl;
        if (index_.isValid() && index_.column() == image_registry_data::COL_IMAGE) {        
          image_registry_tree_item * theItem = _model_->item_from_index(index_);
          const image_registry_data & imgData = theItem->get_data();  
          if (imgData.type == image_registry_data::DATA_DEVICE
              || imgData.type == image_registry_data::DATA_RESOURCE) {
            std::cerr << "***** DEVEL ***** image_registry_delegate::createEditor: Special delegate widget..." << std::endl;
            image_panel * imgPanel = new image_panel(parent_);
            imgPanel->set_no_labels(true);
            imgPanel->set_no_value(true);
            imgPanel->set_no_id(true);
            return imgPanel;
          }
        }
        std::cerr << "***** DEVEL ***** image_registry_delegate::createEditor: Exiting." << std::endl;
        return QStyledItemDelegate::createEditor(parent_, option_, index_);
      }
      */
      
      /*
      void image_registry_delegate::setEditorData(QWidget * editor_,
                                                  const QModelIndex & index_)
      {
        std::cerr << "***** DEVEL ***** image_registry_delegate::setEditorData: Entering..." << std::endl;
        if (index_.isValid() && index_.column() == image_registry_data::COL_IMAGE) {
          // image_registry_data & imgDataValue
          //   = index_.model()->data(index_, Qt::DisplayRole).value<image_registry_data>();
          image_registry_tree_item * theItem = _model_->item_from_index(index_);
          image_registry_data & imgData = theItem->grab_data();
          if (imgData.type == image_registry_data::DATA_DEVICE
              || imgData.type == image_registry_data::DATA_RESOURCE) {  
            image_panel * imgPanel = static_cast<image_panel*>(editor_);
            imgPanel->set_image(*imgData.image);
          }
        } else {
          QStyledItemDelegate::setEditorData(editor_, index_);
        }
        std::cerr << "***** DEVEL ***** image_registry_delegate::setEditorData: Exiting." << std::endl;
        return;
      }
      */
      /*
      void image_registry_delegate::setModelData(QWidget * editor_,
                                                 QAbstractItemModel * model_,
                                                 const QModelIndex & index_)
      {
        std::cerr << "***** DEVEL ***** image_registry_delegate::setModelData: Entering..." << std::endl;
        if (index_.isValid() && index_.column() == image_registry_data::COL_IMAGE) {
          // // image_registry_data & imgDataValue
          // //   = index_.model()->data(index_, Qt::DisplayRole).value<image_registry_data>();
          // image_registry_tree_item * theItem = _model_->item_from_index(index_);
          // image_registry_data & imgData = theItem->grab_data();
          // if (imgData.type == image_registry_data::DATA_DEVICE
          //     || imgData.type == image_registry_data::DATA_RESOURCE) {  
          //   image_panel * imgPanel = static_cast<image_panel*>(editor_);
          //   imgPanel->set_image(*imgData.image);
          // }
        } else {
          QStyledItemDelegate::setModelData(editor_, model_, index_);
        }
        std::cerr << "***** DEVEL ***** image_registry_delegate::setModelData: Exiting." << std::endl;
        return;
      }
      */
      /*
      void image_registry_delegate::updateEditorGeometry(QWidget * editor_,
                                                         const QStyleOptionViewItem & option_,
                                                         const QModelIndex & index_)
      {
        if (index_.isValid() && index_.column() == image_registry_data::COL_IMAGE) {
           editor_->setGeometry(option_.rect);
        } else {
          QStyledItemDelegate::updateEditorGeometry(editor_, option_, index_);
        }
        return;
      }
      */
      // ==========================
           
      image_registry_tree_item::image_registry_tree_item(const image_registry_data & data_,
                                                         image_registry_tree_item * parent_)
      {
        _data_   = data_;
        _parent_ = parent_;
        return;
      }

      image_registry_tree_item::~image_registry_tree_item()
      {
        qDeleteAll(_children_);
        return;
      }

      void image_registry_tree_item::appendChild(image_registry_tree_item * item_)
      {
        //std::cerr << "***** DEVEL ***** item=" << item_ << std::endl;
        _children_.append(item_);
        return;
      }

      image_registry_tree_item * image_registry_tree_item::child(int row_)
      {
        return _children_.value(row_);
      }

      int image_registry_tree_item::childCount() const
      {
        return _children_.count();
      }

      int image_registry_tree_item::columnCount() const
      {
        return image_registry_data::COL_COUNT;
      }
      
      const image_registry_data & image_registry_tree_item::get_data() const
      {
        return _data_;
      }
     
      image_registry_data & image_registry_tree_item::grab_data()
      {
        return _data_;
      }

      int image_registry_tree_item::row() const
      {
        if (_parent_) {
          return _parent_->_children_.indexOf(const_cast<image_registry_tree_item*>(this));
        }
        return 0;
      }
   
      image_registry_tree_item * image_registry_tree_item::parentItem()
      {
        return _parent_;
      }

      // Tree model:
      

      image_registry_tree_model::image_registry_tree_model(image_registry & imreg_,
                                                           QObject * parent_)
        : QAbstractItemModel(parent_)
        , _imreg_(&imreg_)
      {
        return;
      }

      image_registry_tree_model::~image_registry_tree_model()
      {
        delete _root_;
      }

      void image_registry_tree_model::construct()
      {
        _setup_model_data_(*_imreg_);
        return;
      }

      void image_registry_tree_model::set_root(image_registry_tree_item * item_)
      {
        delete _root_;
        _root_ = item_;
        resetInternalData();
        return;
      }

      QModelIndex image_registry_tree_model::index(int row_, int column_, const QModelIndex & parent_)
        const
      {
        if (!_root_ || row_ < 0 || column_ < 0) {
          return QModelIndex();
        }
        image_registry_tree_item * parentItem = item_from_index(parent_);
        image_registry_tree_item * childItem = parentItem->child(row_);
        if (!childItem) {
          return QModelIndex();
        }
        return createIndex(row_, column_, childItem);
      }

      image_registry_tree_item *
      image_registry_tree_model::item_from_index(const QModelIndex & index_) const
      {
        if (index_.isValid()) {
          return static_cast<image_registry_tree_item *>(index_.internalPointer());
        } else {
          return _root_;
        }
      }

      int image_registry_tree_model::rowCount(const QModelIndex & parent_) const
      {
        if (parent_.column() > 0) {
          return 0;
        }
        image_registry_tree_item * parentItem = item_from_index(parent_);
        if (!parentItem) {
          return 0;
        }
        return parentItem->childCount();
      }

      int image_registry_tree_model::columnCount(const QModelIndex & parent_) const
      {
        if (parent_.isValid()) {
          image_registry_tree_item * parentItem = item_from_index(parent_);
          return parentItem->columnCount();
        } else {
          return _root_->columnCount();
        }
      }

      QModelIndex image_registry_tree_model::parent(const QModelIndex & child_index_) const
      {
        image_registry_tree_item * child_item = item_from_index(child_index_);
        if (!child_item) {
          return QModelIndex();
        }
        image_registry_tree_item * parentItem = child_item->parentItem();
        if (!parentItem) {
          return QModelIndex();
        }
        image_registry_tree_item * grandparentItem = parentItem->parentItem();
        if (!grandparentItem){
          return QModelIndex();
        }
        int row = grandparentItem->_children_.indexOf(parentItem);
        return createIndex(row, 0, parentItem);
      }

      QVariant image_registry_tree_model::data(const QModelIndex & index_, int role_) const
      {
        if (!index_.isValid()) {
          return QVariant();
        }

        if (role_ == Qt::DisplayRole) {
          image_registry_tree_item * item = item_from_index(index_);
          if (!item) {
            return QVariant();
          }
          const image_registry_data & imgData = item->get_data();  
          if (index_.column() == image_registry_data::COL_PATH) {
            return tr(imgData.label.c_str());      
          }
          if (index_.column() == image_registry_data::COL_IMAGE) {
            if (imgData.type == image_registry_data::DATA_DEVICE_UNTRACKED) {
              // return tr("untracked device");      
            } else if (imgData.type == image_registry_data::DATA_DEVICE) {
              return QVariant::fromValue(imgData);
              // return tr("device");      
            } else if (imgData.type == image_registry_data::DATA_RESOURCE) {
              return QVariant::fromValue(imgData);
              // return tr("resource");      
            } 
          }
        }

        if (role_ == Qt::DecorationRole) {
          image_registry_tree_item * item = item_from_index(index_);
          if (!item) {
            return QVariant();
          }
          const image_registry_data & imgData = item->get_data();  
          if (index_.column() == image_registry_data::COL_PATH) {
            auto iconIter = _type2icon_.find(imgData.type);
            if (iconIter != _type2icon_.end()) {
              return iconIter->second;
            }
          }
          if (index_.column() == image_registry_data::COL_IMAGE) {
            
          }
        }
 
        if (role_ == Qt::BackgroundRole) {
          image_registry_tree_item * item = item_from_index(index_);
          if (!item) {
            return QVariant();
          }
          const image_registry_data & imgData = item->get_data();
          /*
          // if (index_.row() % 2) {
          if (imgData.image_path.get_depth() % 2) {
            return QColor(Qt::lightGray);
          } else {
            return QColor(Qt::white);
          }
          */
          // if (index_.column() == 1) {
          //   return QColor(Qt::lightGray);
          // }
          return QColor(Qt::white);
        }
        
        if (role_ == Qt::ToolTipRole) {
          image_registry_tree_item * item = item_from_index(index_);
          if (!item) {
            return QVariant();
          }
          const image_registry_data & imgData = item->get_data();  
          if (index_.column() == image_registry_data::COL_PATH) {
            return tr(imgData.image_path.to_string().c_str());      
          }
        }
        
        return QVariant();
      }

      QVariant image_registry_tree_model::headerData(int section_,
                                                     Qt::Orientation orientation_,
                                                     int role_) const
      {
        if (orientation_ == Qt::Horizontal && role_ == Qt::DisplayRole) {
          if (section_ == image_registry_data::COL_PATH) {
            return tr("Path");
          } 
          if (section_ == image_registry_data::COL_IMAGE) {
            return tr("Image");
          } 
        }
        return QVariant();
      }

      // Qt::ItemFlags image_registry_tree_model::flags(const QModelIndex & index_) const
      // {
      //   if (!index_.isValid())
      //          return 0;
      //   return QAbstractItemModel::flags(index_);
      // }
   
      void image_registry_tree_model::insert_icons(const image_registry_data::data_type type_,
                                                   const QIcon & icon_)
      {
        _type2icon_[type_] = icon_;
        return;
      }
   
      void image_registry_tree_model::_setup_model_data_(image_registry & imreg_)
      {
        _imreg_ = &imreg_;
        std::map<std::string, image_registry_tree_item *> processed_paths;
        std::set<std::string> image_paths_str;
        imreg_.build_list_of_image_paths(image_paths_str);
        int max_path_width = 0;
        for (const auto & img_path_str : image_paths_str) {
          vire::utility::path img_path;
          img_path.from_string(img_path_str);
          // std::cerr << "***** DEVEL ***** img_path=" << img_path << std::endl;
          const base_image & img = _imreg_->get_image(img_path_str);
          const std::size_t nsegs = img_path.get_segments().size();
          if (_path_max_depth_ < nsegs + 1) {
            _path_max_depth_ = nsegs + 1;
            if (nsegs > 0) {
              if (img_path.get_leaf().length() > _path_max_depth_leaf_width_) {
                _path_max_depth_leaf_width_ = img_path.get_leaf().length();
              }
            }
          }
          for (std::size_t i = 0; i <= nsegs; i++) {
            vire::utility::path the_path = img_path;
            the_path.truncate(nsegs-i);
            std::string the_path_str = the_path.to_string();
            if (processed_paths.count(the_path_str) == 1) {
              continue;
            }
            // std::cerr << "***** DEVEL *****   the_path=" << the_path << std::endl;
            image_registry_tree_item * node = nullptr;
            image_registry_data imgData;
            imgData.image_path = the_path;
            if (the_path.is_root()) {
              imgData.type = image_registry_data::DATA_ROOT;
              imgData.label = the_path.get_setup() + ":";
            } else {
              if (imreg_.has(the_path_str)) {
                if (imreg_.has_device(the_path_str)) {
                  imgData.type = image_registry_data::DATA_DEVICE;
                } else if (imreg_.has_resource(the_path_str)) {
                  imgData.type = image_registry_data::DATA_RESOURCE;      
                } 
                imgData.image = &imreg_.grab_image(the_path_str);
              } else {
                imgData.type = image_registry_data::DATA_DEVICE_UNTRACKED;
              }
              imgData.label = the_path.get_leaf();
            }
            if (the_path.is_root()) {
              node = new image_registry_tree_item(imgData, nullptr);
              set_root(node);
            } else {
              vire::utility::path the_parent_path = the_path;
              the_parent_path.truncate(1);
              // std::cerr << "***** DEVEL *****   parent_path=" << the_parent_path << std::endl;
              image_registry_tree_item * parent_node = nullptr;
              if (processed_paths.count(the_parent_path.to_string())) {
                parent_node = processed_paths.find(the_parent_path.to_string())->second;
              }
              // std::cerr << "***** DEVEL *****   parent_node=" << parent_node << std::endl;
              node = new image_registry_tree_item(imgData, parent_node);
              parent_node->appendChild(node);
            }
            processed_paths[the_path_str] = node;
          }
        }
        return;
      }

      int image_registry_tree_model::get_path_max_depth() const
      {
        return _path_max_depth_;
      }
      
      int image_registry_tree_model::get_path_max_depth_leaf_width() const
       {
        return _path_max_depth_leaf_width_;
      }
    
      // ****************************************************************      
       
      image_registry_panel::image_registry_panel(QWidget * parent_)
        : QWidget(parent_)
      {
        return;
      }
      
      image_registry_panel::~image_registry_panel()
      {
        if (_tree_model_) {
          delete _tree_model_;
        }
        return;
      }
  
      void image_registry_panel::set_registry(image_registry & registry_)
      {
        _registry_ = &registry_;
        // _registry_emitter_ = &_registry_->get_emitter();
        _construct_();
        return;
      }

      void image_registry_panel::_construct_tree_model_()
      {
        _tree_model_ = new image_registry_tree_model(*_registry_, this);
        auto style = this->style();
        _tree_model_->insert_icons(image_registry_data::DATA_ROOT,
                                   style->standardIcon(QStyle::SP_DirHomeIcon));
        _tree_model_->insert_icons(image_registry_data::DATA_DEVICE_UNTRACKED,
                                   style->standardIcon(QStyle::SP_DirOpenIcon));
        _tree_model_->insert_icons(image_registry_data::DATA_DEVICE, 
                                   style->standardIcon(QStyle::SP_DirClosedIcon));
        _tree_model_->insert_icons(image_registry_data::DATA_RESOURCE, 
                                   style->standardIcon(QStyle::SP_FileIcon));
        _tree_model_->construct();
        return;
      }
      
      void image_registry_panel::_construct_()
      {
        unsigned int cellSize = vire::cms::ui::display_context::get_instance().get_row_height();
        _construct_tree_model_();
        
        QGridLayout * mainLayout = new QGridLayout;

        QLabel * setupTitle = new QLabel(this);
        setupTitle->setText(tr("Setup : "));
        setupTitle->setFixedHeight(cellSize);
        mainLayout->addWidget(setupTitle, 0, 0);

        QLabel * setupLabel = new QLabel(this);
        setupLabel->setText(tr(_registry_->get_device_manager().get_setup_label().c_str()));
        setupLabel->setStyleSheet("QLabel { background-color : white; color : black; text-align : left; }");
        setupLabel->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
        setupLabel->setFixedHeight(cellSize);
        if (_registry_->get_device_manager().has_setup_description()) {
          setupLabel->setToolTip(tr(_registry_->get_device_manager().get_setup_description().c_str()));
        }
        mainLayout->addWidget(setupLabel, 0, 1);

        QLabel * setupVersion = new QLabel(this);
        if (_registry_->get_device_manager().has_setup_version()) {
          setupVersion->setText(tr(_registry_->get_device_manager().get_setup_version().c_str()));
          setupVersion->setStyleSheet("QLabel { background-color : white; color : black; text-align : left; }");
          setupVersion->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
        }
        setupVersion->setFixedHeight(cellSize);
        mainLayout->addWidget(setupVersion, 0, 2);

        QTreeView * treeView = new QTreeView;
        treeView->setModel(_tree_model_);
        // int firstColumnWidth =
        //   _tree_model_->get_path_max_depth() * 20
        //   + _tree_model_->get_path_max_depth_leaf_width() * 20;
        // treeView->setColumnWidth(0, firstColumnWidth);
        QHeaderView * headerView = treeView->header();
        headerView->setSectionResizeMode(QHeaderView::ResizeToContents);
        _delegate_ = new image_registry_delegate(_tree_model_, treeView, this);
        std::cerr << "***** DEVEL ***** image_registry_delegate created..." << std::endl;
        treeView->setItemDelegate(_delegate_);
        mainLayout->addWidget(treeView, 1, 0, 1, 3);

        this->setLayout(mainLayout);
        return;
      }
 
    } // namespace ui

  } // namespace cms

} // namespace vire
