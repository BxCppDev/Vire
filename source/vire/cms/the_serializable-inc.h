// vire/cms/the_serializable-inc.h

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
#include <vire/cms/connection_request-serial.ipp>
#include <vire/cms/connection_success-serial.ipp>
#include <vire/cms/connection_failure-serial.ipp>
#include <vire/cms/disconnection_request-serial.ipp>
#include <vire/cms/disconnection_success-serial.ipp>
#include <vire/cms/disconnection_failure-serial.ipp>

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

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::connection_request)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::connection_request)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::connection_success)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::connection_success)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::connection_failure)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::connection_failure)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::disconnection_request)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::disconnection_request)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::disconnection_success)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::disconnection_success)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cms::disconnection_failure)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cms::disconnection_failure)

// end
