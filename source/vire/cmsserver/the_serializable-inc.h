// vire/cmsserver/the_serializable-inc.h

#include <vire/cmsserver/session_info-serial.ipp>
#include <vire/cmsserver/process_info-serial.ipp>
#include <vire/cmsserver/session_reservation-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::session_info)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmsserver::session_info)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::process_info)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmsserver::process_info)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::session_reservation)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmsserver::session_reservation)

// end
