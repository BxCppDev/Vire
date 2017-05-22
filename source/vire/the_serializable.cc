// vire/the_serializable.cc
// Class serialization support for the Vire core library
//
// Copyright (c) 2015 by Francois Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2015 by Université de Caen
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

// Ourselves:

// Vire/utility:
#include <vire/utility/base_identifier-serial.ipp>
#include <vire/utility/instance_identifier-serial.ipp>
#include <vire/utility/model_identifier-serial.ipp>
#include <vire/utility/base_payload-serial.ipp>
#include <vire/utility/base_request-serial.ipp>
#include <vire/utility/base_response-serial.ipp>
#include <vire/utility/base_event-serial.ipp>
#include <vire/utility/base_alarm-serial.ipp>
#include <vire/utility/base_error-serial.ipp>
#include <vire/utility/invalid_context_error-serial.ipp>
#include <vire/utility/invalid_setup_id_error-serial.ipp>
#include <vire/utility/metadata_record-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::instance_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::model_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_payload)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_request)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_response)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_event)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_alarm)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::invalid_context_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::invalid_setup_id_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::metadata_record)

// Export payload classes:
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::base_alarm)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::base_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::invalid_context_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::invalid_setup_id_error)

// Vire/message:
#include <vire/message/message_identifier-serial.ipp>
#include <vire/message/message_header-serial.ipp>
#include <vire/message/message_body-serial.ipp>
#include <vire/message/message-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message_header)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message_body)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message)

// Vire/user:
// #include <vire/user/user-serial.ipp>
// #include <vire/user/group-serial.ipp>

// Vire/device:
#include <vire/device/slot-serial.ipp>

// // Class vire::user::user :
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::user::user)
// BOOST_CLASS_EXPORT_IMPLEMENT(vire::user::user)

// // Class vire::user::group:
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::user::group)
// BOOST_CLASS_EXPORT_IMPLEMENT(vire::user::group)

// Class vire::device::slot:
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::device::slot)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::device::slot)
