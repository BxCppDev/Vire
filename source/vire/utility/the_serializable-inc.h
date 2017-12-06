// vire/utility/the_serializable-inc.h

#include <vire/utility/base_identifier-serial.ipp>
#include <vire/utility/instance_identifier-serial.ipp>
#include <vire/utility/model_identifier-serial.ipp>
#include <vire/utility/base_payload-serial.ipp>
#include <vire/utility/base_alarm-serial.ipp>
#include <vire/utility/base_error-serial.ipp>
#include <vire/utility/invalid_context_error-serial.ipp>
#include <vire/utility/invalid_setup_id_error-serial.ipp>
#include <vire/utility/metadata_record-serial.ipp>
#include <vire/utility/properties-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_payload)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::instance_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::model_identifier)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_alarm)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::base_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::invalid_context_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::invalid_setup_id_error)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::metadata_record)
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::properties)

// Export classes:
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::base_alarm)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::base_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::instance_identifier)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::model_identifier)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::invalid_context_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::invalid_setup_id_error)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::properties)

// end
