//! \file  vire/cmslapp/the_serializable.cc
// \brief  Class serialization support for the Vire CMSLAPP plugin
//
// Copyright (c) 2017 by Francois Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2017 by Université de Caen Normandie
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

// Third Party:
// - Boost:
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>

// - Bayeux/datatools:
#include <bayeux/datatools/archives_instantiation.h>

// --------------------------------------------------------------------------------------------
// Vire/utility:
#include <vire/cmslapp/connection_request-serial.ipp>
#include <vire/cmslapp/connection_success-serial.ipp>
#include <vire/cmslapp/connection_failure-serial.ipp>
#include <vire/cmslapp/disconnection_request-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmslapp::connection_request)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmslapp::connection_success)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmslapp::connection_failure)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmslapp::disconnection_request)

// Export classes:
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmslapp::connection_request)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmslapp::connection_success)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmslapp::connection_failure)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmslapp::disconnection_request)

// end
