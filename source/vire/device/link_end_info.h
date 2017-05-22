//! \file  vire/device/link_end_info.h
//! \brief Vire link end infos

#ifndef VIRE_DEVICE_LINK_END_INFO_H
#define VIRE_DEVICE_LINK_END_INFO_H

// Standard library:
#include <string>
#include <memory>
#include <vector>

// Third party:
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>

// This project:
#include <vire/device/i_instance.h>

namespace vire {

  namespace device {

    class base_device_model;

    /// \brief Information about a link end
    class link_end_info
      : public datatools::i_tree_dumpable
    {
    public:

      /// Information about a node to port
      class level_entry
      {
      public:

        /// Default constructor
        level_entry();

        /// Destructor
        ~level_entry();

        /// Check if name is set
        bool has_name() const;

        /// Set the name
        void set_name(const std::string & name_);

        /// Return the name
        const std::string & get_name() const;

        /// Check if instance is set
        bool has_instance() const;

        /// Set the instance
        void set_instance(const i_instance * inst_ptr_);

        /// Set the instance
        void set_instance(const std::shared_ptr<i_instance> & inst_);

        /// Return the instance
        const i_instance & get_instance() const;

        /// Reset
        void reset();

      private:

        std::string _name_; ///< Node name
        std::shared_ptr<const i_instance> _instance_; ///< Node instance handle

      };

      /// Default constructor
      link_end_info();

      /// Destructor
      ~link_end_info();

      /// Check validity
      bool is_valid() const;

      /// Reset
      void reset();

      /// Return the depth of the linked port path
      std::size_t get_depth() const;

      /// Return the ports path
      const std::vector<level_entry> & get_ports_path() const;

      /// Return the cardinality
      std::size_t get_cardinality() const;

      /// Parse from a string
      bool parse(const std::string & repr_,
                 const base_device_model & parent_device_model_);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      std::vector<level_entry> _ports_path_; ///< Path to connected ports

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_LINK_END_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
