//! \file  vire/cms/ui/utils
//! \brief UI utilities
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

#ifndef VIRE_CMS_UI_UTILS_H
#define VIRE_CMS_UI_UTILS_H

namespace vire {

  namespace cms {
    
    namespace ui {

      /// \brief Qt display 
      class display_context
      {
      private:

        display_context();
        
      public:
        
        static const int DEFAULT_FONT_POINT_SIZE = 10;
        static const int DEFAULT_ROW_FONT_MARGIN = 2;
        static const int DEFAULT_ROW_HEIGHT      = 14;
        static const int DEFAULT_VERTICAL_SPACING = 2;
        static const int DEFAULT_HORIZONTAL_SPACING = 2;
        
        void set_font_point_size(const int font_point_size_);

        int get_font_point_size() const;

        void set_row_height(const int height_);
        
        int get_row_height() const;
       
        int get_vertical_spacing() const;
       
        int get_horizontal_spacing() const;

        void lock();

        bool is_locked() const;
        
        static display_context & grab_instance();

        static const display_context & get_instance();
        
      private:

        bool _locked_ = false;
        int  _font_point_size_ = DEFAULT_FONT_POINT_SIZE;
        int  _row_font_margin_ = DEFAULT_ROW_FONT_MARGIN;
        int  _row_height_      = DEFAULT_ROW_HEIGHT;
        int  _vertical_spacing_ = DEFAULT_VERTICAL_SPACING;
        int  _horizontal_spacing_ = DEFAULT_HORIZONTAL_SPACING;
        
      };

    } // namespace ui

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_UI_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
