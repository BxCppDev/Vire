// i_file_dialog.cc

// Ourselves:
#include <vire/ui/i_file_dialog.h>

// Third party:
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>

namespace vire {

  namespace ui {

    i_file_dialog::i_file_dialog()
    {
      return;
    }

    i_file_dialog::~i_file_dialog()
    {
      return;
    }

    bool i_file_dialog::is_resolve() const
    {
      return !_no_resolve_;
    }

    void i_file_dialog::set_resolve(bool r_)
    {
      _no_resolve_ = !r_;
      return;
    }

    bool i_file_dialog::has_default_path() const
    {
      if (_default_path_) return true;
      return false;
    }

    void i_file_dialog::set_default_path(const std::string & path_)
    {
      std::string test_path = path_;
      if (is_resolve() && !datatools::fetch_path_with_env(test_path)) {
        DT_THROW(std::logic_error,
                 "Cannot resolve default path '" << path_ << "'");
      }
      std::string default_path_ext = boost::filesystem::extension(path_);
      if (has_extension_filters() && is_allowed_extension(default_path_ext)) {
        DT_THROW(std::logic_error,
                 "Default path '" << path_
                 << "' has not a supported extension '" << default_path_ext << "'!");
      }
      if (has_base_dir_path()) {
        std::string test_base_dir_path = get_base_dir_path();
        if (is_resolve()) {
          datatools::fetch_path_with_env(test_base_dir_path);
        }
        if (!boost::starts_with(test_base_dir_path, test_path)) {
          DT_THROW(std::logic_error,
                   "Default path '" << path_
                   << "' does not match the base dir path '" << get_base_dir_path() << "'!");
        }
      }
      _default_path_ = path_;
      return;
    }

    void i_file_dialog::reset_default_path()
    {
      _default_path_ = boost::none;
      return;
    }

    const std::string & i_file_dialog::get_default_path() const
    {
      DT_THROW_IF(!has_default_path(),
                  std::logic_error,
                  "No default path!");
      return _default_path_.value();
    }

    bool i_file_dialog::has_base_dir_path() const
    {
      if (_base_dir_path_) return true;
      return false;
    }

    void i_file_dialog::set_base_dir_path(const std::string & path_)
    {
      std::string test_path = path_;
      if (is_resolve() && !datatools::fetch_path_with_env(test_path)) {
        DT_THROW(std::logic_error,
                 "Cannot resolve base dir path '" << path_ << "'");
      }
      if (!boost::filesystem::is_directory(test_path)) {
        DT_THROW(std::logic_error,
                 "Base dir path '" << path_ << "' is not a directory!");
      }
      if (has_default_path()) {
        std::string test_default_path = get_default_path();
        if (is_resolve()) {
          datatools::fetch_path_with_env(test_default_path);
        }
        if (!boost::starts_with(test_default_path, test_path)) {
          DT_THROW(std::logic_error,
                   "Base dir path '" << path_
                   << "' does not match the default path '" << get_default_path() << "'!");
        }
      }
      _base_dir_path_ = test_path;
      return;
    }

    void i_file_dialog::reset_base_dir_path()
    {
      _base_dir_path_ = boost::none;
      return;
    }

    const std::string & i_file_dialog::get_base_dir_path() const
    {
      DT_THROW_IF(!has_default_path(),
                  std::logic_error,
                  "No base directory path!");
      return _base_dir_path_.value();
    }

    bool i_file_dialog::is_select_dirs() const
    {
      return _select_dirs_;
    }

    void i_file_dialog::set_select_dirs(bool s_)
    {
      if (_allowed_extensions_.size()) {
        DT_THROW(std::logic_error,
                 "Directory selection mode is not compatible with file extension filters!");
      }
      _select_dirs_ = s_;
      return;
    }

    bool i_file_dialog::is_read_mode() const
    {
      return _read_mode_;
    }

    void i_file_dialog::set_read_mode(bool m_)
    {
      _read_mode_ = m_;
      return;
    }

    bool i_file_dialog::is_create_mode() const
    {
      return _create_mode_;
    }

    void i_file_dialog::set_create_mode(bool m_)
    {
      _create_mode_ = m_;
      return;
    }

    bool i_file_dialog::has_extension_filters() const
    {
      return _allowed_extensions_.size() > 0;
    }

    void i_file_dialog::add_extension_filter(const std::string & ext_, const std::string & category_)
    {
      if (is_select_dirs()) {
        DT_THROW(std::logic_error,
                 "File extension filters are not supported in directory selection mode!");
      }
      if (! _allowed_extensions_.count(category_)) {
        std::set<std::string> new_ext;
        _allowed_extensions_[category_] = new_ext;
      }
      std::set<std::string> & ext_set =_allowed_extensions_.find(category_)->second;
      std::string extension = ext_;
      if (extension[0] != '.') {
        extension = "." + ext_;
      }
      ext_set.insert(extension);
      return;
    }

    const std::map<std::string, std::set<std::string> > &
    i_file_dialog::get_allowed_extensions() const
    {
      return _allowed_extensions_;
    }

    bool i_file_dialog::is_allowed_extension(const std::string & ext_) const
    {
      std::string extension = ext_;
      if (extension[0] != '.') {
        extension = "." + ext_;
      }
      for (const auto & cat_pair : _allowed_extensions_) {
        for (const auto & ext : _allowed_extensions_.find(cat_pair.first)->second) {
          if (ext == extension) {
            return true;
          }
        }
      }
      return false;
    }

    bool i_file_dialog::is_allowed_extension_in_category(const std::string & ext_,
                                                         const std::string & category_) const
    {
      std::string extension = ext_;
      if (extension[0] != '.') {
        extension = "." + ext_;
      }
      for (const auto & cat_pair : _allowed_extensions_) {
        for (const auto & ext : _allowed_extensions_.find(cat_pair.first)->second) {
          if (extension == ext && !category_.empty() && (cat_pair.first == category_)) {
            return true;
          }
        }
      }
      return false;
    }

    void i_file_dialog::build_allowed_extension(std::set<std::string> & exts_,
                                                const std::string & category_) const
    {
      exts_.clear();
      for (const auto & cat_pair : _allowed_extensions_) {
        for (const auto & ext : _allowed_extensions_.find(cat_pair.first)->second) {
          bool add_it = true;
          if (!category_.empty() && (cat_pair.first != category_)) {
            add_it = false;
          }
          if (add_it) {
            exts_.insert(ext);
          }
        }
      }
      return;
    }

  } // namespace ui

} // namespace vire
