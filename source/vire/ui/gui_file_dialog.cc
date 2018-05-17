//! \file vire/ui/gui_file_dialog.cc
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
#include <vire/ui/gui_file_dialog.h>

// Standard library:
#include <iostream>
#include <sstream>

// Third party:
// - Qt:
#include <QString>
#include <QApplication>
#include <QInputDialog>
#include <QComboBox>
#include <QStringList>
#include <QFileDialog>
// - Boost:
#include <boost/filesystem.hpp>
// - Bayeux:
#include <bayeux/datatools/qt/interface.h>
#include <bayeux/datatools/utils.h>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    gui_file_dialog::gui_file_dialog()
    {
      datatools::qt::interface::instance();
      return;
    }

    gui_file_dialog::~gui_file_dialog()
    {
      return;
    }

    dialog_report gui_file_dialog::input(std::string & path_)
    {
      path_.clear();
      QString qcaption;
      if (has_title()) {
        qcaption += QString::fromStdString(get_title());
      }
      if (has_label()) {
        if (!qcaption.isEmpty()) {
          qcaption += " - ";
        }
        qcaption += QString::fromStdString(get_label());
      }
      QString qpath; // = ".";
      if (has_base_dir_path()) {
        std::string base_dir_path = get_base_dir_path();
        if (is_resolve()) {
          datatools::fetch_path_with_env(base_dir_path);
        }
        qpath = QString::fromStdString(base_dir_path);
      }
      if (has_default_path()) {
        std::string default_path = get_default_path();
        if (is_resolve()) {
          datatools::fetch_path_with_env(default_path);
        }
        qpath = QString::fromStdString(default_path);
      }
      bool dir_only = is_select_dirs();
      bool read_only = is_read_mode();
      bool create_only = is_create_mode();
      bool ok = false;
      if (!dir_only) {
        QString qfilter;
        std::ostringstream extfilters;
        if (has_extension_filters()) {
          std::size_t cat_count = 0;
          for (const auto & pair : get_allowed_extensions()) {
            if (cat_count > 0) {
              extfilters << ";;";
            }
            extfilters << pair.first << ' ' << '(';
            const std::set<std::string> & exts = pair.second;
            std::size_t ext_count = 0;
            for (const auto & ext : exts) {
              if (ext_count > 0) {
                extfilters << ' ';
              }
              extfilters << '*';
              extfilters << ext;
              ext_count++;
            }
            extfilters << ')';
            cat_count++;
          }
        }
        qfilter = QString::fromStdString(extfilters.str());
        QString qSelectedFilter;
        QFileDialog::Options opts;
        QString fileName;
        if (create_only) {
          fileName = QFileDialog::getSaveFileName((QWidget*) nullptr,
                                                  qcaption,
                                                  qpath,
                                                  qfilter,
                                                  &qSelectedFilter,
                                                  opts);
        } else {
          opts |= QFileDialog::ReadOnly;
          fileName = QFileDialog::getOpenFileName((QWidget*) nullptr,
                                                qcaption,
                                                qpath,
                                                qfilter,
                                                &qSelectedFilter,
                                                opts);
        }
        if (!fileName.isEmpty()) {
          std::string path = fileName.toStdString();
          std::string path_ext = boost::filesystem::extension(path);
          if (has_extension_filters()) {
            if (!is_allowed_extension(path_ext)) {
              std::string msg;
              msg += "path '";
              msg += path;
              msg += "' has no valid extension";
              return make_dialog_report(false, msg);
            }
          }
          boost::filesystem::path p(path);
          if (read_only && !boost::filesystem::exists(p)) {
            return make_dialog_report(false,
                                      "path '" + path + "' does not exist");
          }
          if (create_only && boost::filesystem::exists(p)) {
            return make_dialog_report(false,
                                      "path '" + path + "' already exists");
          }
          path_ = path;
          return make_dialog_report(true);
        }
      } else {
        QString qfilter;
        QString dirName = QFileDialog::getExistingDirectory((QWidget*) nullptr,
                                                             qcaption,
                                                             qpath);
        if (!dirName.isEmpty()) {
          path_ = dirName.toStdString();
          return make_dialog_report(true);
        }
      }

      return make_dialog_report(false, "no input valid path");
    }

  } // namespace ui

} // namespace vire
