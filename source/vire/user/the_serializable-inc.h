// vire/user/the_serializable-inc.h

#include <vire/user/user-serial.ipp>
#include <vire/user/group-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::user::user)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::user::user)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::user::group)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::user::group)

// end
