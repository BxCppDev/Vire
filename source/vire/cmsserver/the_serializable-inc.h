// vire/cmsserver/the_serializable-inc.h

#include <vire/cmsserver/use_case_info-serial.ipp>
#include <vire/cmsserver/session_reservation-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::use_case_info)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmsserver::use_case_info)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::session_reservation)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmsserver::session_reservation)

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
