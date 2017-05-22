//! \file  vire/device/device_link_info.h
//! \brief Vire device link info

#ifndef VIRE_DEVICE_DEVICE_LINK_INFO_H
#define VIRE_DEVICE_DEVICE_LINK_INFO_H

// This project:
#include <vire/device/link_end_info.h>
#include <vire/device/slot_utils.h>

namespace vire {

  namespace device {

    //! \brief Information about a device link
    class device_link_info
      : public datatools::i_tree_dumpable
    {
    public:

      //! Default constructor
      device_link_info();

      //! Destructor
      ~device_link_info();

      //! Check validity
      bool is_valid() const;

      //! Reset
      void reset();

      //! Check if the from link end is set
      bool has_from() const;

      //! Return the from link end
      const link_end_info & get_from() const;

      //! Set the from link end
      void set_from(const link_end_info &);

      //! Check if the to link end is set
      bool has_to() const;

      //! Return the to link end
      const link_end_info & get_to() const;

      //! Set the to link end
      void set_to(const link_end_info &);

      /// Parse from strings
      bool parse(const std::string & from_repr_,
                 const std::string & to_repr_,
                 const base_device_model & parent_device_model_);


      //! Return the number of items
      std::size_t get_number_of_items() const;

      //!  Compute the end port slots at given item index
      void fetch_end_slot_path(int item_,
                               slot_path_type & from_slot_path_,
                               slot_path_type & to_slot_path_) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      link_end_info _from_;
      link_end_info _to_;

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_DEVICE_LINK_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
