// vire/cmsserver/the_serializable-inc.h

#include <vire/cmsserver/uc_utils-serial.ipp>
#include <vire/cmsserver/uc_composition_description-serial.ipp>
#include <vire/cmsserver/uc_model_description-serial.ipp>
#include <vire/cmsserver/uc_model_db-serial.ipp>
#include <vire/cmsserver/uc_scope_resources-serial.ipp>
#include <vire/cmsserver/uc_functional_resource_specifications-serial.ipp>
#include <vire/cmsserver/uc_resource_mount_link-serial.ipp>

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::uc_port_id)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::uc_resource_mount_link)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::uc_functional_resource_description)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::uc_functional_resource_specifications)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::uc_composition_description)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::uc_model_description)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::uc_model_db)

DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::uc_scope_resources)

// #include <vire/cmsserver/session_reservation-serial.ipp>
// DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::cmsserver::session_reservation)
// BOOST_CLASS_EXPORT_IMPLEMENT(vire::cmsserver::xxx)

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
