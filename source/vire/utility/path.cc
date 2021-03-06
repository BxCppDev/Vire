// vire/utility/path.cc - Vire path
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/utility/path.h>

// Standard Library:
#include <iostream>
#include <sstream>

// Third party:
// - Boost:
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/exception.h>

// This Project:
// #include <vire/version.h>

namespace vire {

  namespace utility {

    // static
    const char path::setup_separator()
    {
      return ':';
    }

    // static
    const char path::root_symbol()
    {
      return '/';
    }

    // static
    const char path::path_separator()
    {
      return '/';
    }

    // static
    const char path::index_separator()
    {
      return '_';
    }

    // static
    const char path::address_separator()
    {
      return '.';
    }

    // static
    bool path::is_root(const std::string & path_)
    {
      std::string setup;
      std::vector<std::string> segments;
      if (!extract(path_, setup, segments)) {
        DT_THROW(std::logic_error, "Invalid path '" << path_ << "'!");
      }
      // std::cerr << "[devel] setup = '" << setup << "'" << std::endl;
      // std::cerr << "[devel] segments = " << segments.size() << " " << std::endl;
      if (segments.size() > 0) {
        return false;
      }
      if (setup.empty()) {
        return false;
      }
      return true;
      // DT_THROW_IF(!validate_path(path_), std::logic_error,
      //             "Invalid path '" << path_ << "'!");
      // if (path_.back() == setup_separator()) {
      //   return true;
      // }
      // return false;
    }

    // static
    bool path::validate_setup(const std::string & setup_)
    {
      static const uint32_t nv_flags = datatools::NV_NO_COLON
        | datatools::NV_NO_DOT
        | datatools::NV_NO_HYPHEN;
      return datatools::name_validation(setup_, nv_flags);
    }

    // static
    bool path::validate_path_segment(const std::string & token_)
    {
      static const uint32_t nv_flags = datatools::NV_NO_COLON
        | datatools::NV_NO_DOT
        | datatools::NV_NO_HYPHEN;
      return datatools::name_validation(token_, nv_flags);
    }

    // static
    bool path::build(const std::string & setup_,
                     const std::vector<std::string> & segments_,
                     std::string & path_)
    {
      path_.clear();
      if (setup_.empty()) return false;
      if (! validate_setup(setup_)) {
        return false;
      }
      std::ostringstream opath;
      if (!setup_.empty()) {
        opath << setup_ << path::setup_separator();
      }
      if (segments_.size()) {
        opath << path::root_symbol();
        for (std::size_t i = 0; i < segments_.size(); i++) {
          if (! validate_path_segment(segments_[i])) {
            return false;
          }
          if (i > 0) opath << path_separator();
          opath << segments_[i];
        }
      }
      path_ = opath.str();
      return true;
    }

    // static
    std::string path::build(const std::string & setup_,
                            const std::vector<std::string> & segments_)
    {
      std::string p;
      build(setup_, segments_, p);
      return p;
    }

    // static
    bool path::build(const std::string & setup_,
                     const std::string & dirs_,
                     const std::string & leaf_,
                     std::string & path_)
    {
      path_.clear();
      std::vector<std::string> segments;
      if (!dirs_to_segments(dirs_, segments)) {
        // std::cerr << "DEVEL: ERROR: path::build: dirs_to_segments FAILED!" << std::endl;
        return false;
      }
      if (!leaf_.empty()) {
        segments.push_back(leaf_);
      }
      return build(setup_, segments, path_);
    }

    // static
    bool path::build_child_with_indexes(const std::string & parent_path_,
                                        const std::string & leaf_,
                                        const std::vector<uint32_t> & indexes_,
                                        std::string & path_)
    {
      std::ostringstream out;
      out << parent_path_ << path::path_separator() << leaf_;
      for (std::size_t i = 0; i < indexes_.size(); i++) {
        out << index_separator() << std::dec << indexes_[i];
      }
      path_ = out.str();
      return true;
    }

    // static
    bool path::build_child_with_index(const std::string & parent_path_,
                                      const std::string & leaf_,
                                      const uint32_t index_,
                                      std::string & path_)
    {
      std::ostringstream out;
      out << parent_path_ << path::path_separator() << leaf_;
      out << index_separator() << std::dec << index_;
      path_ = out.str();
      return true;
    }

    // static
    bool path::build_child(const std::string & parent_path_,
                           const std::string & leaf_,
                           std::string & path_)
    {
      return build_child_with_indexes(parent_path_, leaf_, {}, path_);
    }

    std::string path::build(const std::string & setup_,
                            const std::string & dirs_,
                            const std::string & leaf_)
    {
      std::string pth;
      DT_THROW_IF(!path::build(setup_, dirs_, leaf_, pth),
                  std::logic_error,
                  "Cannot build a path from {setup='" << setup_ << "',dirs='" << dirs_ << "',leaf='" << leaf_ << "'}!");
      return pth;
    }

    // static
    bool path::segments_to_dirs(const std::vector<std::string> & segments_, std::string & dirs_)
    {
      std::ostringstream dirs_out;

      if (segments_.size() == 0) {
        dirs_out << root_symbol();
      } else {
        for (const std::string & seg : segments_) {
          dirs_out << path_separator() << seg;
        }
      }
      dirs_ = dirs_out.str();
      return true;
    }

    // static
    bool path::dirs_to_segments(const std::string & dirs_, std::vector<std::string> & segments_)
    {
      segments_.clear();
      std::string dirs = dirs_;
      if (dirs.empty()) {
        return false;
      }
      if (dirs[0] != root_symbol()) {
        return false;
      }
      dirs = dirs.substr(1); // skip the root symbol
      if (dirs.length()) {
        std::vector<std::string> segs_tokens;
        boost::split(segs_tokens, dirs, boost::is_from_range(path_separator(),path_separator()));
        for (std::size_t i = 0; i < segs_tokens.size(); i++) {
          const std::string & seg = segs_tokens[i];
          if (! validate_path_segment(seg)) {
            segments_.clear();
            return false;
          }
          segments_.push_back(seg);
        }
      }
      return true;
    }

    // static
    bool path::extract(const std::string & path_,
                       std::string & setup_,
                       std::vector<std::string> & segments_)
    {
      setup_.clear();
      segments_.clear();
      std::string setup;
      std::vector<std::string> segs;
      std::string path = path_;
      std::size_t found_setup_sep = path.find(path::setup_separator());
      if (found_setup_sep == std::string::npos) {
        return false;
      }
      setup = path.substr(0, found_setup_sep);
      if (setup.empty() || !validate_setup(setup)) {
        return false;
      }
      path = path.substr(found_setup_sep + 1);
      if (path.length() == 0) {
        setup_ = setup;
        return true;
      }
      if (path[0] != path::root_symbol()) {
        return false;
      }
      path = path.substr(1);
      if (path.length() > 0) {
        if (path.back() == path::path_separator()) {
          return false;
        }
        boost::split(segs, path, boost::is_from_range(path_separator(), path_separator()));
        for (std::size_t i = 0; i < segs.size() - 1; i++) {
          const std::string & segment = segs[i];
          if (segment.empty()) return false;
          if (! validate_path_segment(segment)) {
            return false;
          }
        }
      }
      setup_ = setup;
      segments_ = segs;
      return true;
    }

    bool path::extract(const std::string & path_,
                       std::string & setup_,
                       std::string & dirs_,
                       std::string & leaf_)
    {
      setup_.clear();
      dirs_.clear();
      leaf_.clear();
      std::vector<std::string> segments;
      if (! extract(path_, setup_, segments)) return false;
      // std::cerr << "DEVEL: path::extract: path  = '" << path_ << "'" << std::endl;
      // std::cerr << "DEVEL: path::extract: setup = '" << setup_ << "'" << std::endl;
      // std::cerr << "DEVEL: path::extract: #segments = " << segments.size() << std::endl;
      if (segments.size()) {
        // for (const auto & seg : segments) {
        //   std::cerr << "DEVEL: path::extract: segment = '" << seg << "'" << std::endl;
        // }
        std::ostringstream segout;
        segout << root_symbol();
        for (std::size_t i = 0; i < segments.size() - 1; i++) {
          if (i != 0) segout << path_separator();
          segout << segments[i];
        }
        dirs_ = segout.str();
        leaf_ = segments.back();
      }
      return true;
    }

    bool path::validate_path(const std::string & path_)
    {
      std::string setup;
      std::vector<std::string> segments;
      if (!extract(path_, setup, segments)) {
        return false;
      }
      return true;
    }

    bool path::validate_address(const std::string & address_)
    {
      std::vector<std::string> address_tokens;
      boost::split(address_tokens, address_, boost::is_from_range(address_separator(), address_separator()));
      if (address_tokens.size() < 2) return false;
      for (std::size_t i = 0; i < address_tokens.size(); i++) {
        const std::string & addr = address_tokens[i];
        if (! validate_path_segment(addr)) {
          return false;
        }
      }
      return true;
    }

    // static
    bool path::from_address(const std::string & address_, std::string & path_)
    {
      if (!validate_address(address_)) return false;
      static std::string tok0;
      if (tok0.length() == 0) {
        tok0.push_back(path::address_separator());
      }
      static std::string tok1;
      if (tok1.length() == 0) {
        tok1.push_back(path::setup_separator());
        tok1.push_back(path::path_separator());
      }
      static std::string tok2;
      if (tok2.length() == 0) {
        tok2.push_back(path::path_separator());
      }
      // first "." -> ":/"
      std::string p = boost::replace_first_copy(address_, tok0, tok1);
      // other "." -> "/"
      p = boost::replace_all_copy(p, tok0, tok2);
      path_ = p;
      return true;
    }

    // static
    std::string path::from_address(const std::string & address_)
    {
      std::string path;
      DT_THROW_IF(!from_address(address_, path), std::logic_error, "Invalid address '" << address_ << "'!");
      return path;
    }

    // static
    bool path::to_address(const std::string & path_, std::string & address_)
    {
      if (!validate_path(path_)) return false;
      static std::string tok0;
      if (tok0.length() == 0) {
        tok0.push_back(path::setup_separator());
        tok0.push_back(path::root_symbol());
      }
      static std::string tok1;
      if (tok1.length() == 0) {
        tok1.push_back(path::path_separator());
      }
      static std::string tok2;
      if (tok2.length() == 0) {
        tok2.push_back(path::address_separator());
      }
      std::string p = boost::replace_first_copy(path_, tok0, tok2);
      p = boost::replace_all_copy(p, tok1, tok2);
      address_ = p;
      return true;
    }

    // static
    std::string path::to_address(const std::string & path_)
    {
      std::string addr;
      DT_THROW_IF(!to_address(path_, addr), std::logic_error, "Invalid path '" << path_ << "'!");
      return addr;
    }

    // static
    bool path::is_parent_of(const std::string & parent_path_,
                            const std::string & child_path_,
                            bool direct_ )
    {
      DT_THROW_IF(!validate_path(parent_path_), std::logic_error,
                  "Invalid path '" << parent_path_ << "'!");
      DT_THROW_IF(!validate_path(child_path_), std::logic_error,
                  "Invalid path '" << child_path_ << "'!");
      std::string ppath = parent_path_;
      if (ppath.back() != setup_separator()) {
        if (ppath.back() != path_separator()) {
          ppath.push_back(path_separator());
        }
      }
      // std::clog << "[devel] parent path = '" << ppath << "'" << std::endl;
      std::size_t found = child_path_.find(ppath);
      if (found == 0) {
        if (direct_) {
          std::string leaf = child_path_.substr(ppath.length());
          // std::clog << "[devel] leaf = '" << leaf << "'" << std::endl;
          if (leaf.find(path_separator()) != std::string::npos) {
            return false;
          }
        }
        return true;
      }
      return false;
    }

    // static
    bool path::is_child_of(const std::string & child_path_,
                           const std::string & parent_path_,
                           bool direct_ )
    {
      DT_THROW_IF(!validate_path(child_path_), std::logic_error,
                  "Invalid path '" << child_path_ << "'!");
      DT_THROW_IF(!validate_path(parent_path_), std::logic_error,
                  "Invalid path '" << parent_path_ << "'!");
      if (child_path_ == parent_path_) return false;
      return is_parent_of(parent_path_, child_path_, direct_);
    }

    path::path()
    {
      return;
    }

    path::path(const std::string & setup_)
      : _setup_(setup_)
    {
      return;
    }

    path::path(const std::string & setup_, const std::vector<std::string> & segments_)
      : _setup_(setup_)
      , _segments_(segments_)
    {
      return;
    }

    path::path(const std::string & setup_,
               const std::string & dirs_,
               const std::string & leaf_)
    {
      set_setup(setup_);
      std::vector<std::string> segments;
      DT_THROW_IF(!dirs_to_segments(dirs_, segments),
                  std::logic_error,
                  "Invalid dirs expression '" << dirs_ << "'!");
      _segments_ = segments;
      if (!leaf_.empty()) {
        DT_THROW_IF(! validate_path_segment(leaf_),
                    std::logic_error,
                    "Invalid leaf '" << leaf_ << "'!");
        _segments_.push_back(leaf_);
      }
      return;
    }

    void path::set_setup(const std::string & setup_)
    {
      DT_THROW_IF(! validate_setup(setup_),
                  std::logic_error,
                  "Invalid setup '" << setup_ << "'!");
      _setup_ = setup_;
      return;
    }

    const std::string & path::get_setup() const
    {
      return _setup_;
    }

    const std::vector<std::string> & path::get_segments() const
    {
      return _segments_;
    }

    void path::set_segments(const std::vector<std::string> & segments_,  const bool no_check_)
    {
      if (!no_check_) {
        for (const auto & token : segments_) {
          DT_THROW_IF(! validate_path_segment(token),
                      std::logic_error,
                      "Invalid path segment '" << token << "'!");
        }
      }
      _segments_ = segments_;
      return;
    }

    void path::reset()
    {
      _setup_.clear();
      _segments_.clear();
      return;
    }

    bool path::is_valid() const
    {
      if (_setup_.empty()) return false;
      return true;
    }

    bool path::is_root() const
    {
      if (!is_valid()) return false;
      return (_segments_.size() == 0);
    }

    std::size_t path::get_depth() const
    {
      return _segments_.size();
    }

    bool path::operator==(const path & p_) const
    {
      if (_setup_ != p_._setup_) return false;
      if (_segments_.size() != p_._segments_.size()) return false;
      for (std::size_t i = 0; i < _segments_.size(); i++) {
        if (_segments_[i] == p_._segments_[i])  return false;
      }
      return true;
    }

    bool path::operator<(const path & p_) const
    {
      if (_setup_ < p_._setup_) return true;
      if (_segments_.size() < p_._segments_.size()) return true;
      for (std::size_t i = 0; i < _segments_.size(); i++) {
        if (_segments_[i] < p_._segments_[i])  return true;
      }
      return false;
    }

    void path::to_string(std::string & repr_) const
    {
      build(_setup_, _segments_, repr_);
      return;
    }

    std::string path::to_string() const
    {
      return build(_setup_, _segments_);
    }

    bool path::from_string(const std::string & repr_)
    {
      std::string setup;
      std::vector<std::string> segments;
      if (extract(repr_, setup, segments)) {
        _setup_ = setup;
        _segments_ = segments;
        return true;
      }
      return false;
    }

    void path::append_segment(const std::string & segment_, const bool no_check_)
    {
      if (!no_check_) {
        DT_THROW_IF(! validate_path_segment(segment_),
                    std::logic_error,
                    "Invalid path segment '" << segment_ << "'!");
      }
      _segments_.push_back(segment_);
      return;
    }

    void path::append_segments(const std::vector<std::string> & segments_, const bool no_check_)
    {
      for (const std::string & seg : segments_) {
        append_segment(seg, no_check_);
      }
      return;
    }

    void path::append(const relative_path & relpath_)
    {
      append_segments(relpath_.get_segments(), true);
      return;
    }
    
    void path::truncate(const unsigned int nlevels_)
    {
      DT_THROW_IF(nlevels_ > _segments_.size(), std::logic_error,
                  "Cannot truncate segments more than the path length!");
      for (std::size_t i = 0; i < nlevels_; i++) {
        _segments_.pop_back();
      }
      return;
    }
    
    path path::make_parent() const
    {
      DT_THROW_IF(is_root(), std::logic_error, "Cannot truncate segments on a root path!");
      path parent = *this;
      parent.truncate(1);
      return parent;
    }
 
    path path::make_child(const std::string & leaf_) const
    {
      path child = *this;
      child.append_segment(leaf_);
      return child;
    }

    bool path::is_parent_of(const path & other_, bool direct_) const
    {
      if (_setup_ != other_._setup_) {
        return false;
      }
      std::size_t sz = _segments_.size();
      std::size_t osz = other_._segments_.size();
      if (sz >= osz) {
        // std::cerr << "devel **** sz=" << sz << std::endl;
        // std::cerr << "devel **** osz=" << osz << std::endl;
        return false;
      }
      if (direct_ && (sz + 1 != osz)) {
        // std::cerr << "devel **** !direct" << std::endl;
        return false;
      }
      for (std::size_t i = 0; i < _segments_.size(); i++) {
        if (_segments_[i] != other_._segments_[i]) {
          // std::cerr << "devel **** invalid segment[" << i << "]='" << _segments_[i] << "'" << std::endl;
          return false;
        }
      }
      return true;
    }

    bool path::is_child_of(const path & other_, bool direct_) const
    {
      return other_.is_parent_of(*this, direct_);
    }

    path path::operator+(const relative_path & rp_) const
    {
      path p = *this;
      p.append(rp_);
      return p;
    }

    relative_path path::operator-(const path & p_) const
    {
      return this->subtract(p_);
    }

    relative_path path::subtract(const path & other_) const
    {
      DT_THROW_IF(_setup_ != other_._setup_,
                  std::logic_error,
                  "Subtraction is not applicable; incompatible setups!");
      DT_THROW_IF(_segments_.size() < other_._segments_.size(),
                  std::logic_error,
                  "Subtraction is not applicable; incompatible path depth!");
      for (std::size_t i = 0; i < other_._segments_.size(); i++) {
        DT_THROW_IF(_segments_[i] != other_._segments_[i],
                    std::logic_error,
                    "Subtraction is not applicable: '" << other_ << "' is not a parent of '" << *this << "'!");
      }
      relative_path rp;
      for (std::size_t i = other_._segments_.size(); i < _segments_.size(); i++) {
        rp.append_segment(_segments_[i]);
      }
      return rp;
    }

    bool path::has_leaf() const
    {
      return _segments_.size() > 0;
    }
    
    const std::string & path::get_leaf() const
    {
      DT_THROW_IF(! has_leaf(),
                  std::logic_error,
                  "There is no leaf in this path!");
      return _segments_.back();
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const path & p_)
    {
      out_ << p_.to_string();
      return out_;
    }

    relative_path::relative_path()
    {
      return;
    }

    bool relative_path::is_empty() const
    {
      return _segments_.size() == 0;
    }

    void relative_path::to_string(std::string & repr_) const
    {
      std::ostringstream out;
      for (std::size_t i = 0; i < _segments_.size();) {
        out << _segments_[i];
        if (++i < _segments_.size()) {
          out << path::path_separator();
        }
      }
      repr_ = out.str();
      return;
    }

    std::string relative_path::to_string() const
    {
      std::string repr;
      this->to_string(repr);
      return repr;
    }

    bool relative_path::from_string(const std::string & repr_)
    {
      std::vector<std::string> segments;
      std::string work = path::root_symbol() + repr_;
      if (path::dirs_to_segments(work, segments)) {
        _segments_ = segments;
        return true;
      }
      return false;
    }

    void relative_path::prepend_segments(const std::vector<std::string> & more_)
    {
      for (std::vector<std::string>::const_reverse_iterator i = more_.rbegin();
           i != more_.rend();
           i++) {
        prepend_segment(*i);
      }
      return;
    }
 
    void relative_path::prepend_segment(const std::string & head_)
    {
      DT_THROW_IF(!path::validate_path_segment(head_),
                  std::logic_error,
                  "Invalid path head '" << head_ << "'!");
      _segments_.insert(_segments_.begin(), head_);
      return;
    }

    void relative_path::cut_back(const std::size_t ncut_)
    {
      DT_THROW_IF(ncut_ > _segments_.size(),
                  std::logic_error,
                  "Cannot cut by a too large number of segments!");
      _segments_.resize(_segments_.size() - ncut_);
      return;
    }

    void relative_path::cut_front(const std::size_t ncut_)
    {
      DT_THROW_IF(ncut_ > _segments_.size(),
                  std::logic_error,
                  "Cannot cut by a too large number of segments!");
      _segments_.erase(_segments_.begin(), _segments_.begin() + ncut_);
      return;
    }

    void relative_path::append_segment(const std::string & leaf_)
    {
      DT_THROW_IF(!path::validate_path_segment(leaf_),
                  std::logic_error,
                  "Invalid path leaf '" << leaf_ << "'!");
      _segments_.push_back(leaf_);
      return;
    }
                        
    void relative_path::append_segments(const std::vector<std::string> & more_)
    {
      for (std::size_t i = 0; i < more_.size(); i++) {
        append_segment(more_[i]);
      }
      return;
   }
                        
    void relative_path::set_segments(const std::vector<std::string> & segments_)
    {
      _segments_.clear();
      for (std::size_t i = 0; i < segments_.size(); i++) {
        append_segment(segments_[i]);
      }
      return;
    }

    const std::vector<std::string> & relative_path::get_segments() const
    {
      return _segments_;
    }

    std::size_t relative_path::get_length() const
    {
      return _segments_.size();
    }

    void relative_path::clear()
    {
      _segments_.clear();
      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const relative_path & rp_)
    {
      out_ << rp_.to_string();
      return out_;
    }

    bool relative_path::operator==(const relative_path & rp_) const
    {
      if (_segments_.size() != rp_._segments_.size()) return false;
      for (std::size_t i = 0; i < _segments_.size(); i++) {
        if (_segments_[i] == rp_._segments_[i]) return false;
      }
      return true;
    }

    bool relative_path::operator<(const relative_path & rp_) const
    {
      if (_segments_.size() < rp_._segments_.size()) return true;
      for (std::size_t i = 0; i < _segments_.size(); i++) {
        if (_segments_[i] < rp_._segments_[i]) return true;
      }
      return false;
    }
    
    relative_path relative_path::make_child(const std::string & leaf_) const
    {
      relative_path rp = *this;
      rp.append_segment(leaf_);
      return rp;
    }
  
    relative_path relative_path::make_child(const std::vector<std::string> & more_) const
    {
      relative_path rp = *this;
      rp.append_segments(more_);
      return rp;
    }

    relative_path relative_path::operator+(const relative_path & rp_) const
    {
      relative_path rp = *this;
      rp.append_segments(rp_._segments_);
      return rp;
    }    
 
    // static
    const relative_path & empty_relative_path()
    {
      static const relative_path _erp;
      return _erp;
    }

  } // namespace utility

} // namespace vire
