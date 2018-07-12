// vire/cms/ui/utils.cc
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
#include <vire/cms/ui/utils.h>

// Standard library:
#include <memory>

namespace vire {

  namespace cms {
    
    namespace ui {

      // static
      const int display_context::DEFAULT_FONT_POINT_SIZE;
      
      // static
      const int display_context::DEFAULT_ROW_FONT_MARGIN;
      
      // static
      const int display_context::DEFAULT_ROW_HEIGHT;
       
      // static
      const int display_context::DEFAULT_VERTICAL_SPACING;
       
      // static
      const int display_context::DEFAULT_HORIZONTAL_SPACING;
    
      display_context::display_context()
      {
        return;
      }

      // static
      display_context & display_context::grab_instance()
      {
        std::unique_ptr<display_context> _singleton;
        if (!_singleton) {
          _singleton.reset(new display_context);
        }
        return *_singleton;     
      }

      // static
      const display_context & display_context::get_instance()
      {
        std::unique_ptr<display_context> _singleton;
        if (!_singleton) {
          _singleton.reset(new display_context);
        }
        return const_cast< display_context &>(grab_instance()); 
      }

      void display_context::set_font_point_size(const int font_point_size_)
      {
        if (is_locked()) {
          return;
        }
        _font_point_size_ = font_point_size_;
        if (_row_height_ < _font_point_size_ + 2 * _row_font_margin_) {
          _row_height_ = _font_point_size_ + 2 * _row_font_margin_;
        }
        return;
      }

      int display_context::get_font_point_size() const
      {
        return _font_point_size_;
      }

      void display_context::set_row_height(const int height_)
      {
        if (is_locked()) {
          return;
        }
        _row_height_ = height_;
        return;
      }
     
      int display_context::get_row_height() const
      {
        return _row_height_;
      }
     
      int display_context::get_vertical_spacing() const
      {
        return _vertical_spacing_;
      }
     
      int display_context::get_horizontal_spacing() const
      {
        return _horizontal_spacing_;
      }
 
      void display_context::lock()
      {
        _locked_ = true;
        return;
      }

      bool display_context::is_locked() const
      {
        return _locked_;
      }
   
    } // namespace ui

  } // namespace cms

} // namespace vire
