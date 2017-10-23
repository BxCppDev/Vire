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

// --------------------------------------------------------------------------------------------
// Vire/utility:
#include <vire/utility/base_identifier-serial.ipp>
#include <vire/utility/instance_identifier-serial.ipp>
#include <vire/utility/model_identifier-serial.ipp>
#include <vire/utility/base_payload-serial.ipp>
#include <vire/utility/base_alarm-serial.ipp>
#include <vire/utility/base_error-serial.ipp>
#include <vire/utility/invalid_context_error-serial.ipp>
#include <vire/utility/invalid_setup_id_error-serial.ipp>
#include <vire/utility/metadata_record-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_payload)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::instance_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::model_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_alarm)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::invalid_context_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::invalid_setup_id_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::metadata_record)

// Export classes:
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::base_alarm)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::base_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::instance_identifier)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::model_identifier)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::invalid_context_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::invalid_setup_id_error)


// --------------------------------------------------------------------------------------------
// Vire/message:
#include <vire/message/message_identifier-serial.ipp>
#include <vire/message/message_header-serial.ipp>
#include <vire/message/message_body-serial.ipp>
#include <vire/message/message-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message_header)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message_body)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message)

// Export classes:
BOOST_CLASS_EXPORT_IMPLEMENT(vire::message::message)

// --------------------------------------------------------------------------------------------
// Vire/user:
#include <vire/user/user-serial.ipp>
#include <vire/user/group-serial.ipp>

// Class vire::user::user :
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::user::user)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::user::user)

// Class vire::user::group:
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::user::group)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::user::group)

// --------------------------------------------------------------------------------------------
// Vire/device:
#include <vire/device/slot-serial.ipp>

// Class vire::device::slot:
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::device::slot)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::device::slot)

// --------------------------------------------------------------------------------------------
// Vire/cms:
#include <vire/cms/argument_error-serial.ipp>
#include <vire/cms/device_status_change-serial.ipp>
#include <vire/cms/invalid_credentials_error-serial.ipp>
#include <vire/cms/invalid_resource_error-serial.ipp>
#include <vire/cms/invalid_status_error-serial.ipp>
#include <vire/cms/invalid_user_error-serial.ipp>
#include <vire/cms/method_argument-serial.ipp>
#include <vire/cms/no_pubsub_resource_error-serial.ipp>
#include <vire/cms/resource_exec_error-serial.ipp>
#include <vire/cms/resource_exec_failure-serial.ipp>
#include <vire/cms/resource_exec-serial.ipp>
#include <vire/cms/resource_exec_success-serial.ipp>
#include <vire/cms/resource_fetch_status_failure-serial.ipp>
#include <vire/cms/resource_fetch_status-serial.ipp>
#include <vire/cms/resource_fetch_status_success-serial.ipp>
#include <vire/cms/resource_pubsub_failure-serial.ipp>
#include <vire/cms/resource_pubsub-serial.ipp>
#include <vire/cms/resource_pubsub_success-serial.ipp>
#include <vire/cms/resource_status_change-serial.ipp>
#include <vire/cms/resource_status_record-serial.ipp>
#include <vire/cms/timeout_error-serial.ipp>
#include <vire/cms/unknown_resources_error-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::argument_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::argument_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::device_status_change)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::device_status_change)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::invalid_credentials_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::invalid_credentials_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::invalid_resource_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::invalid_resource_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::invalid_user_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::invalid_user_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::method_argument)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::method_argument)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::no_pubsub_resource_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::no_pubsub_resource_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_exec_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_exec_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_exec_failure)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_exec_failure)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_exec_success)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_exec_success)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_exec)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_exec)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_fetch_status_failure)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_fetch_status_failure)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_fetch_status)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_fetch_status)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_fetch_status_success)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_fetch_status_success)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_pubsub_failure)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_pubsub_failure)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_pubsub_success)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_pubsub_success)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_pubsub)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_pubsub)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_status_change)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_status_change)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::resource_status_record)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::resource_status_record)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::timeout_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::timeout_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::unknown_resources_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::unknown_resources_error)

// // --------------------------------------------------------------------------------------------
// // Vire/cmsserver:
// #include <vire/cmsserver/session_info-serial.ipp>
// #include <vire/cmsserver/process_info-serial.ipp>
// #include <vire/cmsserver/session_reservation-serial.ipp>

// // Class vire::cmsserver::session_info:
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::session_info)
// BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmsserver::session_info)

// // Class vire::cmsserver::process_info:
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::process_info)
// BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmsserver::process_info)

// // Class vire::cmsserver::session_reservation:
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::session_reservation)
// BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmsserver::session_reservation)


// end
