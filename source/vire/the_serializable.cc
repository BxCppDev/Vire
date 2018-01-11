// vire/the_serializable.cc
// Class serialization support for the Vire core library
//
// Copyright (c) 2015-2017 by Francois Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2015-2017 by Université de Caen Normandie
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

// Standard library:
#include <iostream>

// Third Party:
// - Boost:
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>

// - Bayeux/datatools:
#include <bayeux/datatools/archives_instantiation.h>

#include <vire/utility/the_serializable-inc.h>
#include <vire/message/the_serializable-inc.h>
#include <vire/user/the_serializable-inc.h>
#include <vire/device/the_serializable-inc.h>
#include <vire/cms/the_serializable-inc.h>
#include <vire/cmsserver/the_serializable-inc.h>

// end
