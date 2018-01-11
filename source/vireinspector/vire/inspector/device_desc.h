#ifndef VIRE_INSPECTOR_DEVICE_DESC_H
#define VIRE_INSPECTOR_DEVICE_DESC_H

// Standard library:
#include <string>
#include <memory>
#include <iostream>

// Third party:
// - Bayeux/datatools:
// #include <datatools/ui/target_command.h>

// This project:
#include <vire/device/mapping_info.h>

namespace vire {

  namespace inspector {

    //! \brief Device descriptor
    class device_desc
    {
    public:

      device_desc();

      bool is_valid() const;

      void set_full_path(const std::string &);

      const std::string & get_full_path() const;

      bool has_logical_device() const;

      const vire::device::logical_device & get_logical_device() const;

      void set_logical_device(const vire::device::logical_device & logical_);

      bool has_minfo() const;

      const vire::device::mapping_info & get_minfo() const;

      void set_minfo(const vire::device::mapping_info & minfo_);

      friend std::ostream & operator<<(std::ostream &, const device_desc &);

    private:

      std::string _full_path_;
      const vire::device::logical_device * _dev_logical_ = nullptr;
      const vire::device::logical_port *   _port_logical_ = nullptr;
      const vire::device::mapping_info * _minfo_ = nullptr;

    };

    typedef std::shared_ptr<device_desc> device_descriptor_ptr;

  } // end of namespace inspector

} // end of namespace vire

#endif // VIRE_INSPECTOR_DEVICE_DESC_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
