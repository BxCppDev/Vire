#ifndef VIRE_CMS_UNKNOWN_RESOURCES_ERROR_SERIAL_IPP
#define VIRE_CMS_UNKNOWN_RESOURCES_ERROR_SERIAL_IPP

// Ourselves:
#include <vire/cms/unknown_resources_error.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>

namespace vire {

  namespace cms {

    template<class Archive>
    void unknown_resources_error::serialize(Archive & archive_, const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("__base__",
                                                boost::serialization::base_object<vire::utility::base_error>(*this));
      archive_ & boost::serialization::make_nvp("unknown_paths", _unknown_paths_);
      return;
    }

  } // end of namespace cms

} // end of namespace vire

#endif // VIRE_CMS_UNKNOWN_RESOURCES_ERROR_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
