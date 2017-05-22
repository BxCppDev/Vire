
// Ourselves:
#include <vire/device/device_link_info.h>

// This project:
#include <vire/device/slot.h>

namespace vire {

  namespace device {

    device_link_info::device_link_info()
    {
      return;
    }

    device_link_info::~device_link_info()
    {
      return;
    }

    bool device_link_info::is_valid() const
    {
      if (!_from_.is_valid()) return false;
      if (!_to_.is_valid()) return false;
      if (_from_.get_cardinality() != _to_.get_cardinality()) return false;
      return true;
    }

    void device_link_info::reset()
    {
      _from_.reset();
      _to_.reset();
      return;
    }

    bool device_link_info::has_from() const
    {
      return _from_.is_valid();
    }

    const link_end_info & device_link_info::get_from() const
    {
      return _from_;
    }

    void device_link_info::set_from(const link_end_info & from_)
    {
      _from_ = from_;
      return;
    }

    bool device_link_info::has_to() const
    {
      return _to_.is_valid();
    }

    const link_end_info & device_link_info::get_to() const
    {
      return _to_;
    }

    void device_link_info::set_to(const link_end_info & to_)
    {
      _to_ = to_;
      return;
    }

    std::size_t device_link_info::get_number_of_items() const
    {
      if (!is_valid()) {
        return 0;
      }
      return _from_.get_cardinality();
    }

    bool device_link_info::parse(const std::string & from_repr_,
                                 const std::string & to_repr_,
                                 const base_device_model & parent_device_model_)
    {
      DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "Entering...");
      link_end_info from;
      link_end_info to;
      DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "Parsing from...");
      if (!from.parse(from_repr_, parent_device_model_)) {
        DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "Exiting.");
        return false;
      }
      DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "Parsing to...");
      if (!to.parse(to_repr_, parent_device_model_)) {
        DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "Exiting.");
        return false;
      }
      set_from(from);
      set_to(to);
      DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "Ok.");
      DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "Exiting.");
      return true;
    }

    void device_link_info::fetch_end_slot_path(int item_,
                                               slot_path_type & from_slot_path_,
                                               slot_path_type & to_slot_path_) const
    {
      from_slot_path_.clear();
      to_slot_path_.clear();
      DT_THROW_IF(item_ < 0 || item_ >= get_number_of_items(),
                  std::range_error,
                  "Invalid end slot instance index");
      const std::vector<link_end_info::level_entry> & from_lei = _from_.get_ports_path();
      const std::vector<link_end_info::level_entry> & to_lei   = _to_.get_ports_path();
      // for (std::size_t ilvl = 0; ilvl < from_lei.size(); ilvl++) {
      //   std::size_t nb_slots = from_lei[ilvl].get_instance().get_number_of_items();
      //   from_slot_path_.push_back(
      // }

      return;
    }

    void device_link_info::tree_dump(std::ostream & out_,
                                  const std::string & title_,
                                  const std::string & indent_,
                                  bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "From  : ";
      if (!_from_.is_valid()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (_from_.is_valid()) {
        _from_.tree_dump(out_, "", indent_ + "|   ");
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "To  : ";
      if (!_to_.is_valid()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (_to_.is_valid()) {
        _to_.tree_dump(out_, "", indent_ + "|   ");
      }

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << (is_valid()? "<yes>" : "<no>" ) << std::endl;

      return;
    }

  } // end of namespace device

} // end of namespace vire
