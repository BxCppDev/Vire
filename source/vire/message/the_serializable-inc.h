// vire/message/the_serializable-inc.h

#include <vire/message/message_identifier-serial.ipp>
#include <vire/message/message_header-serial.ipp>
#include <vire/message/message_body-serial.ipp>
#include <vire/message/message-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message_identifier)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message_header)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message_body)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::message)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::message::message)


// end
