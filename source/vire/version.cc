// version.cc - Implementation of Vire version struct
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/version.h>

// Standard Library:
#include <sstream>

namespace vire {

  int version::get_major() {
    return static_cast<int>(VIRE_VERSION_MAJOR);
  }

  int version::get_minor() {
    return static_cast<int>(VIRE_VERSION_MINOR);
  }

  int version::get_patch() {
    return static_cast<int>(VIRE_VERSION_PATCH);
  }

  int version::get_revision() {
    return static_cast<int>(VIRE_VERSION_REVISION);
  }

  std::string version::get_version() {
    static std::string version("");

    if (version.empty()) {
      std::ostringstream stream;
      stream << VIRE_VERSION_MAJOR << "."
             << VIRE_VERSION_MINOR << "."
             << VIRE_VERSION_PATCH;
      version = stream.str();
    }

    return version;
  }

  bool version::is_at_least(int major, int minor, int patch) {
    if (VIRE_VERSION_MAJOR < major) return false;
    if (VIRE_VERSION_MAJOR > major) return true;
    if (VIRE_VERSION_MINOR < minor) return false;
    if (VIRE_VERSION_MINOR > minor) return true;
    if (VIRE_VERSION_PATCH < patch) return false;
    return true;
  }

  bool version::has_feature(const std::string&) {
    /// - If you want to add features, then the following implementation
    ///   provides one example based on string features cached in a set.
    ///
    /// static std::set<std::string> features;
    ///
    /// if (features.empty())
    /// {
    ///   // cache the feature list
    ///   features.insert("FASTAPI");
    ///   features.insert("THREADSAFE");
    /// }
    ///
    /// return features.find(name) != features.end();

    return false;
  }

} // namespace vire
