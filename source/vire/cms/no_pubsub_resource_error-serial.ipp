#ifndef VIRE_CMS_NO_PUBSUB_RESOURCE_ERROR_SERIAL_IPP
#define VIRE_CMS_NO_PUBSUB_RESOURCE_ERROR_SERIAL_IPP

// Ourselves:
#include <vire/cms/no_pubsub_resource_error.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>

namespace vire {

  namespace cms {

    template<class Archive>
    void no_pubsub_resource_error::serialize(Archive & archive_, const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("__base__",
                                                boost::serialization::base_object<vire::utility::base_error>(*this));
      archive_ & boost::serialization::make_nvp("path", _path_);
      return;
    }

  } // end of namespace cms

} // end of namespace vire

#endif // VIRE_CMS_NO_PUBSUB_RESOURCE_ERROR_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
