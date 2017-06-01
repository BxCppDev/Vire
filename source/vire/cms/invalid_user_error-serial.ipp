#ifndef VIRE_CMS_INVALID_USER_ERROR_SERIAL_IPP
#define VIRE_CMS_INVALID_USER_ERROR_SERIAL_IPP

// Ourselves:
#include <vire/cms/invalid_user.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>

namespace vire {

  namespace cms {

    template<class Archive>
    void invalid_user::serialize(Archive & archive_, const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("__base__",
                                                boost::serialization::base_object<vire::utility::base_error>(*this));
      archive_ & boost::serialization::make_nvp("invalid_user_name", _invalid_user_name_);
      archive_ & boost::serialization::make_nvp("invalid_user_id", _invalid_user_id_);
      return;
    }

  } // end of namespace cms

} // end of namespace vire

#endif // VIRE_CMS_INVALID_USER_ERROR_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
